//-------------------------------------------------------------------------------
//
//	File:
//		PIActions.h
//
//	(c) Copyright 1996-1998, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file contains the public definitions and structures
//		used by all plug-ins for scripting control and access.
//
//	Use:
//		This file must be included to correctly compile.
//		It is included by all the plug-in headers.
//
//	Version history:
//		1.0.0	1/1/1996	SP		Initial Compilation.
//			Photoshop 4.0.
//
//		2.0.0	3/18/1998	SP		Photoshop 5.0.
//			Updated for for Photoshop 5.0. Suite structures and
//			new actions non-stream-based key access.
//
//-------------------------------------------------------------------------------
//  $Id: PIActions.h,v 1.1.1.1 2001/10/23 08:08:40 hawkfish Exp $ 

#ifndef __PIActions__ // Already defined?
#define __PIActions__

#ifndef MSWindows
#define MSWindows (MSDOS || WIN32)
#endif

#ifndef INSIDE_PHOTOSHOP
#define INSIDE_PHOTOSHOP 0
#endif


#ifdef Rez

#ifdef SystemSevenOrLater
#undef SystemSevenOrLater
#endif

#define SystemSevenOrLater 1

#if !MSWindows
#include "Types.r"
#include "AEUserTermTypes.r"
#include "AppleEvents.r"
#endif

#else

#include "PITypes.h"
#include "SPTypes.h"
#include "SPPlugs.h"

#if !MSWindows && !INSIDE_PHOTOSHOP
#include "AERegistry.h"
#endif

#endif // Rez

//-------------------------------------------------------------------------------
//	Flags for dictionary parameters.
//-------------------------------------------------------------------------------

#define flagsSingleParameter					\
	required,			singleItem,		notEnumerated,	reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsListParameter						\
	required,			listOfItems,	notEnumerated,	reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsEnumeratedParameter				\
	required,			singleItem,		enumerated,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsEnumeratedListParameter			\
	required,			listOfItems,	enumerated,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsOptionalSingleParameter			\
	optional,			singleItem,		notEnumerated,	reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsOptionalListParameter				\
	optional,			listOfItems,	notEnumerated,	reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsOptionalEnumeratedParameter		\
	optional,			singleItem,		enumerated,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

#define flagsOptionalEnumeratedListParameter	\
	optional,			listOfItems,	enumerated,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	prepositionParam,	notFeminine,	notMasculine,	singular

// Flags for object propeties:

#define flagsSingleProperty						\
	reserved,			singleItem,		notEnumerated,	readWrite,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	noApostrophe,		notFeminine,	notMasculine,	singular

#define flagsListProperty						\
	reserved,			listOfItems,	notEnumerated,	readWrite,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	noApostrophe,		notFeminine,	notMasculine,	singular

#define flagsEnumeratedProperty					\
	reserved,			singleItem,		enumerated,		readWrite,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	noApostrophe,		notFeminine,	notMasculine,	singular

#define flagsEnumeratedListProperty				\
	reserved,			listOfItems,	enumerated,		readWrite,	\
	reserved,			reserved,		reserved,		reserved,	\
	reserved,			reserved,		reserved,		reserved,	\
	noApostrophe,		notFeminine,	notMasculine,	singular

// Flags for direct dictionary parameters:

#define flagsOptionalEnumeratedChangesDirect	\
	directParamOptional,	singleItem,	enumerated,	changesState,	\
	reserved,				reserved,	reserved,	reserved,		\
	reserved,				reserved,	reserved,	reserved,		\
	reserved,				reserved,	reserved,	reserved

#define flagsEnumeratedChangesDirect	\
	directParamRequired,	singleItem,	enumerated,	changesState,	\
	reserved,				reserved,	reserved,	reserved,		\
	reserved,				reserved,	reserved,	reserved,		\
	reserved,				reserved,	reserved,	reserved

#define flagsEnumeratedDirect	\
	directParamRequired,	singleItem,	enumerated,	doesntChangeState,	\
	reserved,				reserved,	reserved,	reserved,			\
	reserved,				reserved,	reserved,	reserved,			\
	reserved,				reserved,	reserved,	reserved

#define flagsChangesDirect	\
	directParamRequired,	singleItem,	notEnumerated,	changesState,	\
	reserved,				reserved,	reserved,		reserved,		\
	reserved,				reserved,	reserved,		reserved,		\
	reserved,				reserved,	reserved,		reserved

#define flagsListChangesDirect	\
	directParamRequired,	listOfItems,	notEnumerated,	changesState,	\
	reserved,				reserved,		reserved,		reserved,		\
	reserved,				reserved,		reserved,		reserved,		\
	reserved,				reserved,		reserved,		reserved

//-------------------------------------------------------------------------------
//	Handy short cuts.
//-------------------------------------------------------------------------------

#define NO_REPLY		\
	noReply,			\
	"",					\
	replyRequired,	singleItem,	notEnumerated,	notTightBindingFunction,	\
	reserved,		reserved,	reserved,		reserved,					\
	reserved,		reserved,	reserved,		reserved,					\
	verbEvent,		reserved,	reserved,		reserved

#define IMAGE_DIRECT_PARAMETER	\
	typeImageReference,			\
	"",							\
	flagsOptionalEnumeratedChangesDirect
	
