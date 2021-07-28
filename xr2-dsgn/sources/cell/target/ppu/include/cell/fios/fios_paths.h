/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_paths.h

	Definitions for the path class.
*/
#ifndef __CELL_FIOS_PATHS_H__
#define __CELL_FIOS_PATHS_H__

#include "fios_collections.h"
#include <string.h>

namespace cell {
namespace fios {

class path;
class pathref;
class staticpath;
class md5path;

/** Compares two paths. pathcmp and pathncmp are the bottlenecks for most path comparisons in FIOS
	and can be modified to adjust case-sensitivity behavior.
	\param[in] pA   First path to compare.
	\param[in] pB   Second path to compare.
	\result A numerical value as in strcmp. */
extern FIOS_LINKAGE
int pathcmp(const char *pA, const char *pB) FIOS_ATTRIBUTE_GCC((__pure__));

/** Compares two paths with a length restriction. pathcmp and pathncmp are the bottlenecks for most
	path comparisons in FIOS and can be modified to adjust case-sensitivity behavior.
	\param[in] pA   First path to compare.
	\param[in] pB   Second path to compare.
	\param[in] n    Maximum number of characters to check.
	\result A numerical value as in strncmp. */
extern FIOS_LINKAGE
int pathncmp(const char *pA, const char *pB, size_t n) FIOS_ATTRIBUTE_GCC((__pure__));

#if FIOS_CASE_INSENSITIVE_PATHS
// pathcmp,pathncmp are defined in fios_internal.cpp
#else
// The above two functions are just simple passthrus.
inline int pathcmp(const char *pA, const char *pB) { return strcmp(pA,pB); }
inline int pathncmp(const char *pA, const char *pB, size_t n) { return strncmp(pA,pB,n); }
#endif


/** The pathhash_t type is a pathname hash that allows quick tests for equality (with possible false positives). */
typedef uint32_t           pathhash_t;

/** Hashes a path. This hashcode respects case-sensitivity behavior.
	\param[in] pPath   Path to hash.
	\result A hash code. */
extern FIOS_LINKAGE
pathhash_t pathhash(const char *pPath) FIOS_ATTRIBUTE_GCC((__pure__));


/**
	The path class stores a path and a hash code. If FIOS was initialized with sharedPath = true, paths are
	dynamically allocated and pooled so that they occupy less memory at the risk of heap fragmentation. Otherwise,
	paths are statically allocated (and their buffers must be assigned before use).
*/

class FIOS_LINKAGE path
{
public:
	inline path()                        : m_pBuffer(0), m_hash(0) {}
	inline path(const char *pPath)       : m_pBuffer(0), m_hash(0) { set(pPath); }
	inline path(const path &other)       : m_pBuffer(0), m_hash(0) { set(other); }
	inline path(const pathref &other)    : m_pBuffer(0), m_hash(0) { set(other); }
	inline path(const staticpath &other) : m_pBuffer(0), m_hash(0) { set(other); }
        //	inline ~path()                                                 { set(NULL); }
	
	inline const char * get() const { return m_pBuffer[0] ? m_pBuffer:NULL; }
	void set(const char *pPath);
	void set(const path &other);
	inline void set(const pathref &other);    /* inlined at end of file */
	inline void set(const staticpath &other); /* inlined at end of file */

	inline pathhash_t hash() const     { return m_hash; }
	inline pathhash_t fastHash() const { return m_hash; }
	
	inline operator const char * () const             { return get(); }
	inline path & operator= (const char *pPath)       { set(pPath); return *this; }
	inline path & operator= (const path &other)       { set(other); return *this; }
	inline path & operator= (const pathref &other)    { set(other); return *this; }
	inline path & operator= (const staticpath &other) { set(other); return *this; }
	
