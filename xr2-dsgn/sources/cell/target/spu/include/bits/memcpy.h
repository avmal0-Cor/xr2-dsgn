/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004,2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

_FUNC_DEF( void*, memcpy, (void *_Restrict s1, const void *_Restrict s2, size_t n))
{
// Loading from this table takes one instruction, generating the shuffle pattern would take three and still require a load
// If you call memcpy (or other functions that share this table) more than a few times, there will be a savings
// in memory size.
    
  vector unsigned char mask;
  const vector unsigned char ones = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   
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
    // Copy aligned quadwords till we run out
#if 1   // !!!GAC In this case, the #if 1 case is faster, other places it's not
    for(; n > 15;n-=16)
      *(vdestination++) = *(vsource++);
#else
    size_t i;
    i = n & (~0xf);
    n = n & 0xf;
    for(; i != 0; i-=16)
      *(vdestination++) = *(vsource++);
#endif
    // Copy the remaining partial quadword and return
    if(n > 0)
    {
      mask = spu_rlmaskqwbyte(ones, -n);
      *vdestination = spu_sel(*vsource, *vdestination, mask);
    }
    return(s1);
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
  
  // combine the source and destination and save it
  
  *vdestination = spu_sel(tsrc, *vdestination, mask);
  
  // exit now if this was the last byte
  
  if(endshift < 16)
    return(s1);
  
  // Advance everything by the number of bytes we just copied
  // This will align the destination address but may NOT align the source
  // This is intentional and used to fetch the right shuffle pattern

  n -= remainder;
  vsource = (const vector unsigned char *)(((const unsigned char *)vsource) + remainder);
  vdestination = (vector unsigned char *)(((unsigned char *)vdestination) + remainder);

  // Load the shuffle pattern we need for the rest of the copy

  vector unsigned char pattern = __Shuffles[((unsigned)vsource & 15)];

  // Copy however many full words there are left

  while (n >= 16) 
  {
    *vdestination = spu_shuffle(*vsource, *(vsource+1), pattern);
    vsource++;
    vdestination++;
    n -= 16;
  }

  // Do the last partial word

  if(n != 0)
  {
    mask = spu_rlmaskqwbyte(ones, -n);
    tsrc = spu_shuffle(*vsource, *(vsource+1), pattern);
    *vdestination = spu_sel(tsrc, *vdestination, mask);
    return(s1);
  }
  return(s1);

}