//-------------------------------------------------------------------------------

// NOTE: 1/21/98 (Ace) I'm going to leave these conditional defines in this
// file to keep PITerminology devoid of conditionals:

/* Normalize the AppleScript terminology */

#if MSWindows	// These items come from AppleScript

// New universal headers use enums, not defines!

#define typeFloat				'doub'
#define typeInteger				'long'
#define typeBoolean				'bool'
#define typeAlias				'alis'
#define typeNull				'null'
#define	roman					0
#define english					0
#define japanese				11

#ifndef noReply
#define noReply					typeNull
#endif

#ifndef __APPLEEVENTS__
#define typeType				'type'
#define typeChar				'TEXT'
#define typeObjectSpecifier		'obj '
#define typeEnumerated			'enum'
#endif /* __APPLEEVENTS __ */

#if !INSIDE_PHOTOSHOP

// The plugins need this stuff, but don't use the Universal Headers, so
// we must provide it for them.

#define formName				'name'

#ifndef __AEOBJECTS__
#define formAbsolutePosition	'indx'
#define formPropertyID			'prop'
#define formRelativePosition	'rele'
#endif /* __AEOBJECTS__ */

#endif	// !INSIDE_PHOTOSHOP

#endif /* MSWindows */

#if INSIDE_PHOTOSHOP || !defined(keySelection)
#define keySelection			'fsel'	// keySelection
#endif

#define typePath				'Pth '

#if MSWindows
#define typePlatformFilePath	typePath
#else
#define typePlatformFilePath	typeAlias
#endif

#include "PITerminology.h"

//-------------------------------------------------------------------------------

#ifndef Rez

//-------------------------------------------------------------------------------

#define PIHasTerminologyProperty	0x6873746DL	/* 'hstm' <PITerminology> Has <aete, 0> resource:
														version number
														classID:		Object or event class
														eventID:		For events (typeNull otherwise)
														terminologyID:	'aete' resource ID (ignored if PITerminologyProperty)
														scopeString:	Unique string present if
																		AppleScript not supported
												*/
#define PITerminologyProperty		0x74726D6EL	/* 'trmn' <aete> Used internaly as terminology cache.
													If present it is used instead of an aete resource.
												*/

// These are the flags for recordInfo:
typedef int32 PIDialogRecordOptions;
enum
	{
	plugInDialogOptional,
	plugInDialogRequired,
	plugInDialogNone
	};

// These flags are used for playInfo:
typedef int32 PIDialogPlayOptions;
enum
	{
	plugInDialogDontDisplay,
	plugInDialogDisplay,
	plugInDialogSilent
	};
		
//-------------------------------------------------------------------------------
//	Flags returned by GetKey. Low order word corresponds to AppleEvent flags.
//-------------------------------------------------------------------------------

#define actionSimpleParameter		0x00000000L
#define actionEnumeratedParameter	0x00002000L
#define actionListParameter			0x00004000L
#define actionOptionalParameter		0x00008000L

#define actionObjectParameter		0x80000000L
#define actionScopedParameter		0x40000000L
#define actionStringIDParameter		0x20000000L

//-------------------------------------------------------------------------------

typedef Handle PIDescriptorHandle;

typedef struct PIOpaqueWriteDescriptor*	PIWriteDescriptor;
typedef struct PIOpaqueReadDescriptor*	PIReadDescriptor;

typedef unsigned long	DescriptorEventID;
typedef unsigned long	DescriptorClassID;

typedef unsigned long	DescriptorKeyID;
typedef unsigned long	DescriptorTypeID;
typedef unsigned long	DescriptorUnitID;
typedef unsigned long	DescriptorEnumID;
typedef unsigned long	DescriptorEnumTypeID;

typedef unsigned long	DescriptorFormID;

typedef DescriptorKeyID	DescriptorKeyIDArray[];


#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#if WIN32
#pragma pack(push,1)
#endif

typedef struct PITerminology
	{
	int32	version;
	OSType	classID;
	OSType	eventID;
	int16	terminologyID;
	char	scopeString[1];		// C string
	} PITerminology;

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

#if WIN32
#pragma pack(pop)
#endif

#define PITerminologyMinSize 15

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct PIDescriptorSimpleReference
	{
	DescriptorTypeID	desiredClass;
	DescriptorKeyID		keyForm;
	struct _keyData
		{
		Str255				name;
		int32				index;
		DescriptorTypeID	type;
		DescriptorEnumID	value;
		}				keyData;
	} PIDescriptorSimpleReference;
	
#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