	/* Comparison operators */
	friend FIOS_LINKAGE bool operator== (const path &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator!= (const path &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator== (const path &lhs, const pathref &rhs);
	friend FIOS_LINKAGE bool operator!= (const path &lhs, const pathref &rhs);
	friend FIOS_LINKAGE bool operator== (const path &lhs, const staticpath &rhs);
	friend FIOS_LINKAGE bool operator!= (const path &lhs, const staticpath &rhs);
	
	char *      m_pBuffer;
	pathhash_t  m_hash;
};

/** The pathref class stores a pointer to a path and a hash code. (The path is always
	a pointer, so someone else is responsible for the storage.) */
class FIOS_LINKAGE pathref
{
public:
	inline pathref() : m_pPath(NULL), m_hash(0) {}
	inline pathref(const char *pPath)       { set(pPath); }
	inline pathref(const path &other)       { set(other); }
	inline pathref(const pathref &other)    { set(other); }
	inline pathref(const staticpath &other) { set(other); }
	inline ~pathref()                       {}
	
	inline const char * get() const           { return m_pPath; }
	inline void set(const char *pPath)        { m_pPath = pPath; m_hash = pathhash(pPath); }
	inline void set(const path &other)        { m_pPath = other.get(); m_hash = other.hash(); }
	inline void set(const pathref &other)     { m_pPath = other.get(); m_hash = other.hash(); }
	inline void set(const staticpath &other); /* inlined at end of file */

	inline pathhash_t hash() const            { return m_hash; }
	inline pathhash_t fastHash() const        { return m_hash; }
	
	inline pathref & operator= (const char *pPath)       { set(pPath); return *this; }
	inline pathref & operator= (const path &other)       { set(other); return *this; }
	inline pathref & operator= (const pathref &other)    { set(other); return *this; }
	inline pathref & operator= (const staticpath &other) { set(other); return *this; }
	
	inline operator const char * () const { return get(); }
	
	/* Comparison operators */
	friend FIOS_LINKAGE bool operator== (const pathref &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator!= (const pathref &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator== (const pathref &lhs, const pathref &rhs);
	friend FIOS_LINKAGE bool operator!= (const pathref &lhs, const pathref &rhs);
	friend FIOS_LINKAGE bool operator== (const pathref &lhs, const staticpath &rhs);
	friend FIOS_LINKAGE bool operator!= (const pathref &lhs, const staticpath &rhs);

	const char *m_pPath;
	pathhash_t m_hash;
}; /* class pathref */


/** The staticpath class is a simple character buffer of FIOS_PATH_MAX characters. */
class FIOS_LINKAGE staticpath
{
public:
	inline staticpath()                  : m_hash(0) { m_buffer[0] = 0; }
	inline staticpath(const char *pPath) : m_hash(0) { set(pPath); }
	inline ~staticpath() {}
	
	inline const char * get() const          { return m_buffer[0] ? m_buffer:NULL; }
	void setPath(const char *pPath);
	inline void set(const char *pPath)       { setPath(pPath); m_hash = pathhash(pPath); }
	inline void set(const path &other)       { setPath(other); m_hash = other.hash(); }
	inline void set(const pathref &other)    { setPath(other); m_hash = other.hash(); }
	inline void set(const staticpath &other) { setPath(other); m_hash = other.hash(); }
	
	inline pathhash_t hash() const                          { return m_hash; }
	inline pathhash_t fastHash() const                      { return m_hash; }

	inline operator const char * () const                   { return get(); }
	inline staticpath & operator= (const char *pPath)       { set(pPath); return *this; }
	inline staticpath & operator= (const path &other)       { set(other); return *this; }
	inline staticpath & operator= (const pathref &other)    { set(other); return *this; }
	inline staticpath & operator= (const staticpath &other) { set(other); return *this; }

	/* Comparison operators */
	friend FIOS_LINKAGE bool operator== (const staticpath &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator!= (const staticpath &lhs, const path &rhs);
	friend FIOS_LINKAGE bool operator== (const staticpath &lhs, const staticpath &rhs);
	friend FIOS_LINKAGE bool operator!= (const staticpath &lhs, const staticpath &rhs);
	friend FIOS_LINKAGE bool operator== (const staticpath &lhs, const pathref &rhs);
	friend FIOS_LINKAGE bool operator!= (const staticpath &lhs, const pathref &rhs);
	
	pathhash_t m_hash;
	char m_buffer[FIOS_PATH_MAX];
}; /* class staticpath */


/**
	The md5path class is a write-only path. Paths can be stored and checked for equality,
	but they cannot be retrieved after being set. This is because the only internal storage is
	an MD5. 
	
	This class is primarily used by the fios::cache.
*/
class FIOS_LINKAGE md5path
{
public:
	inline md5path()                     { m_MD5[0] = m_MD5[1] = 0; }
	inline md5path(const char *pPath)    { set(pPath); }
	inline md5path(const md5path &other) { m_MD5[0] = other.m_MD5[0]; m_MD5[1] = other.m_MD5[1]; }
	inline ~md5path()                    {}
	
	// no get function!
	void set(const char *pPath);

	inline md5path & operator= (const char *pPath)    { set(pPath); return *this; }
	inline md5path & operator= (const md5path &other) { m_MD5[0] = other.m_MD5[0]; m_MD5[1] = other.m_MD5[1]; return *this; }
	
	static int compare(const void *p1, const void *p2); // comparison function for qsort
	bool decode(const char *pInString); // decodes an MD5 from a 32-char hex string
	char * encode(char *pOutString);    // encodes an MD5 into a 33-char null-terminated hex string
	
	uint64_t m_MD5[2];
}; /* class md5path */


/* Inline functions that couldn't be inlined above */
inline FIOS_LINKAGE void path::set(const pathref &other)             { set(other.get()); }
inline FIOS_LINKAGE void path::set(const staticpath &other)          { set(other.get()); }
inline FIOS_LINKAGE void pathref::set(const staticpath &other)       { m_pPath = other.get(); m_hash = other.hash(); }

/* Operator definitions */
inline FIOS_LINKAGE bool operator== (const path &lhs, const path &rhs)             { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const path &lhs, const pathref &rhs)          { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const path &lhs, const staticpath &rhs)       { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const staticpath &lhs, const path &rhs)       { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const staticpath &lhs, const pathref &rhs)    { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const staticpath &lhs, const staticpath &rhs) { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const pathref &lhs, const path &rhs)          { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const pathref &lhs, const pathref &rhs)       { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const pathref &lhs, const staticpath &rhs)    { if (lhs.hash() != rhs.hash()) return false; return !pathcmp(lhs,rhs); }
inline FIOS_LINKAGE bool operator== (const md5path &lhs, const md5path &rhs)       { return (lhs.m_MD5[0] == rhs.m_MD5[0]) && (lhs.m_MD5[1] == rhs.m_MD5[1]); }

inline FIOS_LINKAGE bool operator!= (const path &lhs, const path &rhs)             { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const path &lhs, const pathref &rhs)          { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const path &lhs, const staticpath &rhs)       { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const staticpath &lhs, const path &rhs)       { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const staticpath &lhs, const pathref &rhs)    { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const staticpath &lhs, const staticpath &rhs) { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const pathref &lhs, const path &rhs)          { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const pathref &lhs, const pathref &rhs)       { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const pathref &lhs, const staticpath &rhs)    { return !(lhs == rhs); }
inline FIOS_LINKAGE bool operator!= (const md5path &lhs, const md5path &rhs)       { return !(lhs == rhs); }


/* Types used everywhere else */
typedef path        path_t;
typedef pathref     pathreference_t;
typedef staticpath  staticpath_t;

}; /* namespace fios */
}; /* namespace cell */

#endif //  __CELL_FIOS_PATHS_H__
