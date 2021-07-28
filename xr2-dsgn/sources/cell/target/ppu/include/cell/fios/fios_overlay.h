/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 320.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_overlay.h

	Class definitions for the FIOS overlay class.
*/

#ifndef __CELL_FIOS_OVERLAY_H__
#define __CELL_FIOS_OVERLAY_H__

#include <cell/fios/fios_media.h>

namespace cell {
namespace fios {

/**
	\brief An overlay is a media filter class which can be used to redirect accesses from part of one filesystem to somewhere else.
	There are three kinds of redirects:<br>
	 1. Scheduler redirects, sending requests to another scheduler.<br>
	 2. Media redirects, sending requests directly to another media.<br>
	 3. Path redirects, sending requests to another path on the same media.<br>
	 <br>
	A scheduler redirect passes media requests through to another scheduler. It is friendlier to other
	I/O, but slower than a media redirect because it requires additional thread switching.	
	A media redirect passes media requests directly through to another media object. Media redirects are the
	recommended method for merging HOSTFS, Blu-ray, and HDD0 gamedata.<br>
	<br>
	A path redirect takes accesses to a given path on the media and redirects them to another path.
	This can be used to replace files, change art easily (such as offering several "skins" for an interface),
	or whatever else you can come up with.<br>
	<br>
	You can create multiple redirect layers with this class and they are serviced in order. You can
	also change the order later if desired. You can also put more than one overlay layer in your stack
	if you want slightly different behaviors out of each of them. (For a PS3 example, Blu-ray and
	HOSTFS might be underneath a HDD1 cache layer, but patches residing on HDD0 would be better off above it.)<br>
	<br>
	By default, redirect layers are serviced in reverse order of creation: the last redirect created has
	index 0, and is the first one checked. You can query the order with #getIndexedLayer() and you can
	change it with #moveLayer().
*/
class FIOS_LINKAGE overlay : public mediafilter
{
public:
	/** \brief Creates an empty overlay mediafilter.
		\param[in]  pNextMedia   Next media in the stack. */
	        overlay(media *pNextMedia)
				FIOS_ATTRIBUTE_GCC((__nonnull__));
	/** \brief Destructor. */
	virtual ~overlay();
	
	/** \brief Type used to identify individual redirect layers. */
	typedef int32_t id_t;

	/** \brief Index used for redirect layers in the list. */
	typedef int32_t index_t;
	
        /** \brief Overlay constants. */
	enum e_OVERLAYCONSTANTS {
		/** \brief The invalid id. */
		kINVALID_ID = (id_t)0xFFFFFFFFL,
		/** \brief The invalid index. */
		kINVALID_INDEX = (index_t)0xFFFFFFFFL,
		/** \brief The last possible index. */
		kLAST_INDEX = (index_t)0x7FFFFFFFL,
	};
	
	/** \brief Type used for layer flags. */
	typedef uint32_t flags_t;
	
        /** \brief Layer flags. */
	enum e_LAYERFLAGS {
		kDEFAULT              = (flags_t)0,       //!< Requests default behavior.
		kDISABLED             = (flags_t)(1<<0),  //!< Set to temporarily disable a layer.

		// These flags are all mutually exclusive of each other.
		kOPAQUE               = (flags_t)(1<<2),  //!< This path replaces the overlaid path. In no circumstances is anything under the original path ever visible. Mutually exclusive with #kDEFAULT, #kUNDERLAY, #kNEWER, and #kOLDER.
		kUNDERLAY             = (flags_t)(1<<3),  //!< Existing files are always passed through. The layer is only given a chance if access to the existing item fails. Mutually exclusive with #kDEFAULT, #kOPAQUE, #kNEWER, and #kOLDER.
		kNEWER                = (flags_t)(1<<4),  //!< Only overlay an existing file if the overlay's mod date is newer than the original's mod date. Mutually exclusive with #kDEFAULT, #kOPAQUE, #kUNDERLAY, and #kOLDER.
		kOLDER                = (flags_t)(1<<5),  //!< Only overlay an existing file if the overlay's mod date is older than the original's mod date. Mutually exclusive with #kDEFAULT, #kOPAQUE, #kUNDERLAY, and #kNEWER.
	};
	
