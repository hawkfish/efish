//-------------------------------------------------------------------------------
//
//	File:
//		PIGeneral.h
//
//	Copyright 1991-1998, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file contains the public definitions and structures
//		used by all plug-ins.
//
//	Use:
//		This file must be included to correctly compile resources
//		on Windows; it is included by all the plug-in headers.
//
//	Version history:
//		Version 1.0.0	10/9/1997	Ace		Initial compilation.
//			Updated for new headers for Photoshop 5.0.  Broke out
//			ResDefines into separate file for PIGeneral.h and PIGeneral.r.
//
//-------------------------------------------------------------------------------

#ifndef __PIGeneral_h__ // Already defined?
#define __PIGeneral_h__

#pragma once

//-------------------------------------------------------------------------------
//	Includes and alignment.
//-------------------------------------------------------------------------------
#include "PITypes.h"

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#if defined(__BORLANDC__)
#pragma option -a-
#endif

#include "PIResDefines.h"

#if MSWindows
	#define PIVersion "6.0"
	#define ProductVersion  6,0,0x06,0
	// Common macros and defines for Macintosh and Windows resource templates
	// have now been moved into PIResDefines.h.
#endif // MSWindows

//-------------------------------------------------------------------------------
//	Defines -- Photoshop signature -- '8BIM'
//-------------------------------------------------------------------------------
#define kPhotoshopSignature		0x3842494dL

//-------------------------------------------------------------------------------
//	Defines -- General plug-in error codes.
//-------------------------------------------------------------------------------

// "The plug-in requires services not provided by this host.":
#define errPlugInHostInsufficient			-30900

// "A requested property could not be found.":
#define errPlugInPropertyUndefined			-30901

// If an image processing operation does not support colBytes values
// other than 1. "The plug-in requires services not provided by this host.":
#define errHostDoesNotSupportColStep		-30902

// plugIncolorServicesSamplePoint return value indicating point
// is not in image window:
#define errInvalidSamplePoint				-30903
		 
// Notify the host to use the error string for reporting:
#define errReportString						-30904

//-------------------------------------------------------------------------------
//	Defines -- Channel Port error codes.
//-------------------------------------------------------------------------------
#define errUnknownPort						-30910

#define errUnsupportedRowBits				-30911
#define errUnsupportedColBits				-30912
#define errUnsupportedBitOffset				-30913

// We've never heard of this depth:
#define errUnsupportedDepth					-30914

// We've heard of this depth, but cannot do the conversion:
#define errUnsupportedDepthConversion 		-30915

//-------------------------------------------------------------------------------
//	Defines -- Scripting system error codes.
//-------------------------------------------------------------------------------
#define errWrongPlatformFilePath	 		-30916

#define errIllegalCharInID					-30917	// non-alphanum in key, class, etc.

// errAEParamMissed:
#define errMissingParameter					-1715

//-------------------------------------------------------------------------------
//
//	General PiPL Properties
//
//  These properties are also defined by Suite Pea, so we must bracket their
//	definition to make sure they are only defined once.
//
//-------------------------------------------------------------------------------

// Only define general PiPL properties once:
#ifndef kGeneralPiPLPropertiesDefined
#define kGeneralPiPLPropertiesDefined

// This original came from OSType on the Mac:
typedef unsigned long PIType; 

//-------------------------------------------------------------------------------
//	Defines -- Current PiPL version and code descriptor properties.
//-------------------------------------------------------------------------------
#define kCurrentPiPLVersion 	0

// 'kind' <PIType> giving the plug-in's kind:
#define PIKindProperty			0x6b696e64L

// 'm68k' <PI68KCodeDesc> 68k code descriptor. See struct below.
#define PI68KCodeProperty		0x6d36386bL

// 'pwpc' <PICFMCodeDesc> PowerPC code descriptor. See struct below.
#define PIPowerPCCodeProperty	0x70777063L

// 'wx86' <PIWin32X86CodeDesc> Win32 Intel code descriptor. See struct below.
#define PIWin32X86CodeProperty  0x77783836L

// 'fx86' <PIWin16X86CodeDesc> Win16 Intel code descriptor. See struct below.
#define PIWin16X86CodeProperty  0x66783836L

//-------------------------------------------------------------------------------
//	Structures -- PiPL entry definition.
//-------------------------------------------------------------------------------
typedef struct PIProperty
{
	// Vendor specific identifier:	
	PIType vendorID;
	
	// Identifcation key for this property type:
	PIType propertyKey;
	
	// Index within this property type. Must be unique for properties of
	// a given type in a PiPL:
	int32  propertyID;
	
	// Length of following data array. Will be rounded to multiple of 4:
	int32  propertyLength;
	
	// propertyLength bytes long. Must be multiple of 4:
	char   propertyData [1];

} PIProperty;

typedef struct PIPropertyList
{
	// Properties data structure version number:
	int32 version;

	// Number of properties contained:
	int32 count;

	// The actual properties:
	PIProperty properties[1];

} PIPropertyList;

