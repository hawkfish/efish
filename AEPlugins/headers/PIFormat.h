//-------------------------------------------------------------------------------
//
//	File:
//		PIFormat.h
//
//	Copyright 1991-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 1 of Photoshop's plug-in image format interface.
//
//	Use:
//		Include in all Format plug-ins.
//		
//
//	Version history:
//		Version 1.0.0	10/9/1997	Ace		Initial compilation.
//		
//-------------------------------------------------------------------------------

#ifndef __PIFormat__
#define __PIFormat__

#include "PIActions.h"
#include "PIGeneral.h"
#include "SPBasic.h"

/* Operation selectors */

#define formatSelectorAbout 			 0

#define formatSelectorReadPrepare		 1
#define formatSelectorReadStart 		 2
#define formatSelectorReadContinue		 3
#define formatSelectorReadFinish		 4

#define formatSelectorOptionsPrepare	 5
#define formatSelectorOptionsStart		 6
#define formatSelectorOptionsContinue	 7
#define formatSelectorOptionsFinish 	 8

#define formatSelectorEstimatePrepare	 9
#define formatSelectorEstimateStart 	10
#define formatSelectorEstimateContinue	11
#define formatSelectorEstimateFinish	12

#define formatSelectorWritePrepare		13
#define formatSelectorWriteStart		14
#define formatSelectorWriteContinue 	15
#define formatSelectorWriteFinish		16

#define formatSelectorFilterFile		17

/*	We keep various pieces of information about the file format in the PiMI resource.
	Here is the structure of that resource. */

/******************************************************************************/

#define PIFmtFileTypeProperty   0x666d5443 	/* 'fmTC' <TypeCreatorPair> Default file type for file format. */
#define PIReadTypesProperty		0x52645479 	/* 'RdTy' <TypeCreatorPair []> File type info for file format plugins */
#define PIFilteredTypesProperty	0x66667454 	/* 'fftT' <TypeCreatorPair []> File types to filter for file format plugins */
#define PIReadExtProperty		0x52644578 	/* 'RdEx' <OSType []> File type info for file format plugins */
#define PIFilteredExtProperty	0x66667445 	/* 'fftE' <OSType []> File types to filter for file format plugins */
#define PIFmtFlagsProperty		0x666d7466 	/* 'fmtf' <Set> Format flags */

#define PIFmtMaxSizeProperty	0x6d78737a 	/* 'mxsz' <Point> Max rows and cols supported by this format. 	*/
#define PIFmtMaxChannelsProperty 0x6d786368 /* 'mxch' <int16 []> Max channels per mode for this format. 	*/

/******************************************************************************/

/* Format flags indices. */

#define PIFmtReadsAllTypesFlag 0
#define PIFmtSavesImageResourcesFlag 1
#define PIFmtCanReadFlag 2
#define PIFmtCanWriteFlag 3
#define PIFmtCanWriteIfReadFlag 4

#define PIFmtCanWriteTransparency 5 /* for saving transparency index of indexed color image */
									/* or saving transparency as a first alpha channel for  */
									/* other color modes. (see also the comments of 		*/
									/* transparencyMatting and transparencyPlane in	the		*/
									/* FormatRecord)										*/
#define PIFmtCanCreateThumbnail 6 	/* set the flag only when the plug-in does not need to 	*/
									/* query for open parameters.			 				*/

/******************************************************************************/

/* Pragma to byte align structures; for Borland C */

#if defined(__BORLANDC__)
#pragma option -a-
#endif

/* Pragma to byte align structures; for Microsoft C */
#if WIN32
#pragma pack(push,1)
#endif

/******************************************************************************/

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct FormatInfo
	{

	PlugInInfo	plugInInfo; 			/* The information common to all plug-ins.			*/
	int16		typeCount;				/* The number of types in the type list.			*/
	int16		extensionCount; 		/* The number of extensions in the type list.		*/
	Boolean 	canRead;				/* Can we read using this format?					*/
	Boolean 	readsAllTypes;			/* Can this plug-in read from all files?			*/
	Boolean 	canWrite;				/* Can we write using this format?					*/
	Boolean		canWriteIfRead;			/* Can we write if we read using this format? 		*/
	Boolean 	savesImageResources;	/* Does this file format save the resource data.	*/
	Boolean		padFlag;				/* Padding */
	int16		maxChannels [16];		/* Maximum # of channels with each plug-in mode.	*/
	int16		maxRows;				/* Maximum rows allowed in document.				*/
	int16		maxCols;				/* Maximum columns allowed in document. 			*/
	OSType		fileType;				/* The file type if we create a file.				*/
	OSType		creatorType;			/* The creator type if we create a file.			*/
	OSType		typeList [1];			/* The types and extensions supported.				*/

	}
