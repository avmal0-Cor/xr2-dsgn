#ifndef _MFileObject
#define _MFileObject
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MFileObject
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS

class MString;

// ****************************************************************************
// CLASS DECLARATION (MFileObject)

//! \ingroup OpenMaya
//! \brief Manipulate Unix filenames and search paths. 
/*!
The MFileObject class implements an object that contains both a
filename and a search path.  The search path is specified by a single
string in which multiple elements are separated by ':' characters.  As
well this string can contain Unix environment variables, specified as
$VARNAME,

Filenames can be produced by the class by combining the path element
with the filename element of the MFileObject.  As well, methods are
available to test for the existance of the files produced.
*/
class OPENMAYA_EXPORT MFileObject
{
public:
				MFileObject();
				MFileObject( const MFileObject & other );
				virtual ~MFileObject();
	MFileObject& operator=( const MFileObject & other );

	MStatus   	setRawName( const MString & fileName );
	MStatus   	setRawPath( const MString & filePath );
	MStatus   	setRawFullName( const MString & fullFileName );

	MStatus   	overrideResolvedFullName( const MString & fullFileName );

	MString  	rawName() const;
	MString  	rawPath() const;
	MString  	rawFullName() const;

	MString  	expandedPath() const;
	MString  	expandedFullName() const;

	MString  	resolvedName() const;
	MString  	resolvedPath() const;
	MString  	resolvedFullName() const;

	unsigned int	pathCount();
	MString  	ithPath( unsigned int index );
	MString  	ithFullName( unsigned int index );
	bool        exists();
	bool        exists( unsigned int index );
	bool        isSet() const;


	//! Options to be used when resolving a file path.
	enum MFileResolveMethod 
	{
		//! (Default) The resolved path is simply the first path after expanding any environment
		//! variables. Will not check if this file actually exists. User must explicitly check 
		//! MFileObject::exists()
		kNone 				= (1<<0),

		//! Checks if expanded paths exist. If paths are relative, assume it's relative to
		//! the current workspace (so check workspace current directory, file-rule directory and
		//! root directory
		kExact 			= (1<<1),

		//! Checks path against mappings defined with the dirmap command. Only for absolute paths
		kDirMap			= (1<<2),

		//! Check path against any previously re-mapped reference locations. If kRelative/kBaseName
		//! are set, then even if we have an absolute path, convert to relative and/or baseName and
		//! look for them in directories provided to the missing reference dialog.
		kReferenceMappings	= (1<<3),

		//! Strips away the project directory, and treats path as relative. Relative to the current
		//! workspace, that is. So look in the workspace current directory, file-rules directory
		//! and the root directory
		kRelative		= (1<<4),

		//! Strips away everything but the base file name and look in the current workspace
		kBaseName		= (1<<5),

		//! Default on file open and import
		kInputFile		= kExact | kDirMap | kRelative | kBaseName,

		//! Default on file reference
		kInputReference		= kInputFile | kReferenceMappings,

		//! Equivalent to the file -strict flag
		kStrict			= kExact | kDirMap
	};

	void					setResolveMethod(MFileResolveMethod method);
	MFileResolveMethod		resolveMethod() const;

	// The following methods are deprecated.
	//
	MStatus   	setName( const MString & fileName );
	MStatus   	setFullName( const MString & fileName );
	MString  	name() const;
	MString  	path() const;
	MString  	fullName() const;

protected:
// No protected members

private:
	void * data;
};

#endif /* __cplusplus */
#endif /* _MFileObject */
