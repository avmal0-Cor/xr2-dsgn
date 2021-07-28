/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( char *, strcpy, (char *_Restrict s1, const char *_Restrict s2))
{
  vector unsigned char mask;
  const vector unsigned char ones = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  
  const vector unsigned char zeros = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
  vector unsigned char result;
  vector unsigned int gather;

  // Convert incoming char pointers to (possiblly unaligned) vector chars

  vector unsigned char       *vdestination = (vector unsigned char *)s1;
  const vector unsigned char *vsource      = (const vector unsigned char *)s2;

  // Get the byte offset in the quadword for source and dest
  
  unsigned srcshift  = ((unsigned)vsource) & 0xf;
  unsigned destshift = ((unsigned)vdestination) & 0xf;

  // Fast path for quadword aligned copies

  if(srcshift == 0 && destshift == 0)
  {
    vector unsigned char tsrc;
    // Copy aligned quadwords till we hit a zero terminator
    while(1)
    {
      tsrc = *(vsource++);
      // Check for a zero in the source
      result = spu_cmpeq(tsrc, zeros);
      gather = spu_gather(result);
      // keep copying till we hit a zero terminator 
      if(si_to_uint((qword)gather) == 0)
      {
        *(vdestination++) = tsrc;
      }
      else
      {
        // End game, Find the position of the zero terminator and make a mask
        unsigned int         term_pos  = si_to_uint((qword)spu_cntlz(gather))-15;
        vector unsigned char mask_zero = spu_rlmaskqwbyte(ones, -term_pos);
        // Only copy up to the zero terminator and leave the rest alone
        *vdestination = spu_sel(tsrc, *vdestination, mask_zero);
        return(s1);
      }
    }
  }

  // This is the path for unaligned quadwords

  unsigned remainder = 16-destshift;     // this is how many bytes we could copy during the first pass
  mask = spu_slqwbyte(ones, remainder);  // mask puts ones where we want to save the leading dest data
  
  // Align the source with the destination and put it in tsrc

  int maskshift = (int)srcshift - (int)destshift;
  vector unsigned char tsrc; 
  
  if(maskshift <=0)
  {
    // In this case the source has enough bytes to fill the dest, so we don't need to shuffle
    // Right shift the source to line it up with the destination
    tsrc = spu_rlmaskqwbyte(*vsource,maskshift);
  }
  else
  {
    // In this case we have to get bytes from source & source+1 to get enough bytes to fill the dest,
    // so we do a shuffle that gets the bytes and shifts them left at the same time
    vector unsigned char pattern1 = __Shuffles[maskshift];
    tsrc = spu_shuffle(*vsource, *(vsource+1), pattern1);
  }
  
  // See if there is a zero terminator in the source
  result = spu_cmpeq(tsrc, zeros);
  // Ignore matches in the leading bytes we won't be copying
  result = spu_andc(result, mask);
  gather = spu_gather(result);
  // Generate a mask that covers everything after the zero terminator
  unsigned int term_pos          = si_to_uint((qword)spu_cntlz(gather))-15;
  vector unsigned char mask_zero = spu_rlmaskqwbyte(ones, -term_pos);
  mask = spu_or(mask,mask_zero);
  // combine the source and destination and save it
  *vdestination = spu_sel(tsrc, *vdestination, mask);
  
  // exit now if the source contained a zero
  
  if(si_to_uint((qword)gather) != 0)
    return(s1);
  
  // Advance everything by the number of bytes we just copied
  // This will align the destination address but may NOT align the source
  // This is intentional and used to fetch the right shuffle pattern

  vsource = (const vector unsigned char *)(((const unsigned char *)vsource) + remainder);
  vdestination = (vector unsigned char *)(((unsigned char *)vdestination) + remainder);

  // Load the shuffle pattern we need for the rest of the copy

  vector unsigned char pattern = __Shuffles[((unsigned)vsource & 15)];

  // Copy however many full words there are left
  
  while (1) 
  {
    // Get source word
    tsrc = spu_shuffle(*vsource, *(vsource+1), pattern);
    vsource++;
    // Check for a zero terminator
    result = spu_cmpeq(tsrc, zeros);
    gather = spu_gather(result);
    // Did we find one?
    if(si_to_uint((qword)gather) == 0)
    {
      // No terminator, copy the whole word
      *(vdestination++) = tsrc;
    }
    else
    {
      // Generate a mask that covers everything after the zero terminator
      unsigned int lterm_pos          = si_to_uint((qword)spu_cntlz(gather))-15;
      vector unsigned char lmask_zero = spu_rlmaskqwbyte(ones, -lterm_pos);
      // combine the source and destination and save it
      *vdestination = spu_sel(tsrc, *vdestination, lmask_zero);
      return(s1);
    }
  }
}