//-------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
	{
#endif

//-------------------------------------------------------------------------------
//	WriteDescriptor callbacks.
//-------------------------------------------------------------------------------
typedef MACPASCAL PIWriteDescriptor (*OpenWriteDescriptorProc)(void);
typedef MACPASCAL OSErr (*CloseWriteDescriptorProc)(PIWriteDescriptor, PIDescriptorHandle*);

typedef MACPASCAL OSErr (*PutIntegerProc)(PIWriteDescriptor, DescriptorKeyID, int32);
typedef MACPASCAL OSErr (*PutFloatProc)(PIWriteDescriptor, DescriptorKeyID, const double*);
typedef MACPASCAL OSErr (*PutUnitFloatProc)(PIWriteDescriptor, DescriptorKeyID, DescriptorUnitID, const double*);
typedef MACPASCAL OSErr (*PutBooleanProc)(PIWriteDescriptor, DescriptorKeyID, Boolean);
typedef MACPASCAL OSErr (*PutTextProc)(PIWriteDescriptor, DescriptorKeyID, Handle);
typedef MACPASCAL OSErr (*PutAliasProc)(PIWriteDescriptor, DescriptorKeyID, Handle);

typedef MACPASCAL OSErr (*PutEnumeratedProc)(PIWriteDescriptor, DescriptorKeyID key, DescriptorTypeID type, DescriptorEnumID value);
typedef MACPASCAL OSErr (*PutClassProc)(PIWriteDescriptor, DescriptorKeyID, DescriptorTypeID);
typedef MACPASCAL OSErr (*PutSimpleReferenceProc)(PIWriteDescriptor, DescriptorKeyID, const PIDescriptorSimpleReference*);
typedef MACPASCAL OSErr (*PutObjectProc)(PIWriteDescriptor, DescriptorKeyID, DescriptorTypeID, PIDescriptorHandle);

typedef MACPASCAL OSErr	(*PutCountProc)(PIWriteDescriptor, DescriptorKeyID, uint32 count);

typedef MACPASCAL OSErr (*PutStringProc)(PIWriteDescriptor, DescriptorKeyID, ConstStr255Param);

// Scoped classes are not for use by plug-ins in Photoshop 4.0:
typedef MACPASCAL OSErr (*PutScopedClassProc)(PIWriteDescriptor, DescriptorKeyID, DescriptorTypeID);
typedef MACPASCAL OSErr (*PutScopedObjectProc)(PIWriteDescriptor, DescriptorKeyID, DescriptorTypeID, PIDescriptorHandle);

//-------------------------------------------------------------------------------
//	ReadDescriptor callbacks.
//-------------------------------------------------------------------------------
typedef MACPASCAL PIReadDescriptor (*OpenReadDescriptorProc)(PIDescriptorHandle, DescriptorKeyIDArray);
typedef MACPASCAL OSErr (*CloseReadDescriptorProc)(PIReadDescriptor);
typedef MACPASCAL Boolean (*GetKeyProc)(PIReadDescriptor, DescriptorKeyID* key, DescriptorTypeID* type, int32* flags);

typedef MACPASCAL OSErr (*GetIntegerProc)(PIReadDescriptor, int32*);
typedef MACPASCAL OSErr (*GetFloatProc)(PIReadDescriptor, double*);
typedef MACPASCAL OSErr (*GetUnitFloatProc)(PIReadDescriptor, DescriptorUnitID*, double*);
typedef MACPASCAL OSErr (*GetBooleanProc)(PIReadDescriptor, Boolean*);
typedef MACPASCAL OSErr (*GetTextProc)(PIReadDescriptor, Handle*);
typedef MACPASCAL OSErr (*GetAliasProc)(PIReadDescriptor, Handle*);

typedef MACPASCAL OSErr (*GetEnumeratedProc)(PIReadDescriptor, DescriptorEnumID*);
typedef MACPASCAL OSErr (*GetClassProc)(PIReadDescriptor, DescriptorTypeID*);
typedef MACPASCAL OSErr (*GetSimpleReferenceProc)(PIReadDescriptor, PIDescriptorSimpleReference*);
typedef MACPASCAL OSErr (*GetObjectProc)(PIReadDescriptor, DescriptorTypeID*, PIDescriptorHandle*);

typedef MACPASCAL OSErr (*GetCountProc)(PIReadDescriptor, uint32*);

typedef MACPASCAL OSErr (*GetStringProc)(PIReadDescriptor, Str255*);
typedef MACPASCAL OSErr (*GetPinnedIntegerProc)(PIReadDescriptor, int32 min, int32 max, int32*);
typedef MACPASCAL OSErr (*GetPinnedFloatProc)(PIReadDescriptor, const double* min, const double* max, double*);
typedef MACPASCAL OSErr (*GetPinnedUnitFloatProc)(PIReadDescriptor, const double* min, const double* max, DescriptorUnitID*, double*);

//-------------------------------------------------------------------------------

#ifdef __cplusplus
	}
#endif

//-------------------------------------------------------------------------------
//	WriteDescriptorProcs structure.
//-------------------------------------------------------------------------------

// Version:
#define kCurrentWriteDescriptorProcsVersion 0

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct WriteDescriptorProcs
	{
	int16 writeDescriptorProcsVersion;
	int16 numWriteDescriptorProcs;
	
	OpenWriteDescriptorProc		openWriteDescriptorProc;
	CloseWriteDescriptorProc	closeWriteDescriptorProc;

	PutIntegerProc				putIntegerProc;
	PutFloatProc				putFloatProc;
	PutUnitFloatProc			putUnitFloatProc;
	PutBooleanProc				putBooleanProc;
	PutTextProc					putTextProc;
	PutAliasProc				putAliasProc;

	PutEnumeratedProc			putEnumeratedProc;
	PutClassProc				putClassProc;
	PutSimpleReferenceProc		putSimpleReferenceProc;
	PutObjectProc				putObjectProc;
	
	PutCountProc				putCountProc;
	
	PutStringProc				putStringProc;
	
	/* Scoped classes are not for use by plug-ins in Photoshop 4.0 */
	
	PutScopedClassProc			putScopedClassProc;
	PutScopedObjectProc			putScopedObjectProc;
	
	} WriteDescriptorProcs;

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

