#include "VCSAbout.h"

#include "VCSDialog.h"

#include "CInternetConfig.h"
#include "StCurResFile.h"

#include <Dialogs.h>
#include <Resources.h>
#include <TextUtils.h>

class VCSAboutDialog : public VCSDialog

	{
		
	protected:
		
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
	
	
								VCSAboutDialog	(const	VCSContext&		inContext,
												 short					inDLOGid);

		static	void	 		DoDialog		(const	VCSContext&		inContext);
	};
	
// ---------------------------------------------------------------------------
//		€ VCSAboutDialog
// ---------------------------------------------------------------------------

VCSAboutDialog::VCSAboutDialog (

	const	VCSContext&		inContext,
	short					inDLOGid)
	
	: VCSDialog (inContext, inDLOGid)
	
	, mIC ('CWIE')
	, mICLoaded (mIC.Start (0) == noErr)

	{ // begin VCSAboutDialog
		
		InitCursor ();
		
	} // end VCSAboutDialog
	
// ---------------------------------------------------------------------------
//		€ OnItemHit
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
//		€ DoDialog
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
//		€ VCSAbout
// ---------------------------------------------------------------------------

VCSAbout::VCSAbout (

	VCSContext&	inContext)
	
	: VCSCommand (inContext, true)
	
	{ // begin VCSAbout
		
	} // end VCSAbout

// ---------------------------------------------------------------------------
//		€ ~VCSAbout
// ---------------------------------------------------------------------------

VCSAbout::~VCSAbout (void)
	
	{ // begin ~VCSAbout
				
	} // end ~VCSAbout

// ---------------------------------------------------------------------------
//		€ DoRequest
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
