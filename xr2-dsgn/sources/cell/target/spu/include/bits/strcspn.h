/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( size_t, strcspn, (const char *s1, const char *s2))
{   
  const vector unsigned char zeros = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
  vector unsigned char treject, result_zero, result_search; 
  vector unsigned int  gather_zero, gather_search;

  // Temporary pointer to the source character

  const char* vsource = s1;

  // Load the shuffle pattern we need to do an unaligned load on the reject string

  const vector unsigned char shuffle_pattern = __Shuffles[((unsigned)s2 & 15)];

  // Now for every character in s1, see if we can find it in the reject string
  for(;*vsource!=0;vsource++)
  {
    // Splat this character across a quadword
    // !!!GAC this cast should not be necessary
    const vector unsigned char search_pattern  = (vector unsigned char)spu_splats(*vsource);
    // Reset to the beginning of the reject string
    const vector unsigned char *vreject = (const vector unsigned char *)s2;  
    // Search for a match in the reject string, we don't care where
    while (1)
    {
      // Load next 16 bytes of reject string
      treject = spu_shuffle(*vreject, *(vreject+1), shuffle_pattern);
      vreject++;
      // Look for a match or a zero in the reject string
      result_zero   = spu_cmpeq(treject, zeros);
      result_search = spu_cmpeq(treject, search_pattern);
      // gather the lsb from each byte into preferred slot of "gather"
      gather_zero   = spu_gather(result_zero);
      gather_search = spu_gather(result_search);
      // if the preferred slot of either gather is nonzero, we matched somewhere
      if ((si_to_uint((qword)gather_zero) != 0) || (si_to_uint((qword)gather_search) != 0))
      {
        unsigned int match_zero = si_to_uint((qword)spu_cntlz(gather_zero));
        unsigned int match_search = si_to_uint((qword)spu_cntlz(gather_search));
        if (match_zero < match_search)
        {
          // The zero came first, so no match, continue to next source character
          break;
        }
        else
        {
          // Matched a character in the reject string, return where
          return(vsource-s1);
        }
      }
    }
  }
  return(vsource-s1);
}