// Count:
#define kCurrentWriteDescriptorProcsCount \
	((sizeof(WriteDescriptorProcs) - offsetof(WriteDescriptorProcs, openWriteDescriptorProc)) / sizeof(void*))
	
//-------------------------------------------------------------------------------
//	ReadDescriptorProcs structure.
//-------------------------------------------------------------------------------

// Version:
#define kCurrentReadDescriptorProcsVersion 0

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct ReadDescriptorProcs
	{
	int16 readDescriptorProcsVersion;
	int16 numReadDescriptorProcs;
	
	OpenReadDescriptorProc		openReadDescriptorProc;
	CloseReadDescriptorProc		closeReadDescriptorProc;
	GetKeyProc					getKeyProc;

	GetIntegerProc				getIntegerProc;
	GetFloatProc				getFloatProc;
	GetUnitFloatProc			getUnitFloatProc;
	GetBooleanProc				getBooleanProc;
	GetTextProc					getTextProc;
	GetAliasProc				getAliasProc;

	GetEnumeratedProc			getEnumeratedProc;
	GetClassProc				getClassProc;
	GetSimpleReferenceProc		getSimpleReferenceProc;
	GetObjectProc				getObjectProc;

	GetCountProc				getCountProc;
	
	GetStringProc				getStringProc;
	GetPinnedIntegerProc		getPinnedIntegerProc;
	GetPinnedFloatProc			getPinnedFloatProc;
	GetPinnedUnitFloatProc		getPinnedUnitFloatProc;

	} ReadDescriptorProcs;

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

// Count:
#define kCurrentReadDescriptorProcsCount \
	((sizeof(ReadDescriptorProcs) - offsetof(ReadDescriptorProcs, openReadDescriptorProc)) / sizeof(void*))
	
//-------------------------------------------------------------------------------
//	PIDescriptorParameters structure.
//-------------------------------------------------------------------------------

// Version:
#define kCurrentDescriptorParametersVersion 0

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct PIDescriptorParameters
	{
	int16					descriptorParametersVersion;
	int16					playInfo;
	int16					recordInfo;
	
	PIDescriptorHandle		descriptor;
	
	WriteDescriptorProcs*	writeDescriptorProcs;
	ReadDescriptorProcs*	readDescriptorProcs;
	} PIDescriptorParameters;

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

//-------------------------------------------------------------------------------
//	Window notify suite.
//-------------------------------------------------------------------------------
// The window notify suite is used on the Macintosh to receive event notification
// during play:

#if !MSWindows

#define kPSWindowNotifySuite	"482963f4-3ea1-11d1-8da3-00c04fd5f7ee"
#define kPSWindowNotifySuiteVersion 1

#ifdef __cplusplus
extern "C"
	{
#endif

	typedef void (*PIWindowNotifier)(const EventRecord* event, void* data);
	
#ifdef __cplusplus
	}
#endif

typedef struct PSWindowNotifyProcs
	{
	SPAPI OSErr (*AddNotify)(WindowRef window, PIWindowNotifier notifier, void* data);
	SPAPI OSErr (*RemoveNotify)(WindowRef window);
	} PSWindowNotifyProcs;
	
#endif

//-------------------------------------------------------------------------------

typedef struct _ADsc*	PIActionDescriptor;
typedef struct _ALst*	PIActionList;
typedef struct _ARef*	PIActionReference;

// other define for this is in ASZStringSuite.h;  placed here for SuspendHistory
#ifndef ASZString_defined
#define ASZString_defined
struct ASZByteRun;
typedef struct ASZByteRun *ASZString;
#endif

//-------------------------------------------------------------------------------
//	ActionControl suite.  Available only in Automation plug-ins.
//-------------------------------------------------------------------------------
#define kPSActionControlSuite			"bc1e4d70-f953-11d0-8d9f-00c04fd5f7ee"
#define kPSActionControlSuiteVersion	 	3	// Current version, but it's
#define kPSActionControlSuitePrevVersion	2	// compatible with version 2

#ifdef __cplusplus
extern "C"
	{
#endif

	// Use PIEventNotifier as the prototype for your routine that will
	// receive action event notification (see AddNotify(), below):
	typedef void (*PIEventNotifier)
		(
		DescriptorEventID eventID,		// Event that occurred.
		PIActionDescriptor descriptor,	// Copy of result descriptor.
		PIDialogRecordOptions options,	// Options passed from event.
		void* data						// Plug-in data.
		);
		
	// Use PISuspendProc as the prototype for your routine that will
	// play actions which will NOT get recorded to the Actions Palette
	// (see SuspendHistory(), below):
	typedef SPErr (*PISuspendProc) 
		(
		void *data						// Plug-in data.
		);
		
#ifdef __cplusplus
	}
#endif		

