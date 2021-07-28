#ifndef _MFnDependencyNode
#define _MFnDependencyNode
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
// CLASS:    MFnDependencyNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

// ****************************************************************************
// DECLARATIONS

class MDoubleArray;
class MPlugArray;
class MPlug;
class MTypeId;
class MPxNode;
class MObjectArray;
class MCallbackIdArray;

// ****************************************************************************
// CLASS DECLARATION (MFnDependencyNode)

//! \ingroup OpenMaya MFn
//! \brief Dependency node function set.
/*!
  MFnDependencyNode allows the creation and manipulation of dependency
  graph nodes.  Traversal of the dependency graph is possible using
  the getConnections method.

  This function set does not support creation or removal of connections.
  MDGModifier should be used for that purpose.
*/
class OPENMAYA_EXPORT MFnDependencyNode : public MFnBase
{
	declareMFn(MFnDependencyNode, MFnBase);

public:
	//! Specifies the scope of the attribute.
	enum MAttrClass {
		kLocalDynamicAttr = 1,	//!< Dynamically added, applies to this specific node.
		kNormalAttr,		//!< Static attribute which is part of the original definition for this node type.
		kExtensionAttr,		//!< Extension attribute which is part of all nodes of this or derived node types
		kInvalidAttr		//!< None of the above
	};

    MObject         create( const MTypeId &typeId,
								MStatus* ReturnStatus = NULL );
    MObject         create( const MTypeId &typeId,
								const MString& name,
								MStatus* ReturnStatus = NULL );

    MObject         create( const MString &type,
								MStatus* ReturnStatus = NULL );
    MObject         create( const MString &type,
								const MString& name,
								MStatus* ReturnStatus = NULL );

	MTypeId         typeId( MStatus* ReturnStatus = NULL ) const;
	MString         typeName( MStatus* ReturnStatus = NULL ) const;
	MString			name( MStatus * ReturnStatus = NULL ) const;
	MString			pluginName( MStatus * ReturnStatus = NULL ) const;
	MString			setName( const MString &name,
							 MStatus * ReturnStatus = NULL );
	MStatus			getConnections( MPlugArray& array ) const;
	unsigned int		attributeCount( MStatus* ReturnStatus=NULL) const;
	MObject	        attribute(	unsigned int index,
								MStatus* ReturnStatus=NULL) const;
	MObject	        reorderedAttribute(	unsigned int index,
								MStatus* ReturnStatus=NULL) const;
	MObject	        attribute(	const MString& attrName,
								MStatus* ReturnStatus=NULL) const;
	MAttrClass		attributeClass(	const MObject& attr,
									MStatus* ReturnStatus=NULL) const;
	MStatus			getAffectedAttributes ( const MObject& attr,
									MObjectArray& affectedAttributes ) const;
	MStatus			getAffectedByAttributes ( const MObject& attr,
									MObjectArray& affectedByAttributes ) const;
	MPlug			findPlug(	const MObject & attr, bool wantNetworkedPlug,
								MStatus* ReturnStatus=NULL) const;
	MPlug			findPlug(	const MString & attrName, bool wantNetworkedPlug,
								MStatus* ReturnStatus=NULL) const;
	// Obsolete
	MPlug			findPlug(	const MObject & attr,
								MStatus* ReturnStatus=NULL) const;
	// Obsolete
	MPlug			findPlug(	const MString & attrName,
								MStatus* ReturnStatus=NULL) const;
	MStatus			addAttribute	( const MObject & attr );
	MStatus			removeAttribute	( const MObject & attr );
	MPxNode *  		userNode( MStatus* ReturnStatus=NULL ) const;
	bool			isFromReferencedFile(MStatus* ReturnStatus=NULL) const;
	bool			isShared(MStatus* ReturnStatus=NULL) const;
	bool			hasUniqueName(MStatus* ReturnStatus=NULL) const;
	MString			parentNamespace(MStatus* ReturnStatus=NULL) const;
	bool			isLocked(MStatus* ReturnStatus=NULL) const;
	MStatus			setLocked( bool locked );
	static MString	classification( const MString & nodeTypeName );
	bool			isNewAttribute( const MObject& attr,
								MStatus* ReturnStatus=NULL) const;
	static unsigned int	allocateFlag(
						const MString pluginName, MStatus* ReturnStatus=NULL
					);
	static MStatus	deallocateFlag(const MString pluginName, unsigned int flag);
	static MStatus	deallocateAllFlags(const MString pluginName);
	MStatus			setFlag(unsigned int flag, bool state);
	bool			isFlagSet(unsigned int flag, MStatus* ReturnStatus=NULL) const;
	bool			isDefaultNode(MStatus* ReturnStatus=NULL) const;

