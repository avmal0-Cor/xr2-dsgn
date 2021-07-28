/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( _Const_return char *, strrchr, (const char *s, int c))
{   
  vector unsigned char tsource;
  unsigned int  gather_zero, gather_search;
  unsigned int first_zero, first_search;
  unsigned int start = (unsigned)s & 15;
  int last_match = -1;

  // Align the char * to 16 bytes.
  const vector unsigned char *vsource = (const vector unsigned char *)(s-start);  

  const vector unsigned char search_pattern  = spu_splats((const unsigned char)c);

  tsource = *vsource++;
  gather_zero   = spu_extract(spu_gather(spu_cmpeq(tsource, (unsigned char)0)), 0);
  gather_search = spu_extract(spu_gather(spu_cmpeq(tsource, search_pattern)), 0);

  /* Mask off the bits which correspond to chars before the input. */
  gather_zero   &= 0xffffu >> start;
  gather_search &= 0xffffu >> start;
 
  first_zero    = spu_extract(spu_cntlz(spu_promote(gather_zero, 0)), 0);

  while (first_zero >= 32)
    {
      gather_search &= (~gather_search) + 1;  // Remove all but the last match.

      first_search  = spu_extract(spu_cntlz(spu_promote(gather_search, 0)), 0);

      if (first_search < 32)
	last_match = (unsigned)vsource - (unsigned)s - 32 + first_search;

      tsource = *vsource++;
      gather_zero   = spu_extract(spu_gather(spu_cmpeq(tsource, (unsigned char)0)), 0);
      gather_search = spu_extract(spu_gather(spu_cmpeq(tsource, search_pattern)), 0);

      first_zero    = spu_extract(spu_cntlz(spu_promote(gather_zero, 0)), 0);
    }

  gather_search &= ~(0xffffffffu >> (first_zero+1));  // Remove any matches after the zero.
  gather_search &= (~gather_search) + 1;  // Remove all but the last match.

  first_search  = spu_extract(spu_cntlz(spu_promote(gather_search, 0)), 0);

  if (first_search < 32)
    last_match = (unsigned)vsource - (unsigned)s - 32 + first_search;

  if (last_match < 0)
    return 0;

  return (_Const_return char *)(s+last_match);
}