typedef struct PSActionControlProcs
	{

	// Play dispatches an event into the actions mechanism.  You must
	// supply the event's ID (or runtime ID), a 4-character long; a descriptor
	// describing any specific parameters for that event (if any), and the dialog
	// options for whether to pop a dialog, don't pop a dialog (unless necessary),
	// or stay absolutely silent. If it returns an error, the result descriptor
	// may contain keyMessage, a string with the error message:
	SPAPI OSErr (*Play)
		(
		/* OUT */	PIActionDescriptor* result, 	// When done, free this if not NULL.
		/* IN */	DescriptorEventID event, 		// Event to play. See PITerminology.h.
		/* IN */	PIActionDescriptor descriptor, 	// Descriptor for event.
		/* IN */	PIDialogPlayOptions options		// plugInDialogDisplay, dontDisplay, silent.
		);
	
	// Get. The actions system also acts as a database for properties.  Use the Get
	// routine as a quick way to get properties out of the host.  You must build
	// a reference to the object and the property, and it will return a descriptor
	// with the pertinent information:
	SPAPI OSErr (*Get)
		(
		/* OUT */	PIActionDescriptor* result,		// When done, free this if not NULL. 
		/* IN */	PIActionReference reference		// Reference to property. See reference suite.
		);
		
	// StringIDtoTypeID takes the ScopeString from the HasTerminology PiPL property and
	// returns its runtime ID.  If the string is not present, it generates a new
	// runtime ID, so the only way to know if the ID is valid is whether the Play
	// command returns with no error when passing this as the event:
	SPAPI OSErr (*StringIDToTypeID)
		(
		/* IN */	char *stringID,				// ScopeString from HasTerminology PiPL property. 
		/* OUT */	DescriptorTypeID *typeID	// 4-character runtime ID.
		);
		
	// TypeIDToStringID requires a runtime ID and returns the HasTerminology PiPL property's
	// ScopeString, if valid:
	SPAPI OSErr (*TypeIDToStringID)
		(
		/* IN */	DescriptorTypeID typeID, 	// Runtime IDs only.
		/* OUT */	char *stringID, 			// ScopeString from HasTerminology PiPL property.
		/* IN */	uint32 stringLength			// Buffer size/Maximum string length.
		);

	// AddNotify specifies a notification routine for specific events.  Use
	// "eventAll" to get notified on all events.
	//
	// NOTE: It is your responsibility to remove any notifiers you add.
	//
	// Any one event can only dispatch to one notifier routine, but you can have
	// one notifier routine receive multiple events:
	SPAPI OSErr (*AddNotify)
		(
		/* IN */	SPPluginRef ref, 			// Plug-in reference to calling plug-in.
		/* IN */	DescriptorEventID event, 	// Any event. Use "eventAll" for all.
		/* IN */	PIEventNotifier notifier, 	// Routine to call when event occurs.
		/* IN */	void* data					// Any local data.
		);

	// RemoveNotify. NOTE: It is your responsibility to remove any notifiers you add:	
	SPAPI OSErr (*RemoveNotify)
		(
		/* IN */	SPPluginRef ref, 			// Plug-in reference to calling plug-in.
		/* IN */	DescriptorEventID event		// Specific event to remove notifier for.
		);
		
	// SuspendHistory.  Given a reference to a document, SuspendHistory will suspend recording
	//	of events to the History Palette, then call proc(data).  When proc returns, history
	//	recording will be re-enabled, and a final state will be added to the History palette
	//	with the label specified by operationName.
	SPAPI OSErr (*SuspendHistory)
		(
		/* IN */	PIActionReference reference,// Reference to a doc (or something contained in one)
		/* IN */	PISuspendProc proc,			// Callback function in the plugin	
		/* IN */	void *procData,				// Data to be sent to proc
		/* IN */	ASZString operationName		// Name of the operation Actions palette
		);

	// MakeStringAlias creates a new string with the same runtime ID as an existing
	// string. This is useful when the original string contains characters that
	// aren't legal in C identifiers and thus can't be used in the various string-
	// based descriptor calls

	SPAPI OSErr (*MakeStringAlias)
		(
		/* IN */	const char*	newStringID,		// The new string to be created
		/* IN */	const char*	existingStringID	// The string whose value it will share
		);

	} PSActionControlProcs;

//-------------------------------------------------------------------------------
//	Basic Action control suite.  Available to non-Automation plug-ins.
//-------------------------------------------------------------------------------
#define kPSBasicActionControlSuite			"3eb7c480-8797-11d1-8da7-00c04fd5f7ee"
#define kPSBasicActionControlSuiteVersion		2	// Current version, but it's
#define kPSBasicActionControlSuitePrevVersion	1	// compatible with previous version