//-------------------------------------------------------------------------------
//
//	Structures -- PiPL types.
//
//	Below are structures describing types used in plug-in property lists.
//	These structures obey Macintosh 68k alignment and padding rules though
//	generally they are laid out so fields have natural alignment and any
//	needed padding is explicit in the structure.
//
//	In addition to these structure types, simple types such as int32, int16,
//	etc. are used. Two other examples are PString and FlagSet.
//
//	PString is like a rez pstring or a Str255 except that storage may not
//  be present for all 255 characters.  The initial length byte is present
//  in the property.
//
//	FlagSet is a variable length bitstring in which the first member is
//	represented by the most significant bit of the first byte, the eighth
//	member is in the least significant bit of the first byte, etc.
//  The length of the set is in the property's length field.
//
//	Arrays are also used in places with bounds either taken from the
//	property length or explicitly represented within the property.
//
//-------------------------------------------------------------------------------

// For 'm68k' PI68kCodeProperty:
typedef struct PI68KCodeDesc
{
	// 4-character OSType where code for 68k executable is. Usually
	// the plug-in type.  For instance, a Filter's 68k code is
	// usually compiled into its '8BFM' resource:
	PIType fType;

	// Code can be found in resource type fType, resource number fID:
	int16 fID;

} PI68KCodeDesc;

// For 'pwpc' PIPowerPCCodeProperty:
typedef struct PICFMCodeDesc
{
	// Offset from beginning of container:
	long fContainerOffset;
	
	// Length of data:
	long fContainerLength;
	
	// Name of entrypoint ("main", "ENTRYPOINT", "", etc.):
	unsigned char fEntryName[1];

} PICFMCodeDesc;
	
// For 'wx86' PIWin32X86CodeProperty:
typedef struct PIWin32X86CodeDesc
{
	// Name of entrypoint ("ENTRYPOINT", etc.):
	char fEntryName[1];

} PIWin32X86CodeDesc;

// For 'fx86' PIWin16X86CodeProperty:
typedef struct PIWin16X86CodeDesc
{
	// Name of entrypoint ("ENTRYPOINT", etc.):
	char fEntryName[1];

} PIWin16X86CodeDesc;

#endif // kGeneralPiPLPropertiesDefined

//-------------------------------------------------------------------------------
//
//	Additional General PiPL Properties
//
//	These are properties that don't conflict with Suite Pea and are
//	defined for Photoshop specifically.
//
//-------------------------------------------------------------------------------

// 'vers' <int32> Major<int16>.Minor<int16> version number:
#define PIVersionProperty		0x76657273L

// 'prty' <int32> Load order priority:
#define PIPriorityProperty		0x70727479L

// 'cmpt' Component/Version ids:
#define PIComponentProperty		0x636d7074L

// 'mode' <FlagSet> Image modes supported flags. (bitmask):
#define PIImageModesProperty	0x6d6f6465L

// 'enbl' <CString> Enabling expression:
#define PIEnableInfoProperty	0x656e626cL

// 'host' <PIType> giving host required if any:
#define PIRequiredHostProperty	0x686f7374L

// 'catg' <PString> Category name that appears on top level menu:
#define PICategoryProperty		0x63617467L

// 'zcat' <PString> Category name that appears on top level menu:
#define PIZCategoryProperty		0x7A636174L

// 'name' <PString> Menu name:
#define PINameProperty			0x6e616d65L

// 'znam' <PString> Menu name:
#define PIZNameProperty			0x7A6E616DL

// 'prog' <PString> Progress text:
#define PIProgressTextProperty	0x70726f67

// 'zpro' <PString> Progress text:
#define PIZProgressTextProperty	0x7A70726F

// 'pnme' <CString> picker ID:
#define PIPickerIDProperty		0x706e6d65L

// '68fp' <PI68KCodeDesc> 68k w/FPU code descriptor. See struct below.
#define PI68KFPUCodeProperty	0x36386670L

// 'usp7' SPARC version 7:
#define PIUnixSPARCV7CodeProperty	0x75737037

// 'usp8' SPARC version 8:
#define PIUnixSPARCV8CodeProperty	0x75737038

// 'usp9' SPARC version 9:
#define PIUnixSPARCV9CodeProperty	0x75737039
#define PIUnixSPARCCodeProperty	PIUnixSPARCV7CodeProperty

// 'umr3' MIPS R3000:
#define PIUnixMIPSR3000CodeProperty	0x756d7233

// 'umr4' MIPS R4000:
#define PIUnixMIPSR4000CodeProperty	0x756d7234

// 'um4w' MIPS R4000 (can be 64 bit):
#define PIUnixMIPSR400064BitCodeProperty	0x756d3477

// 'umr8' MIPS R8000 (always 64 bit):
#define PIUnixMIPSR8000CodeProperty	0x756d7238
#define PIUnixMIPSCodeProperty	PIUnixMIPSR3000CodeProperty

