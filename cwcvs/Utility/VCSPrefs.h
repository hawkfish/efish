// ===========================================================================
//	VCSPrefs.h ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

#include "EnvPanel.h"
#include "OptionsPanel.h"
#include "VCSCommentPref.h"

#include <AEDataModel.h>
#include <Resources.h>

void 
VCSGetUserName (
	
	const VCSContext&		inPB,
	Str63					userName);

Boolean 
VCSPrefsGetEnv (
	
	const VCSContext&		inPB,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault);

OSErr 
VCSPrefsMakeEnvDescList (
	
	const VCSContext&		inPB,
	AEDescList*				outList);

class VCSOptionsPrefs {
		
		
		const VCSContext&	mContext;
		CWMemHandle			mHandle;
		OptionsRec*			mOptions;
		
	public:
		
							VCSOptionsPrefs		(const VCSContext&	inPB);
		virtual				~VCSOptionsPrefs	(void);
		
		const	OptionsRec*	operator->			(void) const
												{return mOptions;};
	};
	
UInt32 
VCSGetDefaultOptions 	(const VCSContext&			inPB,
						 OptionsRec::OptionsIndex	inIndex);
						 
OSErr 
VCSGetDefaultOptionsList(const VCSContext&			inPB,
						 ResID						inStrnID,
						 ResID						inMapID,
						 AEDescList&				outList);
						 
UInt16 
VCSGetTextKeywordDefault (
	
	const VCSContext&			inPB);

UInt16 
VCSGetBinaryKeywordDefault (
	
	const VCSContext&			inPB);

UInt16 
VCSGetHistoryInfoDefault (
	
	const VCSContext&			inPB);

OSType 
VCSGetClientCreator (
	
	const VCSContext&			inPB);



Boolean 
VCSGetFinderLabel 		(const VCSContext&			inPB,
						 FinderLabelIndex			inIndex,
						 short*						outLabel);