typedef struct PSBasicActionControlProcs
	{

	// Get. The actions system also acts as a database for properties.  Use the Get
	// routine as a quick way to get properties out of the host.  You must build
	// a reference to the object and the property, and it will return a descriptor
	// with the pertinent information:
	SPAPI OSErr (*Get)
		(
		/* OUT */	PIActionDescriptor* result,		// When done, free this if not NULL. 
		/* IN */	PIActionReference reference		// Reference to property. See reference suite.
		);
		
	// StringIDtoTypeID takes the ScopeString from the HasTerminology PiPL property and
	// returns its runtime ID.  If the string is not present, it generates a new
	// runtime ID, so the only way to know if the ID is valid is whether the Play
	// command returns with no error when passing this as the event:
	SPAPI OSErr (*StringIDToTypeID)
		(
		/* IN */	char *stringID,				// ScopeString from HasTerminology PiPL property. 
		/* OUT */	DescriptorTypeID *typeID	// 4-character runtime ID.
		);
		
	// TypeIDToStringID requires a runtime ID and returns the HasTerminology PiPL property's
	// ScopeString, if valid:
	SPAPI OSErr (*TypeIDToStringID)
		(
		/* IN */	DescriptorTypeID typeID, 	// Runtime IDs only.
		/* OUT */	char *stringID, 			// ScopeString from HasTerminology PiPL property.
		/* IN */	uint32 stringLength			// Buffer size/Maximum string length.
		);

	// MakeStringAlias creates a new string with the same runtime ID as an existing
	// string. This is useful when the original string contains characters that
	// aren't legal in C identifiers and thus can't be used in the various string-
	// based descriptor calls

	SPAPI OSErr (*MakeStringAlias)
		(
		/* IN */	const char*	newStringID,		// The new string to be created
		/* IN */	const char*	existingStringID	// The string whose value it will share
		);

	} PSBasicActionControlProcs;

//-------------------------------------------------------------------------------
//	DescriptorRegistry suite.  Use to pass descriptors between plug-ins or
//	save parameters between invocations.
//-------------------------------------------------------------------------------
#define kPSDescriptorRegistrySuite			"61e608b0-40fd-11d1-8da3-00c04fd5f7ee"
#define	kPSDescriptorRegistrySuiteVersion	1

typedef struct PSDescriptorRegistryProcs
	{

	// Register. To add a descriptor to the registry, supply a unique id key,
	// the descriptor, and then mark it persistent to get saved and restored
	// across invocations of the app. (Perfect for preferences-like things):
	SPAPI OSErr (*Register)
		(
		/* IN */	const char* key, 				// Unique string or ID.
		/* IN */	PIActionDescriptor descriptor, 	// Descriptor with pertinent keys.
		/* IN */	Boolean isPersistent			// Indicate whether to save and restore.
		);
		
	// Erase. Remove a key from the registry:
	SPAPI OSErr (*Erase)
		(
		/* IN */	const char* key					// Unique string or ID.
		);
		
	// Get. Return a key from the registry. The descriptor is a copy, so it
	// is your responsibility to Free it when done:
	SPAPI OSErr (*Get)
		(
		/* IN */	const char* key, 				// Unique string or ID.
		/* OUT */	PIActionDescriptor* descriptor	// When done, free if not NULL.
		);

	} PSDescriptorRegistryProcs;

//-------------------------------------------------------------------------------
//	ActionDescriptor suite.  Use for putting keys and objects into descriptors,
//	getting them out of descriptors, and other descriptor management.
//-------------------------------------------------------------------------------
#define kPSActionDescriptorSuite			"df135115-c769-11d0-8079-00c04fd7ec47"
#define	kPSActionDescriptorSuiteVersion		2