//-------------------------------------------------------------------------------
//
//	Additional Structures -- PiPL types.
//
//	Below are structures describing types used in plug-in property lists.
//	These structures obey Macintosh 68k alignment and padding rules though
//	generally they are laid out so fields have natural alignment and any
//	needed padding is explicit in the structure.
//
//	In addition to these structure types, simple types such as int32, int16,
//	etc. are used. Two other examples are PString and FlagSet.
//
//	PString is like a rez pstring or a Str255 except that storage may not
//  be present for all 255 characters.  The initial length byte is present
//  in the property.
//
//	FlagSet is a variable length bitstring in which the first member is
//	represented by the most significant bit of the first byte, the eighth
//	member is in the least significant bit of the first byte, etc.
//  The length of the set is in the property's length field.
//
//	Arrays are also used in places with bounds either taken from the
//	property length or explicitly represented within the property.
//
//-------------------------------------------------------------------------------

// Even though Suite Pea may define PIPropertyList, it does
// not define the handle form of it:
typedef PIPropertyList **PIPropertyListHandle;

// Macintosh structure for file formats comprised of { 'type', 'crea' } where
// the first OSType is the 4-character file type and the second is the
// 4-character creator type:
typedef struct TypeCreatorPair
{
	PIType type;
	PIType creator;

} TypeCreatorPair;

typedef struct PlugInInfo
{

	// Version number of the interface supported:
	int16	version;

	 // Sub-version number:
	int16	subVersion;

	// Plug-in's priority:
	int16	priority;

	// Size of the general info:
	int16	generalInfoSize;

	// Size of the type specific info:
	int16	typeInfoSize;

	// Bit mask indicating supported image modes:
	int16	supportsMode;

	// Required host, if any:
	PIType	requireHost;

} PlugInInfo;

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

typedef struct PSBuffer *BufferID;

#define kPIGeneralInfoSize 16

//-------------------------------------------------------------------------------
//	Defines -- Image modes.
//-------------------------------------------------------------------------------
#define plugInModeBitmap	   0
#define plugInModeGrayScale    1
#define plugInModeIndexedColor 2
#define plugInModeRGBColor	   3
#define plugInModeCMYKColor    4
#define plugInModeHSLColor	   5
#define plugInModeHSBColor	   6
#define plugInModeMultichannel 7
#define plugInModeDuotone	   8
#define plugInModeLabColor	   9
#define plugInModeGray16	  10
#define plugInModeRGB48		  11
#define plugInModeLab48		  12
#define plugInModeCMYK64	  13
#define plugInModeDeepMultichannel 14
#define plugInModeDuotone16   15


//-------------------------------------------------------------------------------
//	Defines -- Blend modes.
//-------------------------------------------------------------------------------

/* Key for host supplied layer blend modes */
#define	kPIHostBlendModeSignature	'8BIM'

/* Host supplied blend modes keys */
#define	PIBlendNormal		'norm'
#define	PIBlendDarken		'dark'
#define	PIBlendLighten		'lite'
#define	PIBlendHue			'hue '
#define	PIBlendSaturation	'sat '
#define	PIBlendColor		'colr'
#define	PIBlendLuminosity	'lum '
#define	PIBlendMultiply		'mul '
#define	PIBlendScreen		'scrn'
#define	PIBlendDissolve		'diss'
#define	PIBlendOverlay		'over'
#define	PIBlendHardLight	'hLit'
#define	PIBlendSoftLight	'sLit'
#define	PIBlendDifference	'diff'
#define	PIBlendExclusion	'smud'
#define	PIBlendColorDodge	'div '
#define	PIBlendColorBurn	'idiv'

//-------------------------------------------------------------------------------
//	Structures -- PiPL types.
//-------------------------------------------------------------------------------

typedef struct PIBlendMode
	{
	PIType signature;  /* kPIHostBlendModeSignature for built-in blend modes */
	PIType key;
	} PIBlendMode;
	
//-------------------------------------------------------------------------------
//
//	Defines -- Moveable Modal Dialog Window.
//
//	This is the Window procID for a Moveable Modal dialog window that can be
//	used for Macintosh OS System 6.  WDEF(128) is part of Photoshop.
//
//-------------------------------------------------------------------------------
#define PSmovableDBoxProc	(128*16+5)

//-------------------------------------------------------------------------------
//	Structures -- General.
//-------------------------------------------------------------------------------

// For transposing one plane location to another on pre- or
// post-processing:
typedef int16 PlaneMap [16];

// For monitor setup information:
typedef struct PlugInMonitor
{
	Fixed gamma;
	Fixed redX;
	Fixed redY;
	Fixed greenX;
	Fixed greenY;
	Fixed blueX;
	Fixed blueY;
	Fixed whiteX;
	Fixed whiteY;
	Fixed ambient;

} PlugInMonitor;

//-------------------------------------------------------------------------------
//
//	Structures -- PSPixelMaps.
//
//	The following structures reflect
//		Version 1 10/14/1997
//		Version 2 03/22/1999
//	of the PSPixelMaps structure.
//
//-------------------------------------------------------------------------------
typedef struct PSPixelMask
{
	struct PSPixelMask *next;
	void *maskData;
	int32 rowBytes;
	int32 colBytes;
	int32 maskDescription;

} PSPixelMask;

// Defines for maskDescription field:
#define kSimplePSMask   0
#define kBlackMatPSMask 1
#define kGrayMatPSMask  2
#define kWhiteMatPSMask 3
#define kInvertPSMask   4