	MStatus			setDoNotWrite ( bool flag );
	bool			canBeWritten(MStatus* ReturnStatus=NULL) const;

	bool			hasAttribute(const MString& name, MStatus* ReturnStatus=NULL) const;

	MObject			getAliasAttr(bool force, MStatus* ReturnStatus=NULL);
	bool			setAlias(const MString& alias,const MString& name, const MPlug& plug, bool add=true,
					MStatus* ReturnStatus=NULL);
	bool			findAlias(const MString& alias, MObject& attrObj, MStatus* ReturnStatus=NULL) const;
	bool 			getAliasList(MStringArray& strArray, MStatus* ReturnStatus=NULL);
	MString			plugsAlias(const MPlug& plug, MStatus* ReturnStatus=NULL);

	MStatus			setIcon( const MString &filename );
	MString			icon( MStatus* ReturnStatus=NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnDependencyNode, MFnBase );
	//!	NO SCRIPT SUPPORT
	bool			getPlugsAlias(const MPlug& plug, MString& aliasName, MStatus* ReturnStatus=NULL);

END_NO_SCRIPT_SUPPORT:

	// For turning node timing on and off, and querying the state...
	MStatus			dgTimerOn();
	MStatus			dgTimerOff();

	//! Possible states for the node's timer.
	enum MdgTimerState {
		kTimerOff,		//!< \nop
		kTimerOn,		//!< \nop
		kTimerUninitialized,	//!< \nop
		kTimerInvalidState	//!< \nop
	};

	MdgTimerState	dgTimerQueryState( MStatus* ReturnStatus = NULL );

	// For resetting node timers to zero...
	MStatus			dgTimerReset();

	//! The different timer metrics which can be queried.
	enum MdgTimerMetric {
		// Primary metrics. The qualities that we measure.
		kTimerMetric_callback,	//!< Time spent within node callbacks for this node.
		kTimerMetric_compute,	//!< Time spent within the compute method for this node.
		kTimerMetric_dirty,	//!< Time spent propogating dirty messages from this node.
		kTimerMetric_draw,	//!< Time spent drawing this node.
		kTimerMetric_fetch,	//!< Time spent fetching data from plugs.

		// Sub metrics. These are finer-grained breakdowns of the primary
		// metrics. There could be many combinations of the above metrics.
		// Currently we only support the following.
		kTimerMetric_callbackViaAPI,	//!< Time spent in callbacks which were registered through the API.
		kTimerMetric_callbackNotViaAPI,	//!< Time spent in callbacks not registered through the API (i.e internal Maya callbacks).
		kTimerMetric_computeDuringCallback,	//!< Time spent in this node's compute while executing node callbacks on any node.
		kTimerMetric_computeNotDuringCallback	//!< Time spent in this nodes compute when not executing any node callbacks on any nodes.
	};

	//! The types of timers which can be queried.
	enum MdgTimerType {
		/*!
		Time spent performing an operation, not including any time
		spent by child operations. For example, if we are drawing a
		node and that requires a compute, self time will only include
		the time spent drawing and not the compute time. Self time
		measures wall-clock time as opposed to CPU time and the values
		are in seconds.
		*/
		kTimerType_self,

		/*!
		Time spent performing an operation including all time spent by
		child operations. For example, if we are drawing a node and
		that requires a compute, inclusive time is the time for the
		draw plus compute. Inclusive time measure wall-clock time as
		opposed to CPU time and the values are in seconds.
		*/
		kTimerType_inclusive,

		/*!
		The number of operations that occurred. Ideally we should
		return an integer when this timer type is queried, but there
		are two advantages to using a double. 1) it keeps the interface
		consistent and 2) integer has a fixed upper bound of roughly
		four billion so using a double allows us to exceed this.
		*/
		kTimerType_count
	};

	// Query the timer value given a given metric and timer type.
	double 	dgTimer( const MdgTimerMetric			timerMetric,
							const MdgTimerType		timerType,
							MStatus* ReturnStatus = NULL ) const;

	// Query the callback timer values.
	MStatus	dgCallbacks( const MdgTimerType      	type,
							MStringArray			&callbackName,
							MDoubleArray			&value );

	// Query the callbackId timer values.
	MStatus	dgCallbackIds( const MdgTimerType      	type,
							const MString			&callbackName,
							MCallbackIdArray		&callbackId,
							MDoubleArray			&value );

	// Obsolete
	MStatus	addAttribute	( const MObject & attr, MAttrClass type );
	// Obsolete
	MStatus	removeAttribute	( const MObject & attr, MAttrClass type );

protected:
// No protected members

private:
// No private members
};

#endif /* __cplusplus */
#endif /* _MFnDependencyNode */
