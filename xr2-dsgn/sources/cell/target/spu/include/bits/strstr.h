/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004,2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( _Const_return char *, strstr, (const char *s1, const char *s2))
{
  const vector unsigned char zeros = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  const vector unsigned int  unequal = {0x0000ffff, 0x0000ffff, 0x0000ffff, 0x0000ffff};
  vector unsigned char s1_temp, s2_temp, result_equal, result_s1_zero; 
  vector unsigned int  gather_equal, gather_s1_zero;
  
  // Return s1 if s2 is empty
  if (*s2 == '\0')
    return((_Const_return char *)s1);

  // Use existing strstr to find the first character in s2
  for (; (s1 = strchr(s1, *s2)) != 0; ++s1)
  {
    // note we swap s1 and s2 below to get the right kind of compare, this code is similar to strcmp    
    const vector unsigned char *s1_source = (const vector unsigned char *)s2;  
    const vector unsigned char *s2_source = (const vector unsigned char *)s1;  

    // Load the shuffle pattern we need to do an unaligned load

    const vector unsigned char s1_shuffle_pattern = __Shuffles[((unsigned)s1_source & 15)];
    const vector unsigned char s2_shuffle_pattern = __Shuffles[((unsigned)s2_source & 15)];

    while (1)
    {
      s1_temp = spu_shuffle(*s1_source, *(s1_source+1), s1_shuffle_pattern);
      s2_temp = spu_shuffle(*s2_source, *(s2_source+1), s2_shuffle_pattern);
      s1_source++;
      s2_source++;
      // Check for equality and for zero bytes
      result_equal   = spu_cmpeq(s1_temp, s2_temp);
      result_s1_zero = spu_cmpeq(s1_temp, zeros);
      // gather the lsb from each byte into preferred slot of "gather"
      gather_equal   = spu_gather(result_equal);
      gather_s1_zero = spu_gather(result_s1_zero);
      // See if we had an inequality or found a zero
      if ((si_to_uint((qword)gather_equal) != 0xffff) || (si_to_uint((qword)gather_s1_zero) != 0))
      {
        // Find position of the first inequality
        gather_equal = spu_xor(gather_equal,unequal);
        unsigned int match_equal   = si_to_uint((qword)spu_cntlz(gather_equal));
        // Find position of the first zero
        unsigned int match_s1_zero = si_to_uint((qword)spu_cntlz(gather_s1_zero));
        if(match_equal >= match_s1_zero)
        {
          // The first inequality was at or after the zero, so the strings must be equal
          return((_Const_return char *)s1);
        }
        else
        {
          // The first inequality was before we found a zero in s1 return no-match
          break;
        }
      }
    }
  }
  return(0);
}
