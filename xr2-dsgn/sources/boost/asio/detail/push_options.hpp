//
// push_options.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// No header guard

#if defined(__COMO__)

// Comeau C++

#elif defined(__DMC__)

// Digital Mars C++

#elif defined(__INTEL_COMPILER) || defined(__ICL) \
  || defined(__ICC) || defined(__ECC)

// Intel C++

#elif defined(__GNUC__)

// GNU C++

# if defined(__MINGW32__) || defined(__CYGWIN__)
#  pragma pack (push, 8)
# endif

#elif defined(__KCC)

// Kai C++

#elif defined(__sgi)

// SGI MIPSpro C++

#elif defined(__DECCXX)

// Compaq Tru64 Unix cxx

#elif defined(__ghs)

// Greenhills C++

#elif defined(__BORLANDC__)

// Borland C++

# pragma option push -a8 -b -Ve- -Vx- -w-inl -vi-
# pragma nopushoptwarn
# pragma nopackwarning
# if !defined(__MT__)
#  error Multithreaded RTL must be selected.
# endif // !defined(__MT__)

#elif defined(__MWERKS__)

// Metrowerks CodeWarrior

#elif defined(__SUNPRO_CC)

// Sun Workshop Compiler C++

#elif defined(__HP_aCC)

// HP aCC

#elif defined(__MRC__) || defined(__SC__)

// MPW MrCpp or SCpp

#elif defined(__IBMCPP__)

// IBM Visual Age

#elif defined(_MSC_VER)

// Microsoft Visual C++
//
// Must remain the last #elif since some other vendors (Metrowerks, for example)
// also #define _MSC_VER

# pragma warning (disable:4103)
# pragma warning (push)
# pragma warning (disable:4127)
# pragma warning (disable:4244)
# pragma warning (disable:4355)
# pragma warning (disable:4512)
# pragma warning (disable:4675)
# pragma warning (disable:4100)
# if defined(_M_IX86) && defined(_Wp64)
// The /Wp64 option is broken. If you want to check 64 bit portability, use a
// 64 bit compiler!
#  pragma warning (disable:4311)
#  pragma warning (disable:4312)
# endif // defined(_M_IX86) && defined(_Wp64)
# pragma pack (push, 8)
// Note that if the /Og optimisation flag is enabled with MSVC6, the compiler
// has a tendency to incorrectly optimise away some calls to member template
// functions, even though those functions contain code that should not be
// optimised away! Therefore we will always disable this optimisation option
// for the MSVC6 compiler.
# if (_MSC_VER < 1300)
#  pragma optimize ("g", off)
# endif
# if !defined(_MT)
#  error Multithreaded RTL must be selected.
# endif // !defined(_MT)

#endif
