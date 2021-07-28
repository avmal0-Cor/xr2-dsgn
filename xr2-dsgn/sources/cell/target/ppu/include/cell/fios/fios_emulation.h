/*	
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2009 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\file fios_emulation.h

	FIOS blu-ray and HDD performance emulation.
*/

#ifndef __CELL_FIOS_EMULATION_H__
#define __CELL_FIOS_EMULATION_H__

#include <cell/fios/fios_media.h>

namespace cell {
namespace fios {

/**
	\brief Emulate blu-ray or retail hdd media performance.
	Use this as a filter above the media layer, generally on top of HDD0 or
	HOSTFS.
	<br>
*/

enum emulation_device {
		kEmulateBlurayDevice,
		kEmulateHDDDevice,
		kEmulateDeviceMax,
};

/** \brief This struct is used to pass parameters to the emulation class on
 * creation. Create your own values, or use the predefined values of
 * g_emulateBluray or g_emulateRetailHDD.
 **/
struct emulation_params {
	uint32_t m_blockSize;				//!< block size in bytes
	uint32_t m_readBandwidth;			//!< read bandwidth in bytes/sec
	uint32_t m_writeBandwidth;			//!< write bandwidth in bytes/sec
	uint32_t m_seekTime;					//!< seek time in microseconds
	uint32_t m_latency;					//!< latency per IO request in microseconds
	emulation_device m_device;		//!< the underlying device. Multiple instances of the fios::emulation class will use a single mutex for each underlying device.
	uint32_t m_partition;				//!< partition nummber, maps to HDD0 and HDD1.
};

extern FIOS_LINKAGE const emulation_params g_emulateBluray;		//!< Emulate Bluray drive
extern FIOS_LINKAGE const emulation_params g_emulateRetailHDD0;	//!< Emulate retail unit HDD0
extern FIOS_LINKAGE const emulation_params g_emulateRetailHDD1;	//!< Emulate retail unit HDD1

class FIOS_LINKAGE emulation : public mediafilter
{
public:
	/** \brief Create a emulation mediafilter.
		\param[in] pNextMedia   Next media in the stack. 
	 	\param[in] pMedia    	Pointer to data specifying the parameters of the media to be emulated */
	emulation(media *pNextMedia, const emulation_params *pMedia)
		FIOS_ATTRIBUTE_GCC((__nonnull__));
	/** \brief Destructor. */
	virtual ~emulation();
	/**
		\brief Synchronously executes a single I/O operation.

		This function is called by the scheduler's worker threads to execute a
		single I/O request. Concrete subclasses should satisfy the request
		synchronously, fill in PCE coefficients, and set the error code in the
		mediaioparams before returning.
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

#ifndef DOXYGEN_IGNORE
private:

	struct deviceMutexes {
		platform::atomicU32 m_allocCount;
		platform::mutex *m_deviceMutex;
		uint32_t lastPartition;
	};
	static struct deviceMutexes m_devices[kEmulateDeviceMax];  //!< serialize IO access for each device

	/** \internal */
	/*@{*/
	emulation_device m_device;		//!< the underlying device. Multiple instances of the fios::emulation class will use a single mutex for each underlying device.
	uint32_t m_partition;				//!< partition number (for example HDD0 or HDD1).
	uint32_t m_blockSize;					//!< emulated media's block size.
	abstime_t m_latency;				//!< latency per IO request in abs time
	uint32_t m_readBandwidth;				//!< read bandwidth in bytes/sec
	uint32_t m_writeBandwidth;				//!< write bandwidth in bytes/sec
	abstime_t m_seekTime;				//!< roughly worst case seek abs time
	pathreference_t m_prevPath;	//!< path name of the previous IO request
	off_t m_prevOffset;			//!< offset of previous IO request
	/*}*/
#endif

public:
	FIOS_OBJECT_NEW_AND_DELETE(overlay,kMemPurposeFIOSMediaObject|kPoolIDLongTerm)
};

}; /* namespace fios */
}; /* namespace cell */

#endif // __CELL_FIOS_EMULATION_H__
