#include "CWCVS.h"

#include "VCSAbout.h"
#include "VCSInitialize.h"

#include "VCSDatabaseConnect.h"
#include "VCSDatabaseVariables.h"

#include "VCSAdd.h"
#include "VCSCheckIn.h"
#include "VCSCheckOut.h"
#include "VCSComment.h"
#include "VCSDelete.h"
#include "VCSDifference.h"
#include "VCSGet.h"
#include "VCSHistory.h"
#include "VCSLabel.h"
#include "VCSProperties.h"
#include "VCSRename.h"
#include "VCSStatus.h"
#include "VCSUndoCheckout.h"
#include "VCSVersion.h"

#include "VCSPrompt.h"
#include "VCSV7Context.h"

#include "Kerberos.h"

#include <string.h>

//	=== Globals ===

KClientSessionInfo		ksession;

// ---------------------------------------------------------------------------
//		€ CWCVSDispatch
// ---------------------------------------------------------------------------

short 
CWCVSDispatch (

	VCSContext&		pb)
	
	{ // begin CWCVSDispatch
		
		/* Dispatch commands */
		switch (pb.GetRequest ()) {
			case reqAbout:
				pb.SetCommandStatus (VCSAbout (pb).OnRequest ());
				break;
				
			
			case reqInitialize:
				::KClientNewSession (&ksession, 0L, 0, 0L, 0);
				return VCSInitialize (pb);
				
			case reqTerminate:
				::KClientDisposeSession (&ksession);
				return cwCommandStatusSucceeded;
			

			case reqDatabaseConnect:
				pb.SetCommandStatus (VCSDatabaseConnect (pb).OnRequest ());
				break;
				
			case reqDatabaseDisconnect:
				pb.SetCommandStatus (VCSCommand (pb, true).OnRequest ());
				break;
				
			case reqDatabaseVariables:
				pb.SetCommandStatus (VCSDatabaseVariables (pb).OnRequest ());
				break;


			case reqFileAdd:
				pb.SetCommandStatus (VCSAdd (pb).OnRequest ());
				break;
				
			case reqFileCheckin:
				pb.SetCommandStatus (VCSCheckIn (pb).OnRequest ());
				break;
			
			case reqFileCheckout:
				pb.SetCommandStatus (VCSCheckOut (pb).OnRequest ());
				break;
				
			case reqFileComment:
				pb.SetCommandStatus (VCSComment (pb).OnRequest ());
				break;

			case reqFileDelete:
				pb.SetCommandStatus (VCSDelete (pb).OnRequest ());
				break;
			
			case reqFileDifference:
				pb.SetCommandStatus (VCSDifference (pb).OnRequest ());
				break;

			case reqFileGet:
				pb.SetCommandStatus (VCSGet (pb).OnRequest ());
				break;
			
			case reqFileHistory:
				pb.SetCommandStatus (VCSHistory (pb).OnRequest ());
				break;
			
			case reqFileLabel:
				pb.SetCommandStatus (VCSLabel (pb).OnRequest ());
				break;

			case reqFileProperties:
				pb.SetCommandStatus (VCSProperties (pb).OnRequest ());
				break;

			case reqFileRename:
				pb.SetCommandStatus (VCSRename (pb).OnRequest ());
				break;
			
			case reqFileStatus:
				pb.SetCommandStatus (VCSStatus (pb).OnRequest ());
				break;
			
			case reqFileUndoCheckout:
				pb.SetCommandStatus (VCSUndoCheckout (pb).OnRequest ());
				break;
			
			case reqFileVersion:
				pb.SetCommandStatus (VCSVersion (pb).OnRequest ());
				break;
			
			case reqFileDestroy:
			case reqFilePurge:
			case reqFileRollback:
			
			case reqFileBranch:
			case reqFileShare:
			case reqFileView:

			default:
				pb.SetCommandStatus (VCSCommand (pb).OnRequest ());
				break;
			} // switch
		
		//	Prepare for next call
		ClearPromptResponses ();
		
		/* return result code */
		return pb.GetCommandStatus ();
	
	} // end CWCVSDispatch

