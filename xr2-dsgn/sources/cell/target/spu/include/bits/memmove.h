/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

_FUNC_DEF( void*, memmove, (void *s1, const void *s2, size_t n))
{ /* copy char s2[n] to s1[n] safely */
  char *sc1 = (char *)s1;
  const char *sc2 = (const char *)s2;

  if (sc2 < sc1 && sc1 < sc2 + n)
    for (sc1 += n, sc2 += n; 0 < n; --n)
      *--sc1 = *--sc2;  /* copy backwards */
  else
  {
    return memcpy(s1, s2, n);
  }
  return(s1);
}