typedef struct PSActionDescriptorProcs
	{
	// ALLOCATES: descriptor
	SPAPI OSErr (*Make)(PIActionDescriptor *descriptor);
	
	SPAPI OSErr (*Free)(PIActionDescriptor descriptor);
	
	SPAPI OSErr (*GetType)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorTypeID *type);
	
	// index is zero based
	SPAPI OSErr (*GetKey)(PIActionDescriptor descriptor, uint32 index, DescriptorKeyID *key);
	
	SPAPI OSErr (*HasKey)(PIActionDescriptor descriptor, DescriptorKeyID key, Boolean *hasKey);
	SPAPI OSErr (*GetCount)(PIActionDescriptor descriptor, uint32 *count);
	SPAPI OSErr (*IsEqual)(PIActionDescriptor descriptor, PIActionDescriptor other, Boolean *isEqual);
	
	SPAPI OSErr (*Erase)(PIActionDescriptor descriptor, DescriptorKeyID key);
	SPAPI OSErr (*Clear)(PIActionDescriptor descriptor);
	
	SPAPI OSErr (*PutInteger)(PIActionDescriptor descriptor, DescriptorKeyID key, int32 value);
	SPAPI OSErr (*PutFloat)(PIActionDescriptor descriptor, DescriptorKeyID key, double value);
	SPAPI OSErr (*PutUnitFloat)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorUnitID unit, double value);
	SPAPI OSErr (*PutString)(PIActionDescriptor descriptor, DescriptorKeyID key,  char *cstrValue);
	SPAPI OSErr (*PutBoolean)(PIActionDescriptor descriptor, DescriptorKeyID key, Boolean value);
	SPAPI OSErr (*PutList)(PIActionDescriptor descriptor, DescriptorKeyID key,  PIActionList value);
	SPAPI OSErr (*PutObject)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID type,  PIActionDescriptor value);
	SPAPI OSErr (*PutGlobalObject)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID type,  PIActionDescriptor value);
	SPAPI OSErr (*PutEnumerated)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorEnumTypeID type, DescriptorEnumID value);
	SPAPI OSErr (*PutReference)(PIActionDescriptor descriptor, DescriptorKeyID key,  PIActionReference value);
	SPAPI OSErr (*PutClass)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID value);
	SPAPI OSErr (*PutGlobalClass)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID value);
	SPAPI OSErr (*PutAlias)(PIActionDescriptor descriptor, DescriptorKeyID key, Handle value);

	SPAPI OSErr (*GetInteger)(PIActionDescriptor descriptor, DescriptorKeyID key, int32* value);
	SPAPI OSErr (*GetFloat)(PIActionDescriptor descriptor, DescriptorKeyID key, double* value);
	SPAPI OSErr (*GetUnitFloat)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorUnitID* unit, double* value);
	SPAPI OSErr (*GetStringLength)(PIActionDescriptor descriptor, DescriptorKeyID key, uint32 *stringLength);
	SPAPI OSErr (*GetString)(PIActionDescriptor descriptor, DescriptorKeyID key, char *cstrValue, uint32 maxLength);
	SPAPI OSErr (*GetBoolean)(PIActionDescriptor descriptor, DescriptorKeyID key, Boolean* value);
	SPAPI OSErr (*GetList)(PIActionDescriptor descriptor, DescriptorKeyID key, PIActionList* value);
	SPAPI OSErr (*GetObject)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID* type, PIActionDescriptor* value);
	SPAPI OSErr (*GetGlobalObject)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID* type, PIActionDescriptor* value);
	SPAPI OSErr (*GetEnumerated)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorEnumTypeID* type, DescriptorEnumID* value);
	SPAPI OSErr (*GetReference)(PIActionDescriptor descriptor, DescriptorKeyID key, PIActionReference* value);
	SPAPI OSErr (*GetClass)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID* value);
	SPAPI OSErr (*GetGlobalClass)(PIActionDescriptor descriptor, DescriptorKeyID key, DescriptorClassID* value);
	SPAPI OSErr (*GetAlias)(PIActionDescriptor descriptor, DescriptorKeyID key, Handle* value);

	SPAPI OSErr (*HasKeys)(PIActionDescriptor descriptor, DescriptorKeyIDArray requiredKeys, Boolean *hasKeys);
	SPAPI OSErr (*PutIntegers)(PIActionDescriptor descriptor, DescriptorKeyID key, uint32 count,  int32* value);
	SPAPI OSErr (*GetIntegers)(PIActionDescriptor descriptor, DescriptorKeyID key, uint32 count, int32* value);

	SPAPI OSErr (*AsHandle)(PIActionDescriptor descriptor, PIDescriptorHandle *value);
	SPAPI OSErr (*HandleToDescriptor)(PIDescriptorHandle value, PIActionDescriptor *descriptor);
	
	SPAPI OSErr (*PutZString)(PIActionDescriptor descriptor, DescriptorKeyID key, ASZString zstring);
	SPAPI OSErr (*GetZString)(PIActionDescriptor descriptor, DescriptorKeyID key, ASZString *zstring);
	
	SPAPI OSErr (*PutData)(PIActionDescriptor descriptor, DescriptorKeyID key, int32 length, void *value);
	SPAPI OSErr (*GetDataLength)(PIActionDescriptor descriptor, DescriptorKeyID key, int32* value);
	SPAPI OSErr (*GetData)(PIActionDescriptor descriptor, DescriptorKeyID key, void* value);
	
	} PSActionDescriptorProcs;

//-------------------------------------------------------------------------------
//	ActionList suite.  Use to manage lists of keys.
//-------------------------------------------------------------------------------
#define kPSActionListSuite			"df135116-c769-11d0-8079-00c04fd7ec47"
#define	kPSActionListSuiteVersion	1

