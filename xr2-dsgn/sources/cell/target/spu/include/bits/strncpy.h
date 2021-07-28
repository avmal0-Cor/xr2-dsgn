/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2007 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(char *, strncpy,(char *_Restrict s1, const char *_Restrict s2, size_t n))
{
  vector unsigned char mask;

  // !!!TREVOR substituting this for the ones=spu_orc(ones,ones) makes no difference
  // making this "const" or not slows the unaligned case by 5 cycles, speeds up the aligned by 3
  const vector unsigned char ones = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  
  const vector unsigned char zeros = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
  vector unsigned char stemp, result;
  vector unsigned int  gather;

  // Some of the loops below expect n to be at least 1

  if(n <= 0) return(s1);

  // Convert incoming char pointers to (possiblly unaligned) vector chars

  vector unsigned char       *vdestination = (vector unsigned char *)s1;
  const vector unsigned char *vsource      = (const vector unsigned char *)s2;

  // Get the byte offset in the quadword for source and dest
  
  unsigned srcshift  = ((unsigned)vsource) & 0xf;
  unsigned destshift = ((unsigned)vdestination) & 0xf;

  // Fast path for quadword aligned copies

  if(srcshift == 0 && destshift == 0)
  {
    // Copy aligned quadwords till we hit a zero or 'n' runs out
    while(1)
    {
      stemp = *(vsource++);
      // Check for a zero in the source
      result = spu_cmpeq(stemp, zeros);
      gather = spu_gather(result);
      // keep copying till we hit a zero terminator or get to the last quadword, then break 
      if ((si_to_uint((qword)gather) == 0) && (n > 16))
      {
        *(vdestination++) = stemp;
      }
      else
      {
        break;
      }
      n -= 16;
    }
    // Which endgame do we do?
    if(n <= 16)
    {
      // In this case, N has run out so the value in stemp will be the last one we touch
      // Make a mask so we only access the last "n" bytes
      vector unsigned char mask_n = spu_rlmaskqwbyte(ones, -n);
      // Find position of zero terminator (if any) and make a mask to zero everything after it
      unsigned int term_pos          = si_to_uint((qword)spu_cntlz(gather))-16;
      vector unsigned char mask_zero = spu_rlmaskqwbyte(ones, -term_pos);
      // Zero out everything after the zero terminator
      stemp = spu_sel(stemp, zeros, mask_zero);
      // Copy into the last "n" bytes
      *vdestination = spu_sel(stemp, *vdestination, mask_n);
      return(s1);
    }
    else
    {
      // Here we hit a zero terminator before the N ran out, so we need to zero fill the
      // remaining quadwords right up to the end
      // Find position of zero terminator make a mask and zero everything after it
      unsigned int term_pos = si_to_uint((qword)spu_cntlz(gather))-16;
      vector unsigned char mask_zero = spu_rlmaskqwbyte(ones, -term_pos);
      *(vdestination++) = spu_sel(stemp, zeros, mask_zero);
      n -= 16;
      while(1)
      {
        if(n > 16)
        {
          *(vdestination++) = zeros;
        }
        else
        {
          // Only zero the last 'n' bytes
          vector unsigned char mask_n = spu_rlmaskqwbyte(ones, -n);
          *vdestination = spu_sel(zeros, *vdestination, mask_n);
          return(s1);
        }
        n-=16;
      }
    }

  }
  // This is the path for unaligned quadwords

  unsigned remainder = 16-destshift;     // this is how many bytes we could copy during the first pass
  mask = spu_slqwbyte(ones, remainder);  // mask puts ones where we want to save the leading dest data
  unsigned endshift = destshift+n;       // if < 16 we will be copying just one quadword
  
  // If we don't have enough source data to fill the trailing part of the quadword, we have to put ones
  // in the mask where we want to save the trailing destination data too.

  if(endshift < 16)
  {
    mask = spu_or(mask,spu_rlmaskqwbyte(ones, -(endshift)));
  }

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
  
  // Check for a zero terminator
  result = spu_cmpeq(tsrc, zeros);
  // Ignore matches in bytes we're not going to copy
  result = spu_andc(result, mask);
  gather = spu_gather(result);
  // Generate a mask from the first zero found to the end (mask will be zero if no zeros found)
  unsigned int term_pos          = si_to_uint((qword)spu_cntlz(gather))-16;
  vector unsigned char mask_zero = spu_rlmaskqwbyte(ones, -term_pos);
  // zero all the bytes after the terminator
  tsrc = spu_sel(tsrc, zeros, mask_zero);

  // combine the source and destination and save it
  
  *vdestination = spu_sel(tsrc, *vdestination, mask);

  // exit now if this was the last byte
  
  if(endshift < 16)
  {
     return(s1);
  }
  
  // Advance everything by the number of bytes we just copied
  // This will align the destination address but may NOT align the source
  // This is intentional and used to fetch the right shuffle pattern

  n -= remainder;
  vsource = (const vector unsigned char *)(((const unsigned char *)vsource) + remainder);
  vdestination = (vector unsigned char *)(((unsigned char *)vdestination) + remainder);

  // If we found a zero above, pad out to "n" with zeros
  
  if(si_to_uint((qword)gather) != 0)
  {
    while(1)
    {
      if(n > 16)
      {
        *(vdestination++) = zeros;
      }
      else
      {
        // Only zero the last 'n' bytes
        vector unsigned char mask_n = spu_rlmaskqwbyte(ones, -n);
        *vdestination = spu_sel(zeros, *vdestination, mask_n);
        return(s1);
      }
      n-= 16;
    }
  }

  // Load the shuffle pattern we need for the rest of the copy

  vector unsigned char pattern = __Shuffles[((unsigned)vsource & 15)];

  // Copy however many full words there are left

  while (1) 
  {
    // Get source word
    tsrc = spu_shuffle(*vsource, *(vsource+1), pattern);
    // Check for a zero terminator
    result = spu_cmpeq(tsrc, zeros);
    gather = spu_gather(result);
    if ((si_to_uint((qword)gather) == 0) && (n > 16))
    {
      *(vdestination++) = tsrc;
    }
    else
    {
      break;
    }
    vsource++;
    n -= 16;
  }
  
  // Which endgame do we do?
  if(n <= 16)
  {
    // In this case, N has run out so the value in stemp will be the last one we touch
    // Make a mask so we only access the last "n" bytes
    vector unsigned char mask_n    = spu_rlmaskqwbyte(ones, -n);
    // Find position of zero terminator (if any) and make a mask to zero everything after it
    unsigned int lterm_pos          = si_to_uint((qword)spu_cntlz(gather))-16;
    vector unsigned char lmask_zero = spu_rlmaskqwbyte(ones, -lterm_pos);
    // Zero out everything after the zero terminator
    tsrc = spu_sel(tsrc, zeros, lmask_zero);
    // Copy into the last "n" bytes
    *vdestination = spu_sel(tsrc, *vdestination, mask_n);
    return(s1);
  }
  else
  {
    // Here we hit a zero terminator before the N ran out, so we need to zero fill the
    // remaining quadwords right up to the end
    // Find position of zero terminator make a mask and zero everything after it
    unsigned int lterm_pos = si_to_uint((qword)spu_cntlz(gather))-16;
    vector unsigned char lmask_zero = spu_rlmaskqwbyte(ones, -lterm_pos);
    *(vdestination++) = spu_sel(tsrc, zeros, lmask_zero);
    n -= 16;
    while(1)
    {
      if(n > 16)
      {
        *(vdestination++) = zeros;
      }
      else
      {
        // Only zero the last 'n' bytes
        vector unsigned char mask_n = spu_rlmaskqwbyte(ones, -n);
        *vdestination = spu_sel(zeros, *vdestination, mask_n);
        return(s1);
      }
      n-=16;
    }
  }
  return(s1);

}
