/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( size_t, strlen, (const char *s))
{
  vector unsigned char tsource;
  unsigned int gather_zero;
  unsigned int first_zero;
  unsigned int start = (unsigned)s & 15;

  // Align the char * to 16 bytes.
  const vector unsigned char *vsource = (const vector unsigned char *)(s-start);  

  tsource = *vsource++;

  gather_zero   = spu_extract(spu_gather(spu_cmpeq(tsource, (unsigned char)0u)), 0);

  /* Mask off the bits which correspond to chars before the input. */
  gather_zero &= (0xffffu >> start);
 
  while (__builtin_expect(gather_zero == 0, 1))
    {
      tsource = *vsource++;
      gather_zero   = spu_extract(spu_gather(spu_cmpeq(tsource, (unsigned char)0u)),0) << 16;
      tsource = *vsource++;
      gather_zero  |= spu_extract(spu_gather(spu_cmpeq(tsource, (unsigned char)0u)),0);
    }

  first_zero    = spu_extract(spu_cntlz(spu_promote(gather_zero, 0)), 0);

  return (unsigned)vsource - (unsigned)s - 32 + first_zero;
}