	/** \brief Adds a redirect layer.

		The new layer is inserted at index 0, making it the first layer to be processed. Older
		layers will have their index bumped by 1.
		\param[in]  pOverlayPoint  Path inside the media where the overlay is created.
		\param[in]  pScheduler     If this is a scheduler overlay, scheduler to send requests to. Otherwise pass NULL.
		\param[in]  pMedia         If this is a media overlay, other media to send requests to. Otherwise pass NULL.
		\param[in]  pPath          Path on other scheduler, other media, or this media.
		\param[in]  layerFlags     Flags controlling the layer's behavior.
		\param[out] pLayerID       On successful completion, set to a layer ID which can be used to change, delete, or disable the layer. May be NULL.
		\syncreturn */
	err_t addLayer(const char *pOverlayPoint, scheduler *pScheduler, media *pMedia, const char *pPath, flags_t layerFlags, id_t *pLayerID)
		FIOS_ATTRIBUTE_GCC((__nonnull__(2,5)));
	
	/** \brief Deletes a redirect layer.
		\param[in]  layerID      Layer to delete.
		\syncreturn */
	err_t deleteLayer(id_t layerID);
	
	/** \brief Modifies a redirect layer.

		All properties of the layer can be modified with this call. It's very similar to simply deleting and re-adding
		the layer, except that the layer keeps its ID and position in the list.
		\param[in]  layerID        Identifier of the layer to modify.
		\param[in]  pOverlayPoint  Path inside this media where the overlay is created.
		\param[in]  pScheduler     If this is a scheduler redirect, scheduler to send requests to. Otherwise pass NULL.
		\param[in]  pMedia         If this is a media redirect, other media to send requests to. Otherwise pass NULL.
		\param[in]  pPath          Path on other scheduler, other media, or this media.
		\param[in]  layerFlags     Flags controlling the layer's behavior.
		\syncreturn */
	err_t modifyLayer(id_t layerID, const char *pOverlayPoint, scheduler *pScheduler, media *pMedia, const char *pPath, flags_t layerFlags)
		FIOS_ATTRIBUTE_GCC((__nonnull__(3,6)));
	
	/** \brief Enables or disables a redirect layer.
		\param[in]  layerID        Identifier of the layer to modify.
		\param[in]  enabled        True to enable, false to disable.
		\syncreturn */
	err_t setLayerEnabled(id_t layerID, bool enabled);
	
	/** \brief Gets information about a redirect layer.

		The paths returned are trimmed of trailing slashes. This means that the root
		path ('/') is returned as an empty string.
		\param[in]  layerID        Identifier of the layer to query.
		\param[out] pIndex         Receives the index of the layer in the list. May be NULL.
		\param[out] pOverlayPoint  Receives a pointer to the path inside this media where the overlay was created. May be NULL.
		\param[out] pScheduler     Receives a pointer to the scheduler, if this request is a scheduler redirect. May be NULL.
		\param[out] pMedia         Receives a pointer to the other media, if this request is a media redirect. May be NULL.
		\param[out] pPath          Receives a pointer to the overlaid path. May be NULL.
		\param[out] pLayerFlags    Receives the layer flags. May be NULL.
		\syncreturn */
	err_t getLayerInfo(id_t layerID, index_t *pIndex, const char **pOverlayPoint, scheduler **pScheduler, media **pMedia, const char **pPath, flags_t *pLayerFlags) const;
	
	/** \brief Gets the enabled status of a layer.
		\note This function is an inline callthrough to getLayerInfo, provided for convenience.
		\param[in]  layerID        Identifier of the layer to query.
		\param[out] pEnabled       Set to true if enabled, false if disabled.
		\syncreturn */
	inline err_t getLayerEnabled(id_t layerID, bool *pEnabled) const {
		flags_t flags = kDISABLED;
		err_t err = getLayerInfo(layerID, NULL, NULL, NULL, NULL, NULL, &flags);
		if (FIOS_LIKELY(pEnabled != NULL)) *pEnabled = !(flags & kDISABLED);
		return err;
	}