typedef struct PSActionListProcs
	{
	SPAPI OSErr (*Make)(PIActionList *actionList);
	SPAPI OSErr (*Free)(PIActionList actionList);

	SPAPI OSErr (*GetType)(PIActionList list, uint32 index, DescriptorTypeID *value);

	SPAPI OSErr (*GetCount)(PIActionList list, uint32 *value);

	SPAPI OSErr (*PutInteger)(PIActionList list, int32 value);
	SPAPI OSErr (*PutFloat)(PIActionList list, double value);
	SPAPI OSErr (*PutUnitFloat)(PIActionList list, DescriptorUnitID unit, double value);
	SPAPI OSErr (*PutString)(PIActionList list, char *cstr);
	SPAPI OSErr (*PutBoolean)(PIActionList list, Boolean value);
	SPAPI OSErr (*PutList)(PIActionList list, PIActionList value);
	SPAPI OSErr (*PutObject)(PIActionList list, DescriptorClassID type, PIActionDescriptor value);
	SPAPI OSErr (*PutGlobalObject)(PIActionList list, DescriptorClassID type, PIActionDescriptor value);
	SPAPI OSErr (*PutEnumerated)(PIActionList list, DescriptorEnumTypeID type, DescriptorEnumID value);
	SPAPI OSErr (*PutReference)(PIActionList list, PIActionReference value);
	SPAPI OSErr (*PutClass)(PIActionList list, DescriptorClassID value);
	SPAPI OSErr (*PutGlobalClass)(PIActionList list, DescriptorClassID value);
	SPAPI OSErr (*PutAlias)(PIActionList list, Handle value);

	SPAPI OSErr (*GetInteger)(PIActionList list, uint32 index, int32 *value);
	SPAPI OSErr (*GetFloat)(PIActionList list, uint32 index, double *value);
	SPAPI OSErr (*GetUnitFloat)(PIActionList list, uint32 index, DescriptorUnitID* unit, double *value);
	SPAPI OSErr (*GetStringLength)(PIActionList list, uint32 index, uint32 *stringLength);
	SPAPI OSErr (*GetString)(PIActionList list, uint32 index, char *cstr, uint32 maxLength);
	SPAPI OSErr (*GetBoolean)(PIActionList list, uint32 index, Boolean *value);
	SPAPI OSErr (*GetList)(PIActionList list, uint32 index, PIActionList *actionList);
	SPAPI OSErr (*GetObject)(PIActionList list, uint32 index, DescriptorClassID* type, PIActionDescriptor *value);
	SPAPI OSErr (*GetGlobalObject)(PIActionList list, uint32 index, DescriptorClassID* type, PIActionDescriptor *value);
	SPAPI OSErr (*GetEnumerated)(PIActionList list, uint32 index, DescriptorEnumTypeID* type, DescriptorEnumID *value);
	SPAPI OSErr (*GetReference)(PIActionList list, uint32 index, PIActionReference *value);
	SPAPI OSErr (*GetClass)(PIActionList list, uint32 index, DescriptorClassID *value);
	SPAPI OSErr (*GetGlobalClass)(PIActionList list, uint32 index, DescriptorClassID *value);
	SPAPI OSErr (*GetAlias)(PIActionList list, uint32 index, Handle *aliasHandle);

	SPAPI OSErr (*PutIntegers)(PIActionList list, uint32 count, int32* value);
	SPAPI OSErr (*GetIntegers)(PIActionList list, uint32 count, int32* value);

	SPAPI OSErr (*PutData)(PIActionList list, int32 length, void *data);
	SPAPI OSErr (*GetDataLength)(PIActionList list, uint32 index, int32* length);
	SPAPI OSErr (*GetData)(PIActionList list, uint32 index, void* value);

	SPAPI OSErr (*PutZString)(PIActionList list, ASZString zstring);
	SPAPI OSErr (*GetZString)(PIActionList list, uint32 index, ASZString *zstring);

	} PSActionListProcs;

//-------------------------------------------------------------------------------
//	ActionReference suite.  Use to build references to objects to be able to
//	get/set properties and target events.
//-------------------------------------------------------------------------------
#define kPSActionReferenceSuite			"df135117-c769-11d0-8079-00c04fd7ec47"
#define	kPSActionReferenceSuiteVersion	2

typedef struct PSActionReferenceProcs
	{
	SPAPI OSErr (*Make)(PIActionReference *reference);
	SPAPI OSErr (*Free)(PIActionReference ref);
	SPAPI OSErr (*GetForm)(PIActionReference ref, DescriptorFormID *value);
	SPAPI OSErr (*GetDesiredClass)(PIActionReference ref, DescriptorClassID *value);
	SPAPI OSErr (*PutName)(PIActionReference ref, DescriptorClassID desiredClass, char *cstrValue);
	SPAPI OSErr (*PutIndex)(PIActionReference ref, DescriptorClassID desiredClass, uint32 value);
	SPAPI OSErr (*PutIdentifier)(PIActionReference ref, DescriptorClassID desiredClass, uint32 value);
	SPAPI OSErr (*PutOffset)(PIActionReference ref, DescriptorClassID desiredClass, int32 value);
	SPAPI OSErr (*PutEnumerated)(PIActionReference ref, DescriptorClassID desiredClass, DescriptorEnumTypeID type, DescriptorEnumID value);
	SPAPI OSErr (*PutProperty)(PIActionReference ref, DescriptorClassID desiredClass, DescriptorKeyID value);
	SPAPI OSErr (*PutClass)(PIActionReference ref, DescriptorClassID desiredClass);
	
	SPAPI OSErr (*GetNameLength)(PIActionReference ref, uint32 *stringLength);
	SPAPI OSErr (*GetName)(PIActionReference ref, char *name, uint32 maxLength);
	SPAPI OSErr (*GetIndex)(PIActionReference ref, uint32 *value);
	SPAPI OSErr (*GetIdentifier)(PIActionReference ref, uint32 *value);
	SPAPI OSErr (*GetOffset)(PIActionReference ref, int32 *value);
	SPAPI OSErr (*GetEnumerated)(PIActionReference ref, DescriptorEnumTypeID* type, DescriptorEnumID *enumValue);
	SPAPI OSErr (*GetProperty)(PIActionReference ref, DescriptorKeyID *value);
	SPAPI OSErr (*GetContainer)(PIActionReference ref, PIActionReference *value);

	} PSActionReferenceProcs;


//-------------------------------------------------------------------------------
//	PIActionParameters structure.
//-------------------------------------------------------------------------------
#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

#if WIN32
#pragma pack(push,1)
#endif

typedef struct PIActionParameters
	{
	
	PIDialogPlayOptions		playInfo;
	PIDialogRecordOptions	recordInfo;
	
	PIActionDescriptor		descriptor;
	
	} PIActionParameters;


#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

#if WIN32
#pragma pack(pop)
#endif

//-------------------------------------------------------------------------------

#endif // Rez

//-------------------------------------------------------------------------------

#endif // __PIActions_h__
