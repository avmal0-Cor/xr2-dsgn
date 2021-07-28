#ifndef _MFnParticleSystem
#define _MFnParticleSystem
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
// CLASS:    MFnParticleSystem
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MFnDagNode.h>

// ****************************************************************************
// DECLARATIONS

class MTime;
class MPoint;
class MVector;
class MFnDagNode;
class MIntArray;
class MPointArray;
class MDoubleArray;
class MVectorArray;
class MIntArray;

// ****************************************************************************
// CLASS DECLARATION (MFnParticleSystem)

//! \ingroup OpenMayaFX MFn
//! \brief Class for obtaining information about a particle system. 
/*!
	Particle object access class.

	\li <b>isValid()</b>: data is valid (predicate)
	\li <b>renderType()</b>: object render type
	\li <b>count()</b>: particle count at current frame


	Use this chart to determine which methods to call based on the
	render type of the particle object:

	<ul>
	<li> Valid Methods for render type kCloud:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		<li> <b>surfaceShading()</b>: object surface shading value
		<li> <b>betterIllum()</b>: invoke thick cloud sampling
		<li> <b>threshold()</b>: object threshold
		<li> <b>disableCloudAxis()</b>: for internal use; do not call
		</ul>

	<li> Valid Methods for render type kTube:
		<ul>
		<li> <b>position0()</b>: particle start position
		<li> <b>position1()</b>: particle end position
		<li> <b>radius0()</b>: particle start radius
		<li> <b>radius1()</b>: particle end radius
		<li> <b>tailSize()</b>: length scale factor
		</ul>

	<li> Valid Methods for render type kBlobby:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		<li> <b>threshold()</b>: object threshold
		</ul>

	<li> Valid Methods for render type kHardware:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		</ul>
    </ul>
*/
class OPENMAYAFX_EXPORT MFnParticleSystem : public MFnDagNode
{
    declareDagMFn(MFnParticleSystem, MFnDagNode);

public:
	//! Ways in which particles can be rendered.
	enum RenderType
	{
		kCloud,		//!< \nop
		kTube,		//!< \nop
		kBlobby,	//!< \nop
		kMultiPoint,	//!< \nop
		kMultiStreak,	//!< \nop
		kNumeric,	//!< \nop
		kPoints,	//!< \nop
		kSpheres,	//!< \nop
		kSprites,	//!< \nop
		kStreak		//!< \nop
	};

	MObject			create                  ( MStatus* status = NULL );
	MObject			create                  ( MObject parent, MStatus* status = NULL );
	MStatus                 emit                    ( const MPoint& position );
	MStatus                 emit                    ( const MPointArray& positionArray );
	MStatus                 emit                    ( const MPoint& position, const MVector& velocity );
	MStatus                 emit                    ( const MPointArray& positionArray, const MVectorArray& velocityArray );
	MStatus                 saveInitialState        () const;
	void			evaluateDynamics        ( MTime &to, bool runupFromStart );
	bool			isValid			() const;
	MString			particleName	        () const;
	unsigned int	        count			() const;
	void	                setCount		( unsigned int ) const;
	RenderType		renderType		() const;
	void			position		( MVectorArray& ) const;
	void			velocity		( MVectorArray& ) const;
	void			acceleration		( MVectorArray& ) const;
	void			position0		( MVectorArray& ) const;
	void			position1		( MVectorArray& ) const;
	void			radius			( MDoubleArray& ) const;
	void			radius0			( MDoubleArray& ) const;
	void			radius1			( MDoubleArray& ) const;
	double			surfaceShading	        () const;
	double			threshold		() const;
	bool			betterIllum		() const;
	bool			disableCloudAxis        () const;
	double			tailSize		() const;
	void			particleIds		( MIntArray& ) const;
	void			age			( MDoubleArray& ) const;
	void			lifespan		( MDoubleArray& ) const;
	void			rgb			( MVectorArray& ) const;
	void			opacity			( MDoubleArray& ) const;
	void			mass			( MDoubleArray& ) const;
	void			emission		( MVectorArray& ) const;
	bool			hasLifespan		() const;
	bool			hasRgb			() const;
	bool			hasOpacity		() const;
	bool			hasEmission		() const;
	bool			primaryVisibility	() const;
	bool			visibleInReflections    () const;
	bool			visibleInRefractions    () const;
	bool			castsShadows		() const;
	bool			receiveShadows		() const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MIntArray&, MStatus* status = NULL ) const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	void                    setPerParticleAttribute ( const MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	void                    setPerParticleAttribute ( const MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	bool                    isPerParticleIntAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isPerParticleDoubleAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isPerParticleVectorAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isDeformedParticleShape ( MStatus* status = NULL ) const;
	MObject                 deformedParticleShape   ( MStatus* status = NULL ) const;
	MObject                 originalParticleShape   ( MStatus* status = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnParticleSystem, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:

	virtual bool	objectChanged( MFn::Type, MStatus * );

private:
// No private members
};

#endif /* __cplusplus */
#endif /* _MFnParticleSystem */