typedef struct PSPixelOverlay
	{
	struct PSPixelOverlay *next;
	void *data;
	int32 rowBytes;
	int32 colBytes;
	unsigned8 r, g, b, opacity;
	int32 overlayAlgorithm;	// 0 for alpha overlay. 1 for inverted alpha overlay.
	} PSPixelOverlay;

// Overlay algorithms.
#define kStandardAlphaOverlay	0
#define kInvertedAlphaOverlay	1

// Colormanagement options:
#define kViewAsMask				0xF	// Low 4 bits though we currently only use two of them
#define kViewAsStandardRGB		0	// Just use Photoshop's default handling
#define kViewAsMacRGB			1	// View as if using AppleRGB
#define kViewAsWindowsRGB		2	// Views as if using sRGB
#define kViewAsUncompensated	3	// Perform no compensation

typedef struct PSPixelMap
{
	int32 version;
	VRect bounds;
	int32 imageMode;
	int32 rowBytes;
	int32 colBytes;
	int32 planeBytes;
	void *baseAddr;
	
	//---------------------------------------------------------------------------
	// Fields new in version 1:
	//---------------------------------------------------------------------------	
	PSPixelMask *mat;
	PSPixelMask *masks;
	
	// Use to set the phase of the checkerboard:
	int32 maskPhaseRow;
	int32 maskPhaseCol;

	//---------------------------------------------------------------------------
	// Fields new in version 2:
	//---------------------------------------------------------------------------	
	PSPixelOverlay *pixelOverlays;
	unsigned32 colorManagementOptions;

} PSPixelMap;

//-------------------------------------------------------------------------------
//	Structures -- ColorServicesInfo.
//-------------------------------------------------------------------------------
typedef struct ColorServicesInfo
{
	int32 infoSize;
	int16 selector;
	int16 sourceSpace;
	int16 resultSpace;
	Boolean resultGamutInfoValid;
	Boolean resultInGamut;
	
	// Both voids must be NULL or will return paramErr:
	void *reservedSourceSpaceInfo;
	void *reservedResultSpaceInfo;

	int16 colorComponents[4];

	// Reserved must be NULL or will return paramErr:
	void *reserved;

	union
	{
		Str255 *pickerPrompt;
		Point *globalSamplePoint;
		int32 specialColorID;
	} selectorParameter;

} ColorServicesInfo;

// Color Service Selectors:
#define plugIncolorServicesChooseColor		0
#define plugIncolorServicesConvertColor		1		
#define plugIncolorServicesSamplePoint		2
#define plugIncolorServicesGetSpecialColor	3

// Constants for resultSpace and sourceSpace fields:
#define plugIncolorServicesRGBSpace		0
#define plugIncolorServicesHSBSpace		1
#define plugIncolorServicesCMYKSpace	2
#define plugIncolorServicesLabSpace		3
#define plugIncolorServicesGraySpace	4
#define plugIncolorServicesHSLSpace		5
#define plugIncolorServicesXYZSpace		6

// Only valid in the resultSpace field.  Will leave the color in the space the user
// chose, returning resultSpace as the user chosen space:
#define plugIncolorServicesChosenSpace	-1

// Used with the plugIncolorServicesGetSpecialColor selector:
#define plugIncolorServicesForegroundColor 0
#define plugIncolorServicesBackgroundColor 1

//-------------------------------------------------------------------------------
//	Structures -- Resampling callbacks.
//-------------------------------------------------------------------------------

typedef struct PSImagePlane
{
	void *data;
	Rect bounds;
	int32 rowBytes;
	int32 colBytes;

} PSImagePlane;

typedef struct PSImageMultiPlane
{
	void *data;
	Rect bounds;
	int32 rowBytes;
	int32 colBytes;
	int16 activeChannels;

} PSImageMultiPlane;

//-------------------------------------------------------------------------------
//	Structures -- Channel Ports.
//-------------------------------------------------------------------------------

// Describes an area of memory as pixels.  The actual rectangle is
// expected to be given separately:
typedef struct PixelMemoryDesc
{
	void *data;
	int32 rowBits;		// Note bits not bytes!
	int32 colBits;		// Note bits not bytes!
	int32 bitOffset;
	int32 depth;

} PixelMemoryDesc;

// We describe a scaling by providing two rectangles:
typedef struct PSScaling
{
	VRect sourceRect;
	VRect destinationRect;

} PSScaling;

// We provide access to internal channels via the following abstract types:

typedef struct _PIChannelPortDesc *PIChannelPort;

// Old declarations. (4.0)

typedef PIChannelPort ChannelReadPort;
typedef PIChannelPort ChannelWritePort;

//-------------------------------------------------------------------------------
//	Structures -- Channel Ports -- Channel Types
//-------------------------------------------------------------------------------
#define ctUnspecified		 0
#define ctRed				 1
#define ctGreen				 2
#define ctBlue				 3
#define ctCyan				 4
#define ctMagenta			 5
#define ctYellow			 6
#define ctBlack				 7
#define ctL					 8
#define ctA					 9
#define ctB					10
#define ctDuotone			11
#define ctIndex				12
#define ctBitmap			13
#define ctColorSelected		14
#define ctColorProtected	15
#define ctTransparency      16
#define ctLayerMask         17
#define ctInvertedLayerMask 18
#define ctSelectionMask     19
#define ctSpotColor			20

