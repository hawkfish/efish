#include "VCSAbout.h"

#include "VCSDialog.h"

#include "CInternetConfig.h"
#include "StCurResFile.h"

#include <Controls.h>
#include <Dialogs.h>
#include <Resources.h>
#include <TextUtils.h>

class VCSAboutDialog : public VCSDialog

	{
		
	protected:
		
		ControlColorUPP			mColorUPP;
		
		CInternetConfig			mIC;
		Boolean					mICLoaded;
				
		//	Modal dialog handling
		virtual	Boolean 		OnItemHit 		(DialogItemIndex		inItemHit);

	public:
	
		enum DialogItems {
			kIllegalItem,
			kAboutURLItem,
			kAboutBackgroundItem,
			kAboutVersionItem,
			kAboutCopyrightItem,
			
			kResourceID = 16006
			};
	
		enum Strings {
			kDownloadIndex = 1
			};
	
		static	pascal OSStatus	WhiteOnBlack	(ControlRef				comtrol,
												 SInt16					message,
												 SInt16					depth,
												 Boolean				inColor);

	
								VCSAboutDialog	(const	VCSContext&		inContext,
												 short					inDLOGid);
		virtual					~VCSAboutDialog	(void);

		static	void	 		DoDialog		(const	VCSContext&		inContext);
	};
	
// ---------------------------------------------------------------------------
//		¥ WhiteOnBlack
// ---------------------------------------------------------------------------

pascal OSStatus
VCSAboutDialog::WhiteOnBlack (

	ControlRef				/*control*/,
	SInt16					message,
	SInt16					/*depth*/,
	Boolean					/*inColor*/)
	
	{ // begin WhiteOnBlack
		
		RGBColor	rgb;
		
		switch (message) {
			case kControlMsgSetUpBackground:
				rgb.red = rgb.green = rgb.blue = 0;					//	Black
				::RGBBackColor (&rgb);
				break;
				
			case kControlMsgApplyTextColor:
				rgb.red = rgb.green = rgb.blue = 0xFFFF;			//	White
				::RGBForeColor (&rgb);
				break;
			
			default:
				return paramErr;
			} // switch
			
		return noErr;
		
	} // end WhiteOnBlack
	
// ---------------------------------------------------------------------------
//		¥ VCSAboutDialog
// ---------------------------------------------------------------------------

VCSAboutDialog::VCSAboutDialog (

	const	VCSContext&		inContext,
	short					inDLOGid)
	
	: VCSDialog (inContext, inDLOGid)
	
	, mColorUPP (NewControlColorUPP (WhiteOnBlack))
	
	, mIC ('CWIE')
	, mICLoaded (mIC.Start (0) == noErr)

	{ // begin VCSAboutDialog
		
		InitCursor ();
		
		ControlRef	c;
		
		if (noErr == GetDialogItemAsControl (GetDialogPtr (), kAboutVersionItem, &c))
			SetControlColorProc (c, mColorUPP);
		
		if (noErr == GetDialogItemAsControl (GetDialogPtr (), kAboutCopyrightItem, &c))
			SetControlColorProc (c, mColorUPP);
		
	} // end VCSAboutDialog
	
// ---------------------------------------------------------------------------
//		¥ ~VCSAboutDialog
// ---------------------------------------------------------------------------

VCSAboutDialog::~VCSAboutDialog (void)
	
	{ // begin ~VCSAboutDialog
		
		if (mColorUPP) DisposeControlColorUPP (mColorUPP);
		mColorUPP = nil;
		
	} // end ~VCSAboutDialog
	
// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
VCSAboutDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		switch (inItemHit) {
			case kAboutURLItem:
				if (!mICLoaded) break;
				
				Str255	url;
				::GetIndString (url, kResourceID, kDownloadIndex);
				mIC.DoURL (url);
				//return VCSDialog::OnItemHit (inItemHit);
			} // switch
		
		return false;
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ DoDialog
// ---------------------------------------------------------------------------

void 
VCSAboutDialog::DoDialog (
	
	const	VCSContext&		inContext)

	{ // begin DoDialog
		
		VCSAboutDialog d (inContext, kResourceID);
		d.DoModalDialog ();
		
	} // end DoDialog

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ VCSAbout
// ---------------------------------------------------------------------------

VCSAbout::VCSAbout (

	VCSContext&	inContext)
	
	: VCSCommand (inContext, true, true)
	
	{ // begin VCSAbout
		
	} // end VCSAbout

// ---------------------------------------------------------------------------
//		¥ ~VCSAbout
// ---------------------------------------------------------------------------

VCSAbout::~VCSAbout (void)
	
	{ // begin ~VCSAbout
				
	} // end ~VCSAbout

// ---------------------------------------------------------------------------
//		¥ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSAbout::DoRequest (void)
	
	{ // begin DoRequest
					
		Str255			version = {0};
		
		Handle			rHandle;
		StCurResFile	saveResFile;
		
		//	Find our vers resource
		rHandle = ::GetResource ('vers', 1);
		if (rHandle != nil) {
			VersRecHndl	vers = (VersRecHndl) rHandle;
			::BlockMoveData ((**vers).shortVersion, version, (**vers).shortVersion[0] + 1);
			} // if
		
		VCSDialog::SetParamText (version);
		VCSAboutDialog::DoDialog (mContext);
		
		return cwCommandStatusSucceeded;
	
	} // end DoRequest
