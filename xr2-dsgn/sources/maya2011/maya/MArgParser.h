#ifndef _MArgParser
#define _MArgParser
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
// CLASS:    MArgParser
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MArgParser)
//
// This class is used to parse and store all of the flags, arguments
// and objects which are passed to a command.  The MArgParser accepts
// an MSyntax object, which describes the format for a command, and uses
// it to parse the command arguments into a form which is easy to query.
//
// ***************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS

class MSyntax;
class MSelectionList;
class MStringArray;
class MArgList;
class MDistance;
class MAngle;
class MTime;

// ****************************************************************************
// CLASS DECLARATION (MArgParser)

//! \ingroup OpenMaya
//! \brief Command argument list parser.
/*!
This class is used to parse and store all of the flags, arguments
and objects which are passed to a command.  The MArgParser accepts
an MSyntax object, which describes the format for a command, and uses
it to parse the command arguments into a form which is easy to query.
The documentation for the MSyntax command contains the details on three
types of arguments to commands:

\li <b>flags arguments</b> preceded by a '-' character.
\li <b>command arguments</b> required parameters that follow the flags
\li <b>objects</b> an optional list of Maya objects or the contents
of the selection list.

Once the syntax for your command has been specified, when the <i>doIt</i>
method for the command it called, the provided MArgList instance can
be passed to the MArgParser constructor along with the command's syntax
object to parse those arguments.  If the parsing is unsuccessful, then
the provided command was syntactically invalid.  If successful, the
methods in this class can be used to extract the values of the arguments
to the command.

The <i>isFlagSet</i> method can be used to find out if a particular flag
argument was provided in the command invocation, and the <i>getFlagArgument</i>
methods are used to return the value of that flag.

If the command requires <i>command arguments</i> (i.e. the <i>addArg</i>
method was used to construct the syntax object), then <i>getCommandArgument</i>
methods are used to return the values of those arguments.

If the command requires <i>objects</i> then the <i>getObjects</i> methods
are used to return those objects either as a selection list of as a list
of strings.

\note
<i>command arguments</i> and <i>objects</i> are incompatible
and must not be combined in the syntax definition of a command. If both
have been specified to the Syntax object, then neither the
<i>getCommandArgument()</i> or <i>getObjects()</i> methods will work predictably.
*/
class OPENMAYA_EXPORT MArgParser
{
public:
			MArgParser();

			MArgParser			(const MSyntax &syntax,
								 const MArgList &argList,
								 MStatus *ReturnStatus = NULL);

	virtual ~MArgParser();

	bool	isFlagSet			(const char *flag,
								 MStatus *ReturnStatus = NULL) const;

	bool	isEdit				(MStatus *ReturnStatus = NULL) const;

	bool	isQuery				(MStatus *ReturnStatus = NULL) const;


	unsigned int numberOfFlagsUsed	() const;

	unsigned int numberOfFlagUses	(const char *flag) const;

BEGIN_NO_SCRIPT_SUPPORT:

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 bool &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 int &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 double &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MString &result) const;

	MStatus getFlagArgument		(const char *flag, unsigned int index,
								 unsigned int &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MDistance &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MAngle &result) const;

	MStatus	getFlagArgument		(const char *flag, unsigned int index,
								 MTime &result) const;

END_NO_SCRIPT_SUPPORT:

	bool	flagArgumentBool		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL ) const;

	int		flagArgumentInt			(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL ) const;

	double	flagArgumentDouble		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MString	flagArgumentString		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MDistance flagArgumentMDistance	(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MAngle	flagArgumentMAngle		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;

	MTime	flagArgumentMTime		(const char *flag, unsigned int index,
								 	 MStatus *ReturnStatus = NULL) const;


	MStatus getFlagArgumentPosition	(const char *flag, unsigned int i,
								 	 unsigned int &position) const;

	MStatus getFlagArgumentList		(const char *flag, unsigned int i,
								 	 MArgList& args) const;

BEGIN_NO_SCRIPT_SUPPORT:

	MStatus	getCommandArgument	(unsigned int index, bool &result) const;

	MStatus	getCommandArgument	(unsigned int index, int &result) const;

	MStatus	getCommandArgument	(unsigned int index, double &result) const;

	MStatus	getCommandArgument	(unsigned int index, MString &result) const;

	MStatus	getCommandArgument	(unsigned int index, MDistance &result) const;

	MStatus	getCommandArgument	(unsigned int index, MAngle &result) const;

	MStatus	getCommandArgument	(unsigned int index, MTime &result) const;

END_NO_SCRIPT_SUPPORT:

	bool commandArgumentBool	(unsigned int index, MStatus *ReturnStatus = NULL ) const;

	int	commandArgumentInt	(unsigned int index, MStatus *ReturnStatus = NULL ) const;

	double commandArgumentDouble (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MString	commandArgumentString (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MDistance commandArgumentMDistance (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MAngle commandArgumentMAngle (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MTime commandArgumentMTime (unsigned int index, MStatus *ReturnStatus = NULL) const;

	MStatus	getObjects			(MStringArray &result) const;

protected:
	bool fOwn;
	void *apiData;

private:
	const char *className() const;

	MArgParser (void *);
};

#endif /* __cplusplus */
#endif /* _MArgParser */