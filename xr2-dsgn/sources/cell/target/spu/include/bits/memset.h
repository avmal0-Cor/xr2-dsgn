/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

_FUNC_DEF( void*, memset, (void *s, int c, size_t n))
{   
  vector unsigned char mask;
 
  // Make an all-ones value to use for making masks later  
  
  const vector unsigned char ones = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  
  // Splat the value of c (the char we are filling with) into all bytes in the source
  
  const vector unsigned char source = spu_splats((const unsigned char)c);
  
  // Make sure n is sane, lets us use more efficient looping structures later
  
  if(n <= 0) return(s);
  
  // Starting value for the destination

  vector unsigned char *vdestination = (vector unsigned char *)s;
  
  // Get the byte offset in the quadword for dest
  
  unsigned destshift = ((unsigned)vdestination) & 0xf;    // figure byte position of first dest byte

  // Fast path for quadword aligned destination strings

  if(destshift == 0)
  {
    // Copy aligned quadwords till we run out
#if 0   // !!!GAC In this case, the #if 0 case is faster, other places it's not
    for(; n > 15;n-=16)
      *(vdestination++) = source;
#else
    size_t i;
    i = n & (~0xf);
    n = n & 0xf;
    for(; i != 0; i-=16)
      *(vdestination++) = source;
#endif    
    if(n > 0)
    {
      mask = spu_rlmaskqwbyte(ones, -n);
      *vdestination = spu_sel(source, *vdestination, mask);
    }
    return(s);
  }
    
  // From here on handles unaligned destination

  unsigned remainder = 16-destshift;     // this is how many bytes we can copy during the first pass
  mask = spu_slqwbyte(ones, remainder);  // mask to save the first part of the dest quadword
  unsigned endshift = (destshift+n);       // if < 16 we will be copying just one quadword
  
  // If we aren't going to fill the entire trailing part of the quadword, we have to put ones
  // in the mask where we want to save the trailing destination data too.

  if(endshift < 16)
  {
    mask = spu_or(mask,spu_rlmaskqwbyte(ones, (-endshift)));
    *vdestination = spu_sel(source, *vdestination, mask);
    return(s);
  }

  // combine the constant source with the destination

  *vdestination = spu_sel(source, *vdestination, mask);
  
  n -= remainder;
  vdestination = (vector unsigned char *)(((unsigned char *)vdestination) + remainder);

  // Copy the constant into all the remaining whole words

  while (n >= 16) 
  {
    *vdestination = source;
    vdestination++;
    n -= 16;
  }

  // Do the last partial word

  if(n != 0)
  {
    mask = spu_rlmaskqwbyte(ones, -n);
    *vdestination = spu_sel(source, *vdestination, mask);
    return(s);
  }
  return(s);

}