//-------------------------------------------------------------------------------
//	Structures -- Channel Ports -- WriteChannelDesc
//-------------------------------------------------------------------------------
#define kCurrentMinVersWriteChannelDesc	0
#define kCurrentMaxVersWriteChannelDesc	0

typedef struct WriteChannelDesc
{
	
	// The minimum and maximum version which can be used to
	// interpret this record:
	int32 minVersion;
	int32 maxVersion;
	
	//---------------------------------------------------------------------------
	//	Version 0 fields:
	//---------------------------------------------------------------------------
	
	// The next descriptor in the list:
	struct WriteChannelDesc *next;
	
	ChannelWritePort port;	// Port to use for writing.
	
	VRect bounds;			// Bounds of the channel data.
	int32 depth;			// Depth of the data.
	
	VPoint tileSize;		// Size of the tiles.
	VPoint tileOrigin;		// Origin for the tiles.
	
	int16 channelType;		// Channel type, see above. 
	
	int16 padding;			// Reserved. Defaults to zero.
	
	void *contextInfo;		// A pointer to additional info dependent on context.
	
	const char *name;		// Name of the channel as a C string.
	
} WriteChannelDesc;

//-------------------------------------------------------------------------------
//	Structures -- Channel Ports -- ReadChannelDesc
//-------------------------------------------------------------------------------
#define kCurrentMinVersReadChannelDesc	0
#define kCurrentMaxVersReadChannelDesc	2

typedef struct ReadChannelDesc
{
	
	// The minimum and maximum version which can be used to
	// interpret this record:
	int32 minVersion;
	int32 maxVersion;

	//---------------------------------------------------------------------------
	//	Version 0 fields:
	//---------------------------------------------------------------------------
		
	// The next descriptor in the list:
	struct ReadChannelDesc *next;
	
	PIChannelPort port;		// Port to use for reading.
	
	VRect bounds;			// Bounds of the channel data.
	int32 depth;			// Depth of the data.
	
	VPoint tileSize;		// Size of the tiles.
	VPoint tileOrigin;		// Origin for the tiles.
	
	Boolean target;			// Is this a target channel.
	Boolean shown;			// Is this channel shown?
	
	int16 channelType;		// Channel type. See above.
	
	void *contextInfo;		// Pointer to additional info dependent on context.
	
	const char *name;		// Name of the channel as a C string.

	//---------------------------------------------------------------------------
	//	Version 1 fields:
	//---------------------------------------------------------------------------

	PIChannelPort writePort;	/* Port to use for writing if writing is allowed.
								 This may not point back to the same collection
								 of pixels. In fact, it generally, won't. */
	
	//---------------------------------------------------------------------------
	//	Version 2 fields:
	//---------------------------------------------------------------------------

	unsigned32 alphaID;		/* Persistent ID for the channel. Only set for alpha
							   channels. */
	
} ReadChannelDesc;

//-------------------------------------------------------------------------------
//
//	Structures -- Channel Ports -- ImageDocumentDesc
//
//	We describe a document as a collection of lists of read channels.
//
//-------------------------------------------------------------------------------
#define kCurrentMinVersReadImageDocDesc	0
#define kCurrentMaxVersReadImageDocDesc	1

typedef struct ReadImageDocumentDesc
{
	
	// The minimum and maximum version which can be used to
	// interpret this record:
	int32 minVersion;
	int32 maxVersion;
	
	//---------------------------------------------------------------------------
	//	Version 0 fields:
	//---------------------------------------------------------------------------
	
	int32 imageMode;		// Color mode for the document.
	int32 depth;			// Default depth for the document.
	VRect bounds;			// Bounds for the document.
	
	Fixed hResolution;		// Resolution for the document.
	Fixed vResolution;
	
	LookUpTable redLUT;		// Color table for indexed color and duotone.
	LookUpTable greenLUT;
	LookUpTable blueLUT;
	
	// Composite channels in the target layer:
	ReadChannelDesc *targetCompositeChannels;
							
	// Transparency channel for the target layer:
	ReadChannelDesc *targetTransparency;
							
	// Layer mask for the target layer:
	ReadChannelDesc *targetLayerMask;
							
	// Composite channels in the merged data:
	ReadChannelDesc *mergedCompositeChannels;
							
	// Transparency channel for the merged data:
	ReadChannelDesc *mergedTransparency;
							
	// Alpha channels:
	ReadChannelDesc *alphaChannels;
							
	// Selection mask, if any:
	ReadChannelDesc *selection;

	// New with version 1.

	// The file specification if any.

	struct SPPlatformFileSpecification_t *fileSpec;
	
} ReadImageDocumentDesc;

//-------------------------------------------------------------------------------
//	Defines -- Prototypes -- Callbacks.
//-------------------------------------------------------------------------------
#ifdef __cplusplus

