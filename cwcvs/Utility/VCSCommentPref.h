/*
	File:		VCSCommentPref.h

	Contains:	VCSComment Drop-In Preferences

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>     2/25/98    rmgw    Optional finder labels.
         <3>      2/5/98            Fix aete resource.
         <2>    11/13/97    rmgw    Fix menu choices
         <1>    10/29/97    rmgw   first checked in.
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
	cVCSComment					= 'cVCC',

	prefsModeProperty			= 'PRCM',	/* Comment mode			*/
	prefsInitialsProperty		= 'PRIN',	/* Initials				*/
	prefsCopyrightProperty		= 'PR©©',	/* Copyright			*/
	
	prefsUseLabels				= 'PRFL',	/* Finder Labels		*/
	prefsCheckedOutLabel		= 'PROL',	/* 	Checked out 		*/
	prefsCheckedInLabel			= 'PRIL',	/* 	Checked in	 		*/
	prefsMROLabel				= 'PRML',	/* 	MRO			 		*/
	prefsOrphanedLabel			= 'PRNL',	/* 	Orphaned	 		*/
	
	/* enumeration for project type */
	cUseComments				= 'Mode',
	enum_VCSComments_Never		= 'Nevr',	/* never				*/
	enum_VCSComments_Prompt		= 'Pmpt',	/* prompt				*/
	enum_VCSComments_Always		= 'Awys'	/* always				*/
};


/* internal codes for comment mode */
typedef enum eHeaderCommentsMode { 
	kHeaderCommentsNever,
	kHeaderCommentsPrompt,
	kHeaderCommentsAlways,
	
	kHeaderCommentsFnord
	} eHeaderCommentsMode;


/*	This is the structure that is manipulated by the panel.
 */

/* current version number for our prefs data */
#define kVCSCommentPrefVersion		2

typedef enum {
	kCheckedOutLabelIndex,
	kCheckedInLabelIndex,
	kMROLabelIndex,
	kOrphanedLabelIndex,
	kFinderLabelIndexCount
	} FinderLabelIndex;
	
typedef struct VCSCommentPref {
	short					version;			/* version # of prefs data	*/
	eHeaderCommentsMode		mode;		
	Str15					initials;
	Str255					copyright;
	Boolean					useFinderLabels;
	short					finderLabels[kFinderLabelIndexCount];
	} 	VCSCommentPref,
		 **VCSCommentPrefHandle;

#pragma options align=reset
