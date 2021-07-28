/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2007 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file psarclib_md5.h
	Defines the cell::fios::PSArc::MD5 class for creating MD5 digests.
*/
#ifndef __CELL_PSARCLIB_MD5_H___
#define __CELL_PSARCLIB_MD5_H___

#include <cell/fios/compression/compression_defines.h>

namespace cell {
namespace fios {
namespace PSArc {

// MD5.CC - source code for the C++/object oriented translation and 
//          modification of MD5.

// Translation and modification (c) 1995 by Mordechai T. Abzug 

// This translation/ modification is provided "as is," without express or 
// implied warranty of any kind.

// The translator/ modifier does not claim (1) that MD5 will do what you think 
// it does; (2) that this translation/ modification is accurate; or (3) that 
// this software is "merchantible."  (Language for this disclaimer partially 
// copied from the disclaimer below).

#define MD5_DIGEST_SIZE 16

/* based on:

   MD5.H - header file for MD5C.C
   MDDRIVER.C - test driver for MD2, MD4 and MD5

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

typedef uint32_t uint4;
typedef uint16_t uint2;
typedef uint8_t uint1;

class COMPRESSION_EXPORT MD5 {

public:

// methods for controlled operation:
  MD5              ();  // simple initializer
  void  update     (const uint1 *input, uint4 input_length);
  void updateString(const char *string, bool uppercaserize);

  void  finalize   ();

// constructors for special circumstances.  All these constructors finalize the MD5 context.
  MD5              (const char *string, bool uppercaserize); // digest string, finalize

// methods to acquire finalized result
  const unsigned char *peekDigest() { return digest; }

  void init             ();               // called by all constructors

private:

// first, some types:

// next, the private data:
  uint4 state[4];
  uint4 count[2];     // number of *bits*, mod 2^64
  uint1 buffer[64];   // input buffer
  uint1 digest[16];
  uint1 finalized;

// last, the private methods, mostly static:
  void transform        (const uint1 *buffer);  // does the real update work.  Note 
                                          // that length is implied to be 64.

  static void encode    (uint1 *dest, uint4 *src, uint4 length);
  static void decode    (uint4 *dest, const uint1 *src, uint4 length);

  static inline uint4  rotate_left (uint4 x, uint4 n);
  static inline uint4  F           (uint4 x, uint4 y, uint4 z);
  static inline uint4  G           (uint4 x, uint4 y, uint4 z);
  static inline uint4  H           (uint4 x, uint4 y, uint4 z);
  static inline uint4  I           (uint4 x, uint4 y, uint4 z);
  static inline void   FF  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
  static inline void   GG  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
  static inline void   HH  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
  static inline void   II  (uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);

};

};	// namespace PSArc
};	// namespace fios
};	// namespace cell

#endif /* __CELL_PSARCLIB_MD5_H___ */