extern "C"
{
#endif

	// Direct callbacks:
	typedef MACPASCAL Boolean (*TestAbortProc) (void);

	typedef MACPASCAL void (*ProgressProc) (int32 done, int32 total);

	typedef MACPASCAL void (*HostProc) (int16 selector, int32 *data);

	typedef MACPASCAL int32 (*SpaceProc) (void);

	// Handle procs:
	typedef MACPASCAL Handle (*NewPIHandleProc) (int32 size);
	typedef MACPASCAL void (*DisposePIHandleProc) (Handle h);
	typedef MACPASCAL void (*DisposeRegularHandlePIHandleProc) (Handle h);

	typedef MACPASCAL int32 (*GetPIHandleSizeProc) (Handle h);
	typedef MACPASCAL OSErr (*SetPIHandleSizeProc) (Handle h, int32 newSize);
	typedef MACPASCAL Ptr (*LockPIHandleProc) (Handle h, Boolean moveHigh);
	typedef MACPASCAL void (*UnlockPIHandleProc) (Handle h);

	typedef MACPASCAL void (*RecoverSpaceProc) (int32 size);

	typedef MACPASCAL OSErr (*AllocateBufferProc) (int32 size, BufferID *bufferID);
	typedef MACPASCAL Ptr (*LockBufferProc) (BufferID bufferID, Boolean moveHigh);
	typedef MACPASCAL void (*UnlockBufferProc) (BufferID bufferID);
	typedef MACPASCAL void (*FreeBufferProc) (BufferID bufferID);
	typedef MACPASCAL int32 (*BufferSpaceProc) (void);

	typedef MACPASCAL int16 (*CountPIResourcesProc) (ResType type);
	typedef MACPASCAL Handle (*GetPIResourceProc) (ResType type, int16 index);
	typedef MACPASCAL void (*DeletePIResourceProc) (ResType type, int16 index);
	typedef MACPASCAL OSErr (*AddPIResourceProc) (ResType type, Handle data);

	typedef MACPASCAL void (*ProcessEventProc) (void *event);

	typedef MACPASCAL OSErr (*DisplayPixelsProc) (const PSPixelMap *source,
												  const VRect *srcRect,
												  int32 dstRow,
												  int32 dstCol,
												  void *platformContext);
											
	typedef MACPASCAL OSErr (*AdvanceStateProc) (void);

	// Property suite:
	typedef MACPASCAL OSErr (*GetPropertyProc) (PIType signature,
												PIType key,
												int32 index,
												int32 *simpleProperty,
												Handle *complexProperty);
	typedef MACPASCAL OSErr (*SetPropertyProc) (PIType signature,
												PIType key,
												int32 index,
												int32 simpleProperty,
												Handle complexProperty);

	typedef MACPASCAL OSErr (*ColorServicesProc) (ColorServicesInfo *info);

	typedef MACPASCAL OSErr (*PIResampleProc) (PSImagePlane *source,
											   PSImagePlane *destination,
											   Rect *area,
											   Fixed *coords,
											   int16 method);

	typedef MACPASCAL OSErr (*PIResampleMultiProc) (PSImageMultiPlane *source,
												   PSImageMultiPlane *destination,
												   Rect *area,
												   Fixed *coords,
												   int16 method);
											   
	// Channel ports suite:
	typedef MACPASCAL OSErr (*ReadPixelsProc) (ChannelReadPort port,
											   const PSScaling *scaling,
											   const VRect *writeRect,
											   		/* Rectangle in destination coordinates. */
											   const PixelMemoryDesc *destination,
											   VRect *wroteRect);
	typedef MACPASCAL OSErr (*WriteBasePixelsProc) (ChannelWritePort port,
													const VRect *writeRect,
													const PixelMemoryDesc *source);
	typedef MACPASCAL OSErr (*ReadPortForWritePortProc) (ChannelReadPort *readPort,
														 ChannelWritePort writePort);
	
#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------
//	Structures -- Buffer procs.
//-------------------------------------------------------------------------------

#define kCurrentBufferProcsVersion 2

#define kPIBufferSuite 			"Photoshop Buffer Suite for Plug-ins"
#define kPIBufferSuiteVersion	kCurrentBufferProcsVersion

typedef struct BufferProcs
{

	int16 bufferProcsVersion;
	
	int16 numBufferProcs;

	AllocateBufferProc allocateProc;

	LockBufferProc lockProc;

	UnlockBufferProc unlockProc;

	FreeBufferProc freeProc;
	
	BufferSpaceProc spaceProc;

} BufferProcs;

#define kCurrentBufferProcsCount \
	((sizeof(BufferProcs) - offsetof(BufferProcs, allocateProc)) / sizeof(void *)) 

//-------------------------------------------------------------------------------
//	Structures -- Resource Procs.
//-------------------------------------------------------------------------------

#define kCurrentResourceProcsVersion 3

#define kPIResourceSuite 			"Photoshop Resource Suite for Plug-ins"
#define kPIResourceSuiteVersion		kCurrentResourceProcsVersion

typedef struct ResourceProcs
{
	
	int16 resourceProcsVersion;
	
	int16 numResourceProcs;
	
	CountPIResourcesProc countProc;
	
	GetPIResourceProc getProc;
	
	DeletePIResourceProc deleteProc;
	
	AddPIResourceProc addProc;
	
} ResourceProcs;

#define kCurrentResourceProcsCount \
	((sizeof(ResourceProcs) - offsetof(ResourceProcs, countProc)) / sizeof(void *)) 

// Reserved resource types:

#define kDayStarColorMatchingResource 'DCSR'
#define kPhotoDeluxeResource 'PHUT'

//-------------------------------------------------------------------------------
//	Structures -- Handle Procs.
//-------------------------------------------------------------------------------

#define kCurrentHandleProcsVersion 1

#define kPIHandleSuite 				"Photoshop Handle Suite for Plug-ins"
#define kPIHandleSuiteVersion		kCurrentHandleProcsVersion

typedef struct HandleProcs
{
	
	int16 handleProcsVersion;
	
	int16 numHandleProcs;
	
	NewPIHandleProc newProc;
	
	DisposePIHandleProc disposeProc;

	GetPIHandleSizeProc getSizeProc;
	
	SetPIHandleSizeProc setSizeProc;
	
	LockPIHandleProc lockProc;
	
	UnlockPIHandleProc unlockProc;
	
	RecoverSpaceProc recoverSpaceProc;
	
	DisposeRegularHandlePIHandleProc disposeRegularHandleProc;
	
} HandleProcs;

#define kCurrentHandleProcsCount \
	((sizeof(HandleProcs) - offsetof(HandleProcs, newProc)) / sizeof(void *)) 

//-------------------------------------------------------------------------------
//	Structures -- Image Services suite.
//-------------------------------------------------------------------------------

#define kCurrentImageServicesProcsVersion 1

#define kPIImageServicesSuite 				"Photoshop Image Services Suite for Plug-ins"
#define kPIImageServicesSuiteVersion		kCurrentImageServicesProcsVersion
	
typedef struct ImageServicesProcs
{
	
	int16 imageServicesProcsVersion;
	int16 numImageServicesProcs;
	
	PIResampleProc interpolate1DProc;
	PIResampleProc interpolate2DProc;
	PIResampleMultiProc interpolate1DMultiProc;
	PIResampleMultiProc interpolate2DMultiProc;
	
} ImageServicesProcs;

#define kCurrentImageServicesProcsCount \
	((sizeof(ImageServicesProcs) - offsetof(ImageServicesProcs, interpolate1DProc)) / sizeof(void *)) 
	
//-------------------------------------------------------------------------------
//	Structures -- Property procs.
//-------------------------------------------------------------------------------

#define kCurrentPropertyProcsVersion 1

#define kPIPropertySuite 				"Photoshop Property Suite for Plug-ins"
#define kPIPropertySuiteVersion			kCurrentPropertyProcsVersion

typedef struct PropertyProcs
{
	
	int16 propertyProcsVersion;
	int16 numPropertyProcs;
	
	GetPropertyProc	getPropertyProc;
	SetPropertyProc setPropertyProc;
	
} PropertyProcs;

#define kCurrentPropertyProcsCount \
	((sizeof(PropertyProcs) - offsetof(PropertyProcs, getPropertyProc)) / sizeof(void *))

//-------------------------------------------------------------------------------
//	Structures -- Channel Ports.
//-------------------------------------------------------------------------------

#define kCurrentChannelPortProcsVersion 1

#define kPIChannelPortSuite 			"Photoshop Channel Ports Suite for Plug-ins"
#define kPIChannelPortSuiteVersion		kCurrentChannelPortProcsVersion
	
typedef struct ChannelPortProcs
{
	
	int16 channelPortProcsVersion;
	int16 numChannelPortProcs;
	
	ReadPixelsProc readPixelsProc;
	WriteBasePixelsProc writeBasePixelsProc;
	ReadPortForWritePortProc readPortForWritePortProc;
	
} ChannelPortProcs;

#define kCurrentChannelPortProcsCount \
	((sizeof(ChannelPortProcs) - offsetof(ChannelPortProcs, readPixelsProc)) / sizeof(void *)) 
	
//-------------------------------------------------------------------------------
//	Suite - Matrix Math
//-------------------------------------------------------------------------------

#define kPSMatrixMathSuite			"9cfaa249-e6f1-11d2-b8eb-0060b0c1f95d"
#define	kPSMatrixMathSuiteVersion	1

typedef struct PSMatrixMathProcs
	{
	/* kPSMatrixMathSuiteVersion == 1 functions */
	
	// if result == NULL solution is put in m 
	SPAPI Boolean (*Inverse) (PIAffineMatrix *m,
								PIAffineMatrix *result);
	
	SPAPI Boolean (*Equals) (const PIAffineMatrix *m1,
								const PIAffineMatrix *m2);

	// works in-place if result == NULL
	SPAPI void (*ScalarMultiply) (const double scalar,
										PIAffineMatrix *m,
											PIAffineMatrix *result);

	// if result == NULL solution is put in m2 
	SPAPI void (*MatrixMultiply) (const PIAffineMatrix *m1,
										PIAffineMatrix *m2,
											PIAffineMatrix *result);
	
	// if result == NULL solution is put in m 
	SPAPI void (*PointMultiply) (PIFloatPoint *p,
										const PIAffineMatrix *m,
											PIFloatPoint *result);
	
	
	// if result == NULL solution is put in p 
	SPAPI void (*PointsMultiply) (PIFloatPoint *p,
											int32 count,
												const PIAffineMatrix *m,
													PIFloatPoint *result);

	} PSMatrixMathProcs;

//-------------------------------------------------------------------------------
//
//	Defines -- Filter padding.
//
//	The following constants indicate what sort of padding to use.  Values 0
// 	through 255 specify a constant value.  Negative values imply special
// 	operations.
//
// 	If the plug-in does not want padding and does not want errors, then
// 	exterior areas will contain arbitrary values.
//
//-------------------------------------------------------------------------------
#define plugInWantsEdgeReplication			-1
#define plugInDoesNotWantPadding			-2
#define plugInWantsErrorOnBoundsException	-3

//-------------------------------------------------------------------------------
//	Defines -- Sampling flag
//-------------------------------------------------------------------------------
#define hostDoesNotSupportSampling		0
#define hostSupportsIntegralSampling	1
#define hostSupportsFractionalSampling	2

//-------------------------------------------------------------------------------
//	Defines -- Layout values for filters and export modules.
//-------------------------------------------------------------------------------

// Traditional = Rows, columns, planes with colbytes = # planes:
#define piLayoutTraditional				0

#define piLayoutRowsColumnsPlanes		1
#define piLayoutRowsPlanesColumns		2
#define piLayoutColumnsRowsPlanes		3
#define piLayoutColumnsPlanesRows		4
#define piLayoutPlanesRowsColumns		5
#define piLayoutPlanesColumnsRows		6

//-------------------------------------------------------------------------------
//	Defines -- Property suite -- PIInterface color property.
//-------------------------------------------------------------------------------
typedef struct PIInterfaceColor
{
	RGBtuple color32;	 // interface color for full color depth.
	RGBtuple color2;	 // interface color for B/W display.

} PIInterfaceColor;

// Interface color selectors:
#define kPIInterfaceButtonUpFill		1
#define kPIInterfaceBevelShadow 		2
#define kPIInterfaceIconFillActive		3
#define	kPIInterfaceIconFillDimmed  	4
#define	kPIInterfacePaletteFill 		5
#define	kPIInterfaceIconFrameDimmed 	6
#define	kPIInterfaceIconFrameActive 	7
#define	kPIInterfaceBevelHighlight  	8
#define	kPIInterfaceButtonDownFill  	9
#define	kPIInterfaceIconFillSelected	10
#define kPIInterfaceBorder				11
#define	kPIInterfaceButtonDarkShadow 	12
#define	kPIInterfaceIconFrameSelected 	13
#define	kPIInterfaceRed					14

//-------------------------------------------------------------------------------
//	Defines -- Photoshop -- SuitePEA caller and selector strings for Photoshop.
//-------------------------------------------------------------------------------
#define kPSPhotoshopCaller			"PS Photoshop"
#define kPSDoIt						"Do It"

//-------------------------------------------------------------------------------
//	Action plug-in specific PiPL properties
//-------------------------------------------------------------------------------

// 'prst' -- Always stay loaded property:
#define PIPersistentProperty		0x70727374L

//-------------------------------------------------------------------------------
//	Defines -- Image Resources for Annotation.
//-------------------------------------------------------------------------------

/* Major & Minor version number */
#define	kPIAnnotMajorVersion	0x0002
#define kPIAnnotMinorVersion	0x0001

/* keys */
#define	PIAnnotTextPopup	'txtA' /* text with popup annotation */
#define	PIAnnotSound		'sndA' /* sound annotation */
#define	PIAnnotTextContent	'txtC' /* text as content */
#define	PIAnnotSoundFile	'sndF' /* content as a sound file */
#define	PIAnnotSoundMem		'sndM' /* content as a sound in memory */
#define	PIFilterFlate		'fltD' /* default parameters */
#define	PIFilterLZW			'lzwD' /* default parameters */
#define PIFilterNone		'none' /* no compression */
#define PIFilterUndef		'    ' /* treated as 'none' */

//-------------------------------------------------------------------------------
//	Defines -- Image Resources for Jumpto.
//-------------------------------------------------------------------------------

/* Major & Minor version number */
#define	kPIJumptoMajorVersion	0x0001
#define kPIJumptoMinorVersion	0x0001

/* keys */
#define	PIJumptoDocDirty		'jtDd' /* document dirty bit when we jump to IR */
#define PIJumptoFileModDate	'jtMd' /* file mod date when we jump to IR */


//-------------------------------------------------------------------------------
//	Alignment.
//-------------------------------------------------------------------------------
#if defined(__BORLANDC__)
	#pragma option -a.
#endif

//-------------------------------------------------------------------------------

#endif // __PIGeneral_h__