FormatInfo;

/******************************************************************************/
/* turn off the byte align pragma back to its original state; only for Borland C */
#if defined(__BORLANDC__)
#pragma option -a.
#endif

/* Pragma to turn off align structures for Microsoft C */
#if WIN32
#pragma pack(pop)
#endif
/******************************************************************************/

/*	Error return values. The plug-in module may also return standard Macintosh
	operating system error codes, or report its own errors, in which case it
	can return any positive integer. */

#define formatBadParameters -30500
#define formatCannotRead	-30501

#if WIN32
#pragma pack(push,4)
#endif

typedef struct FormatRecord
	{

	int32				serialNumber;	/* Host's serial number, to allow
										   copy protected plug-in modules. */

	TestAbortProc		abortProc;		/* The plug-in module may call this no-argument
										   BOOLEAN function (using Pascal calling
										   conventions) several times a second during long
										   operations to allow the user to abort the operation.
										   If it returns TRUE, the operation should be aborted
										   (and a positive error code returned). */

	ProgressProc		progressProc;	/* The plug-in module may call this two-argument
										   procedure periodically to update a progress
										   indicator.  The first parameter is the number
										   of operations completed; the second is the total
										   number of operations. */

	int32				maxData;		/* Maximum number of bytes that should be
										   passed back at once, plus the size of any
										   interal buffers. The plug-in may reduce this
										   value in the formatSelectorPrepare routine. */

	int32				minDataBytes;	/* Disk space needed for the data fork. */
	int32				maxDataBytes;	/* Disk space needed for the data fork. */

	int32				minRsrcBytes;	/* Disk space needed for the resource fork. */
	int32				maxRsrcBytes;	/* Disk space needed for the resource fork. */

	int32				dataFork;		/* refnum for the data fork. */
	int32				rsrcFork;		/* refnum for the resource fork. */
	
	FSSpec *			fileSpec;		/* Full file specification. */

	int16				imageMode;		/* Image mode */
	Point				imageSize;		/* Size of image */
	int16				depth;			/* Bits per sample, currently must be 1 or 8 */
	int16				planes; 		/* Samples per pixel */

	Fixed				imageHRes;		/* Pixels per inch */
	Fixed				imageVRes;		/* Pixels per inch */

	LookUpTable			redLUT; 		/* Red LUT, only used for Indexed Color images */
	LookUpTable			greenLUT;		/* Green LUT, only used for Indexed Color images */
	LookUpTable			blueLUT;		/* Blue LUT, only used for Indexed Color images */

	void *				data;			/* A pointer to the returned image data. The
										   plug-in module is responsible for freeing
										   this buffer. Should be set to NIL when
										   all the image data has been returned. */

	Rect				theRect;		/* Rectangle being returned */
	int16				loPlane;		/* First plane being returned */
	int16				hiPlane;		/* Last plane being returned */
	int16				colBytes;		/* Spacing between columns */
	int32				rowBytes;		/* Spacing between rows */
	int32				planeBytes; 	/* Spacing between planes (ignored if only one
										   plane is returned at a time) */
	PlaneMap			planeMap;		/* Maps plug-in plane numbers to host plane
										   numbers.  The host initializes this is a linear
										   mapping.  The plug-in may change this mapping if
										   it sees the data in a different order. */

	Boolean 			canTranspose;	/* Is the host able to transpose the image? */
	Boolean 			needTranspose;	/* Does the plug-in need the image transposed? */

	OSType				hostSig;		/* Creator code for host application */
	HostProc			hostProc;		/* Host specific callback procedure */

	int16				hostModes;		/* Used by the host to inform the plug-in which
										   imageMode values it supports.  If the corresponding
										   bit (LSB = bit 0) is 1, the mode is supported. */

	Handle				revertInfo; 	/* Information to be kept with the document for reverting or
										   saving.	Where possible this handle should be used to
										   avoid bringing up an options dialog. */

	NewPIHandleProc		hostNewHdl; 	/* Handle allocation and disposal for revert info. */
	DisposePIHandleProc	hostDisposeHdl;
	
	Handle				imageRsrcData;	/* Handle containing the block of resource data. */
	int32				imageRsrcSize;	/* size of image resources. */

	PlugInMonitor		monitor;		/* The host's monitor. */

	void *				platformData;	/* Platform specific information. */

	BufferProcs *		bufferProcs;	/* The procedures for allocating and */
										/* releasing buffers. */
										
	ResourceProcs *		resourceProcs;	/* Plug-in resource procedures. */

	ProcessEventProc	processEvent;	/* Pass event to the application. */
	
	DisplayPixelsProc	displayPixels;	/* Display dithered pixels. */

	HandleProcs			*handleProcs;	/* Platform independent handle manipulation. */
	
	/* New in 3.0. */
	
	OSType				fileType;		/* File-type for filtering */
	
	ColorServicesProc	colorServices; /* Routine to access color services. */
	
	AdvanceStateProc	advanceState;	/* Valid from continue selectors. */
	
	/* New in 3.0.4. */
	
	PropertyProcs		*propertyProcs; /* A suite to allow getting and setting image
										   document properties. */
	
	ImageServicesProcs	*imageServicesProcs;
										/* The suite of image processing services
										   callbacks. */
	
	int16				tileWidth;		/* The width of the tiles. Zero if not set. */
	int16				tileHeight;		/* The height of the tiles. Zero if not set. */
	
	Point				tileOrigin;		/* The origin point for the tiles. */
	
	/* New in 4.0 */
	
	PIDescriptorParameters	*descriptorParameters;	/* For recording and playback */
	Str255					*errorString;			/* For silent and errReportString */
	
	/* New in 5.0 */
	
	int32				maxValue;		/* used for read with 16bit depth only */
	SPBasicSuite		*sSPBasic;		/* SuitePea basic suite */
	void				*plugInRef;		/* plugin reference used by SuitePea*/
	
	int32				transparentIndex;	/* if IndexedColor, and between 0 and 255, this is the index of the transparent color (for GIF) */
	
	Handle				iCCprofileData;		/* Handle containing the ICC profile for the image. (NULL if none)
												For reads:
													The handle must be allocated using Photoshop's handle suite.
													Photoshop will use the data after the Finish call
													and Photoshop will free the handle
												For writes:
													Photoshop allocates the handle using Photoshop's handle suite
													The handle is unlocked while calling the plug-in
													The handle will be valid from Start to Finish
													Photoshop will free the handle after Finish
													
											*/
	int32				iCCprofileSize;		/* size of profile. */
	
	int32				canUseICCProfiles;	/* non-zero if the host can accept/export ICC profiles 
												if this is zero, you'd better not set or dereference iCCprofileData.
											*/

	/* New in 5.5 */
	
	int32			lutCount;			/* Number of entries in the indexed color table.
										   This will include the transparent index if any. */
										   
	/* New in 6.0 */
	
	/* The following fields are only relevant when reading. */
	
	int32			preferredColorModes;/* A bitmask indicating which color modes are preferred by the host
										   This will be zero if not set. The plug-in is free to ignore the
										   hint (though it is wise to obey the hostModes information).
										   Note that this is an int32 rather than an int16 to allow for
										   future expansion. */
										   
	int32			convertMode;		/* 0 if the host does not support conversions. -1 if the host does
										   support conversions after reading. The plug-in can set this field
										   to the index of a color mode and this will be treated by the host
										   as a request to automatically convert to that mode. Hosts can
										   ignore this request but they will make the host+plug-in combination
										   appear broken. */

	VPoint			preferredSize;		/* 0,0 if not set. This is a hint about how big an image the
										   wants. Note that this field uses 32-bit coordinates rather than
										   16-bit coordinates. At some point the rest of the API will
										   probably be extended to handle big coordinates and it seemed
										   reasonable to make the hint big enough to be ready for that
										   so that we don't need to adjust it as well. */

	int32			imageIndex;			/* 0 if not set. One means first image, etc. Only used for PDF Image
										   extractor currently. */
										   
	int32			transparencyPlane;	/* When writing files:
											Set to zero if host does not support transparency or there is
											no transparency. Set to the index of the plane containing
											transparency information if the host does support transparency.
											Any alpha channels after this index are pushed down by one.
											Note that the interpretation of zero is chosen for compatibility
											with old hosts, but it also means that the transparency plane
											will never be the first plane.
											
										  When reading files:
										  	This is set to zero by hosts that do not support transparency.
										  	This is set to -1 by hosts that support transparency. The
										  	plug-in should set it to the index of the plane containing
										  	transparency information if there is one. */
										  	
	int32			transparencyMatting;/* 0: no matte, 1: black matte, 2: gray matte, 3: white matte	
										   When reading files, how should the transparency data be interpreted
											with respect to color matting. 
											
										   When writing, this is set by the plug-in to indicate whether 
										   the host should be premultiplied with a choosen color. The default 
										   value is no matting (unassociated). */

	 /* Reserved for future expansion */

	char			reserved [156]; /* Set to zero */

	}
FormatRecord, *FormatRecordPtr;

#if WIN32
#pragma pack(pop)
#endif

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

#endif  // __PIFormat_h__
