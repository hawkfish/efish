#include "VCSInitialize.h"

#include "VCSDialog.h"
#include "VCSDialogControlItem.h"
#include "VCSDialogTextItem.h"
#include "VCSUtil.h"

#include "SerialNumber.h"
#include "StCurResFile.h"
#include "CInternetConfig.h"

#include <Folders.h>
#include <Resources.h>
#include <Sound.h>
#include <TextUtils.h>

//	=== Constants ===

enum RegistrationStrings {
	kRegistrationStringsIllegalIndex,
	
	kPrefsFileNameIndex,
	kSecsIndex,
	
	kRegistrationStrings = 16005,
	
	kFnordRegistration
	};

//	=== Constants ===

const	unsigned	char	
kXorMask = 'F';

class VCSRegistrationDialog : public VCSDialog

	{
		
	protected:
		
		CInternetConfig			mIC;
		VCSDialogControlItem	mOKButton;
		VCSDialogControlItem	mCancelButton;
		VCSDialogControlItem	mNotYetButton;
		VCSDialogTextItem		mTextItem;
		VCSDialogTextItem		mTimeoutItem;
		VCSDialogTextItem		mURLItem;
		
		short					mRefNum;
		UInt32					mStartTicks;
		
		static	UInt32			sNotYetTicks;
		
		//	Event handling
		virtual	Boolean OnIdle					(EventRecord&			theEvent,
												 DialogItemIndex&		itemHit);

		//	Modal dialog handling
		virtual	Boolean OnItemHit 				(DialogItemIndex		inItemHit);

	public:
	
		enum RegistrationItems {
			kNotYetItem = cancel + 1,
			kRegistrationPromptItem,
			kRegistrationTextItem,
			kRegistrationIconItem,
			kRegistrationTimeoutItem,
			kRegistrationURLItem,
			
			kResourceID = 16005,

			kRegistrationFnordItem
			};
	
	
						VCSRegistrationDialog	(const	VCSContext&		inContext,
												 short					inDLOGid,
												 short	 				inRefNum);


		static	CWVCSCommandStatus 	DoDialog	(const	VCSContext&		inContext,
												 short					inRefNum);
	};
	
//	===	Static Members ===

UInt32
VCSRegistrationDialog::sNotYetTicks = 10 * 60;

// ---------------------------------------------------------------------------
//		¥ VCSRegistrationDialog
// ---------------------------------------------------------------------------

VCSRegistrationDialog::VCSRegistrationDialog (

	const	VCSContext&		inContext,
	short					inDLOGid,
	short	 				inRefNum)
	
	: VCSDialog (inContext, inDLOGid)
	
	, mIC ('CWIE')
	
	, mOKButton (*this, ok)
	, mCancelButton (*this, cancel)
	, mNotYetButton (*this, kNotYetItem)
	, mTextItem (*this, kRegistrationTextItem)
	, mTimeoutItem (*this, kRegistrationTimeoutItem)
	, mURLItem (*this, kRegistrationURLItem)

	, mRefNum (inRefNum)
	, mStartTicks (::TickCount ())
	
	{ // begin VCSRegistrationDialog
		
		InitCursor ();
		
		mIC.Start (0);
		
	} // end VCSRegistrationDialog
	
// ---------------------------------------------------------------------------
//		¥ OnIdle
// ---------------------------------------------------------------------------

Boolean 
VCSRegistrationDialog::OnIdle (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnIdle
		
		const	long	soFarTicks = ::TickCount () - mStartTicks;
		
		mNotYetButton.SetEnable (soFarTicks >= sNotYetTicks);
		if (soFarTicks < sNotYetTicks) {
			mTimeoutItem.SetValue ((sNotYetTicks - soFarTicks + 59) / 60);	
			} // if
		
		else mTimeoutItem.Hide ();
		
		return VCSDialog::OnIdle (theEvent, itemHit);
	
	} // end OnIdle

// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
VCSRegistrationDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Str255		serial;

		switch (inItemHit) {
			case ok:
				GetItemText (kRegistrationTextItem, serial);
				if (!TestSerial (serial)) {
					SysBeep (1);
					break;
					} // if
					
				if (mRefNum != -1) {
					Handle	h = nil;
					XorSerial (serial, kXorMask);
					if (noErr != PtrToHand (serial, &h, serial[0] + 1)) break;
					
					AddResource (h, 'STR ', 128, nil);
					WriteResource (h);
					} // if
				//	Fall throughÉ

			case cancel:
			case kNotYetItem:
				return false;
				
			case kRegistrationURLItem:
				mURLItem.GetDescriptor (serial);
				mIC.DoURL (serial);
				break;
			} // switch
		
		return VCSDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ DoDialog
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSRegistrationDialog::DoDialog (
	
	const	VCSContext&		inContext,
	short					inRefNum)

	{ // begin DoDialog
		
		VCSRegistrationDialog d (inContext, kResourceID, inRefNum);
		
		if (d.GetDialogPtr ()) {
			switch (d.DoModalDialog ()) {
				case ok:
				case kNotYetItem:
					return cwCommandStatusSucceeded;
				} // switch
			} // if
		
		return cwCommandStatusFailed;
		
	} // end DoDialog

// ---------------------------------------------------------------------------
//		¥ VCSInitialize
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSInitialize (
	
	const VCSContext&	inContext)

	{ // begin VCSInitialize
		
		CWVCSCommandStatus	result = cwCommandStatusSucceeded;
		FSSpec				prefsSpec;
		
		//	stuff to clean up
		StCurResFile		saveResFile;
		short				refNum = -1;
		
		//	Prepare

		//	Open the prefs file
		if (noErr == FindFolder (kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &prefsSpec.vRefNum, &prefsSpec.parID)) {
			FSSpec	test;
			::GetIndString (prefsSpec.name, kRegistrationStrings, kPrefsFileNameIndex);
			if (noErr != FSMakeFSSpec (prefsSpec.vRefNum, prefsSpec.parID, prefsSpec.name, &test)) 
				::FSpCreateResFile (&prefsSpec, 'RSED', 'rsrc', smSystemScript);
			
			refNum = ::FSpOpenResFile (&prefsSpec, fsRdWrPerm);
			} // if
		
		//	Read the serial number from the prefs
		if (refNum != -1) {
			StringHandle	h = nil;
			UseResFile (refNum);
			h = (StringHandle) Get1Resource ('STR ', 128);
			if (h && *h) {
				//	Check prefs serial
				Str255		serial;
				BlockMoveData (*h, serial, **h + 1);
				XorSerial (serial, kXorMask);
				if (TestSerial (serial)) goto CleanUp;
				
				RemoveResource ((Handle) h);
				DisposeHandle ((Handle) h);
				h = nil;
				} // if
			} // if
		
		//	Bug them about registration if its missing/invalid
		result = VCSRegistrationDialog::DoDialog (inContext, refNum);
		
	CleanUp:
		
		if (-1 != refNum) ::CloseResFile (refNum);
		refNum = -1;
		
		return result;
		
	} // end VCSInitialize
