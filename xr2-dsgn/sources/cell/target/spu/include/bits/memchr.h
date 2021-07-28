/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004,2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( _Const_return void *, memchr, (const void *s, int c, size_t n))
{   
  /* NOTE: since we're trying to detect equality with our compares,
     whenever we mask two partial quadwords we make the unused bytes of
     one quadword zero and the other one to prevent a match in the
     unused section */

  vector unsigned char mask, result;
  const vector unsigned char zero = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  const vector unsigned char ones = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  vector unsigned int gather;
  unsigned int bitcount;
  _Const_return void *found;
  
  // Make sure n is sane, lets us use more efficient looping structures later
  
  if(n <= 0) return(NULL);
  
  // Splat the value of c into all bytes in the source
  
  const vector unsigned char source = spu_splats((const unsigned char)c);

  // Starting value for the destination

  const vector unsigned char *vdestination = (const vector unsigned char *)s;
  
  // Get the byte offset in the quadword for dest
  
  unsigned destshift = ((unsigned)vdestination) & 0xf;    // figure byte position of first dest byte

  // Fast path for quadword aligned compares

  if(destshift == 0)
  {
 #if 0   // !!!GAC In this case, the #if 0 case is faster, but the improvements are not consistant
    for(; n > 15;n-=16)
    {
      result = spu_cmpeq(*vdestination, source);
      gather = spu_gather(result);
      bitcount = si_to_uint((qword)gather);
      if(bitcount != 0)
      {
        bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
        return((void *)vdestination+bitcount);
      }
      vdestination++;
    }
 #else
    size_t i;
    i = n & (~0xf);
    n = n & 0xf;
    for(; i != 0; i-=16)
    {
      result = spu_cmpeq(*vdestination, source);
      gather = spu_gather(result);
      bitcount = si_to_uint((qword)gather);
      if(bitcount != 0)
      {
        bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
        return((_Const_return void *)((const char*)vdestination+bitcount));
      }
      vdestination++;
    }
 #endif
    
    // Compare the remaining partial quadword

    if(n != 0)
    {
      mask = spu_rlmaskqwbyte(ones, -n);

      // do masked compare
      result = spu_cmpeq(spu_sel(*(vdestination),zero, mask), spu_sel(source, ones, mask));
      gather = spu_gather(result);
      bitcount = si_to_uint((qword)gather);
      if(bitcount != 0)
      {
        bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
        found = (_Const_return char *)vdestination+bitcount;
        return(found);
      }
    }
    return(NULL);
  }

  // From here on handles unaligned destination

  unsigned remainder = 16-destshift;     // this is how many bytes we can copy during the first pass
  mask = spu_slqwbyte(ones, remainder);  // mask to save the first part of the dest quadword
  unsigned endshift = (destshift+n);       // if < 16 we will be copying just one quadword
  
  // If the memset starts and ends in the first word, we need a more involved mask value

  if(endshift < 16)
  {
    mask = spu_or(mask,spu_rlmaskqwbyte(ones, (-endshift)));
  }  
  
  // do masked compare
  result = spu_cmpeq(spu_sel(*(vdestination),zero, mask), spu_sel(source, ones, mask));
  gather = spu_gather(result);
  bitcount = si_to_uint((qword)gather);
  if(bitcount != 0)
  {
    bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
    found = (_Const_return char *)vdestination+bitcount-destshift;
    return(found);
  }
  
  if(endshift < 16)
    return(NULL);

  n -= remainder;
  vdestination = (const vector unsigned char *)(((const unsigned char *)vdestination) + remainder);

  // Now do straight compares for the rest

  while (n >= 16) 
  {
    result = spu_cmpeq(*vdestination, source);
    gather = spu_gather(result);
    bitcount = si_to_uint((qword)gather);
    if(bitcount != 0)
    {
      bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
      found = (_Const_return char *)vdestination+bitcount;
      return(found);
    }
    vdestination++;
    n -= 16;
  }

  // Do the last partial word

  if(n != 0)
  {
    mask = spu_rlmaskqwbyte(ones, -n);

    // do masked compare
    result = spu_cmpeq(spu_sel(*(vdestination),zero, mask), spu_sel(source, ones, mask));
    gather = spu_gather(result);
    bitcount = si_to_uint((qword)gather);
    if(bitcount != 0)
    {
      bitcount = si_to_uint((qword)spu_cntlz(gather)) - 16;
      found = (_Const_return char *)vdestination+bitcount;
      return(found);
    }
  }
  return(NULL);
}