	/** \brief Returns the number of layers in the list.
		\return The number of layers handled by this layer. */
	index_t countLayers() const;
	
	/** \brief Gets the Nth layer from the list.
		\note When processing requests, layer 0 is given the first shot at it, then layer 1, etc.
		\param[in]  index    Zero-based index of the layer to retrieve.  This index is pinned to valid values, so #kLAST_INDEX can be used to get the index at the end of the list.
		\return Layer identifier, or kINVALID_ID if there are no layers. */
	id_t getIndexedLayer(index_t index) const;
	
	/** \brief Changes the index of a layer.

		Moving a layer to an index _before_ its current index shifts all layers
		in the range [newIndex,oldIndex) one position higher.  Moving a layer to an index 
		_after_ its current index shifts all layers in the range (oldIndex,newIndex]
		one position lower.
		
		To move a layer to the bottom of the stack, so that it is processed last, use
		moveLayer(id, kLAST_INDEX). To move a layer to the top of the stack, so that it is
		processed first, use moveLayer(id, 0).
		\param[in]  layerID        Identifier for the layer to move.
		\param[in]  newIndex       Where you want to put the layer. This index is pinned to valid values, so #kLAST_INDEX can be used to move an index to the end of the list.
		\syncreturn */
	err_t moveLayer(id_t layerID, index_t newIndex);

	/** \brief Frees memory allocated by readDirectory. */
	void freeDirectoryEntries() { freeDirectoryEntries(true); }

public: // Media layer implementations

#ifndef DOXYGEN_IGNORE
	/**
		\brief Synchronously executes a single I/O operation.

		This function is called by the scheduler's worker threads to execute a single I/O request. Concrete subclasses
		should satisfy the request synchronously, fill in PCE coefficients, and set the error code in
		the mediaioparams before returning.
		\param[in,out] pIOP           I/O parameters describing the operation.
	*/
	virtual void executeIOP(
		mediaioparams *pIOP)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	
	/*
		\brief Immediately returns the predicted PCE coefficients for an I/O request.

		This function is used by the scheduler to predict I/O completion times.
		It always returns the results immediately, without blocking, and should be
		as efficient as possible.
		\param[in,out] pIOP           I/O parameters describing the request.
	*/
	virtual void speculateIOP(
		mediaioparams *pIOP)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
#endif // DOXYGEN_IGNORE
	
#ifndef DOXYGEN_IGNORE
protected:
        /** \internal */
	/*@{*/
	typedef struct LayerDescriptor {
		id_t       id;                   //!< Identifier for this layer.
		flags_t    flags;                //!< Flags for this layer.
		scheduler *pScheduler;           //!< Scheduler, or NULL.
		media *    pMedia;               //!< Media, or NULL.
		uint16_t   srcLen;               //!< Length of the string in src.
		uint16_t   dstLen;               //!< Length of the string in dst.
		char       src[FIOS_PATH_MAX];   //!< The overlay point.
		char       dst[FIOS_PATH_MAX];   //!< The destination path (either locally or on the other scheduler) to which requests are redirected.
	} LayerDescriptor;
	
	mutable platform::rwlock   m_rwLock;                /**< Read-write lock for the overlay mediafilter. */
	collections::array<LayerDescriptor> m_layers;       /**< List of layers. */

	char m_dirEntryPath[FIOS_PATH_MAX];                 /**< Path that we've gathered directory information for. */
	collections::slicearray<direntry_t*,4,512> m_dirEntries;  /**< List of directory entries. */
	collections::chunkstorage m_dirEntryStorage;        /**< Storage for directory entries. */
	bool m_dirPassthru;                                 /**< Whether this directory should be a simple passthru. */
	
	static platform::atomicI32  s_lastID;               /**< Last ID allocated. */

	/** \brief Frees memory allocated by readDirectory.
		\param[in] needsLock  Whether we need to acquire a write lock. Set to false if the caller already has one. */
	void freeDirectoryEntries(bool needsLock);

