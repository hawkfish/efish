/*
	File:		OptionsPanel.h

	Contains:	Interface to the CWProjector Options prefs panel.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     11/3/98    rmgw    Export map structure.
        <1+>     11/3/98    rmgw    Implement.
         <1>     11/3/98    rmgw    first checked in.
*/


#pragma once

#ifndef __TYPES__
#	include <Types.h>
#endif


#pragma options align=mac68k


/*
 *	AppleScript dictionary info.  
 */

enum {
/*	Symbolic Name				   Code		AETE Terminology		*/
	cCWProjectorOptions			= 'CVSo',
	
		pAddTextKeyword			= 'CVAt',
		pAddBinaryKeyword		= 'CVAb',
		pHistoryFileInfo		= 'CVHf',
		pClientCreator			= 'CVSc',
		
	kKeywordsNone				= 'CVk ',
	kKeywordsDefault			= 'CVkk',
	kKeywordsLocker				= 'CVkl',
	kKeywordsNames				= 'CVkn',
	kKeywordsOld				= 'CVko',
	kKeywordsBinary				= 'CVkb',
	kKeywordsValues				= 'CVkv',

	kFileInfoFull				= 'CVff',
	kFileInfoShort				= 'CVft',
	kFileInfoMinimal			= 'CVfh',
	kFileInfoRSCName			= 'CVfR'
};


/*	This is the structure that is manipulated by the panel.
 */

/* current version number for our prefs data */
#define kOptionsPrefVersion		2

typedef struct OptionsRec {
	enum OptionsIndex {
		kGetOptions,
		kCheckinOptions,
		kDiffGetOptions,
		kStatusOptions,
		kLabelOptions,
		kRemoteLabelOptions,
		kHistoryOptions,
		kOptionsCount
		};
	
	UInt16		version;			/* version # of prefs data	*/
	UInt32		options[kOptionsCount];
	UInt16		textKeyword;
	UInt16		binaryKeyword;
	UInt16		historyInfo;
	OSType		clientCreator;
	} 	OptionsRec,
		*OptionsRecPtr,
		**OptionsRecHandle;

typedef struct OptionBitPair {
	UInt32	key;
	UInt16	index;
	UInt16	shift;
	}	OptionBitPair;

typedef	struct OptionBitMapRec {
	UInt16			count;
	OptionBitPair	bit[];
	}	OptionBitMapRec;

#pragma options align=reset
