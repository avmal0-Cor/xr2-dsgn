/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef CGNV2RTSTR_HEADER
#define CGNV2RTSTR_HEADER

// Adds STR string at the end of the string table and returns the
// index of the new string.
#include <string.h>

static unsigned int stringTableAdd( STL_NAMESPACE vector<char> &stringTable, const char* str )
{
	unsigned int ret = (unsigned int)stringTable.size();

	if ( ret == 0 )
	{
		stringTable.push_back('\0'); //add the empty string
		ret = 1;
	}

	size_t stringLength = strlen(str) + 1;
	stringTable.resize(ret + stringLength);
	memcpy(&stringTable[0] + ret,str,stringLength);

	return ret;
}

static unsigned int stringTableFind( STL_NAMESPACE vector<char> &stringTable, const char* str  )
{
	const char* data = &stringTable[0];
	size_t size = stringTable.size();
	const char *end = data + size;

	size_t length = strlen(str);
	if (length+1 > size)
		return 0;
	data += length;

	const char *p = (char*)memchr(data,'\0',end-data);
	while (p && (end-data)>0)
	{
		if (!memcmp(p - length, str, length))
		{
			//found
			return (unsigned int)(p - length - &stringTable[0]);
		}
		data = p+1;	
		p = (char*)memchr(data,'\0',end-data);
	}
	return 0;
}

static unsigned int stringTableAddUnique( STL_NAMESPACE vector<char> &stringTable, const char* str )
{
	if ( stringTable.size() == 0 )
		stringTable.push_back('\0'); //add the empty string
	unsigned int ret = stringTableFind(stringTable, str);
	if (ret == 0 && str[0] != '\0')
		ret = stringTableAdd(stringTable, str);
	return ret;
}

#endif