	/** \brief Resolves a path through the overlays.
		\param[in,out] pIOP               Media I/O params.
		\param[in]     needsLock          Whether to acquire a lock.
		\param[in,out] pTargetPath        Path to resolve.
		\param[in,out] pTargetOffset      Offset to resolve.
		\param[in,out] pTargetLength      Length to resolve.
		\param[out]    pTargetMedia       The target media object.
		\param[out]    pTargetScheduler   The target scheduler.
		\return True if the path could be resolved, false otherwise. */
	bool resolvePath(
		mediaioparams *pIOP,
		bool needsLock,
		char *pTargetPath,
		fios::off_t *pTargetOffset,
		fios::off_t *pTargetLength,
		fios::media **pTargetMedia,
		fios::scheduler **pTargetScheduler);

	/** \brief Find the index of an layer, given its id.
		\param[in] layerID      Identifier for an laye.
		A read or write lock must be held.
		\return Index of the layer, or kINVALID_INDEX if not found. */
	index_t lookupLayer(id_t layerID) const;

	/** \brief Returns true if the flags are valid.
		\param[in] flags   Flags to check.
		\return True if the flags are valid, false otherwise. */
	static bool validFlags(flags_t flags);
	
	/** \brief Maps a client path through a layer.
		\param[in]  pPath    Incoming path.
		\param[in]  desc     Layer descriptor that is being considered.
		\param[out] pOutPath If the function returns TRUE, this is filled in with the path after being mapped through the layer.
		\return True if the path should be mapped through the layer, false otherwise. */
	static bool pathInsideOverlayPoint(const char *pPath, const LayerDescriptor &desc, char *pOutPath);

	/** \brief Stats a path, possibly on another scheduler.
		\param[in]  pIOP         I/O parameters that accumulate any performance coefficients.
		\param[in]  pScheduler   If non-NULL, scheduler to pass the request to. If NULL, use pMedia.
		\param[in]  pMedia       If non-NULL, media to pass the request to. If NULL, indicates that we're passing this request to the next media in the stack.
		\param[in]  pPath        Path to stat.
		\param[out] pOutStat     Filled in with stat results. May be NULL.
		\syncreturn */
	err_t overlayStat(mediaioparams *pIOP, scheduler *pScheduler, media *pMedia, const char *pPath, stat_t *pOutStat);

	/** \brief Passes a request through with a modified path, possibly via another scheduler.
		\param[in]  pIOP         I/O parameters describing the request.
		\param[in]  pScheduler   If non-NULL, scheduler to pass the request to. If NULL, use pMedia.
		\param[in]  pMedia       If non-NULL, media to pass the request to. If NULL, indicates that we're passing this request to the next media in the stack.
		\param[in]  pPath        Path to use instead of the request's path. */
	void overlayIOP(mediaioparams *pIOP, scheduler *pScheduler, media *pMedia, const char *pPath);

	/** \brief Passes an IOP through to a scheduler.
		\param[in,out]   pIOP        I/O parameters describing the request.
		\param[in]       pScheduler  Scheduler that executes the I/O.
		\param[in]       pFH         Filehandle to use. (may be NULL)
		\param[in]       pPath       Path associated with the request.
		\return True if the operation completed with no error. */
	bool passthruIOPToScheduler(mediaioparams *pIOP, scheduler *pScheduler, filehandle *pFH, const char *pPath);

	/** \brief Passes an IOP through to a media obejct.
		\param[in,out]   pIOP        I/O parameters describing the request.
		\param[in]       pMedia      Media that executes the I/O, NULL if a straight pass-thru.
		\param[in]       pFD         Media fd to use. (may be NULL)
		\param[in]       pPath       Path associated with the request.
		\return True if the operation completed with no error. */
	bool passthruIOPToMedia(mediaioparams *pIOP, media *pMedia, mediafd *pFD, const char *pPath);
	
	/** \brief Handles kMEDIAACTION_GETDIRENTRIES.
		\param[in,out]   pIOP        I/O parameters describing the request. */
	void readDirectoryWithOverlay(mediaioparams *pIOP);
	/*@}*/
#endif // DOXYGEN_IGNORE

public:
	FIOS_OBJECT_NEW_AND_DELETE(overlay,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
};
}; /* namespace fios */
}; /* namespace cell */

#endif /* __CELL_FIOS_OVERLAY_H__ */
