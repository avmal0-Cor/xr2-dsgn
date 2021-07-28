/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

_FUNC_DEF( int, memcmp, (const void *s1 , const void *s2, size_t n ))
{
  vector unsigned char s1_temp, s2_temp, result_equal; 
  vector unsigned int  gather_equal;

  // Change the input string into a vector (probably unaligned)

  const vector unsigned char *s1_source = (const vector unsigned char *)s1;  
  const vector unsigned char *s2_source = (const vector unsigned char *)s2;  

  // Load the shuffle pattern we need to do an unaligned load from each source

  const vector unsigned char s1_shuffle_pattern = __Shuffles[((unsigned)s1_source & 15)];
  const vector unsigned char s2_shuffle_pattern = __Shuffles[((unsigned)s2_source & 15)];
  n += 32;
  
  while (1)
  {
    n -= 16;
    s1_temp = spu_shuffle(*s1_source, *(s1_source+1), s1_shuffle_pattern);
    s2_temp = spu_shuffle(*s2_source, *(s2_source+1), s2_shuffle_pattern);
    s1_source++;
    s2_source++;
    // Check for inequality
    result_equal   = spu_cmpeq(s1_temp, s2_temp);
    gather_equal   = spu_gather(result_equal);
    // See if we had an inequality or if this is the last quadword
    if ((si_to_uint((qword)gather_equal) != 0xffff) || n < 33)
    {
      // Find position of the first inequality
      // Flip the lower 16 bits in the gather so inequalities will be 1
      // then count leading zeros to find the first inequality
      unsigned int match_equal   = si_to_uint((qword)spu_cntlz(spu_xor(gather_equal,0xffffU)));
      // The first inequality was after the last byte, so strings are equal
      if (match_equal >= n)
      {
        return(0);
      }
      else
      {
      // The first inequality was before the last byte
      vector unsigned char result_gt  = spu_cmpgt(s1_temp, s2_temp);
      vector unsigned int  gather_gt  = spu_gather(result_gt);
      unsigned int         match_gt   = si_to_uint((qword)spu_cntlz(gather_gt));
      return((match_gt == match_equal)? 1 : -1);
      }
    }
  }
}
