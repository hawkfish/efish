#include "FileEditText.h"
#include <UKeyFilters.h>
#include "MPString.h"
#include <LAction.h>
#include "PhotoPrintResources.h"
#include <UDebugging.h>

const ResIDT	str_RenameFileAction = 23;
const ResIDT	alrt_DuplicateFilename = 136;
const ResIDT	alrt_RenameFailure = 137;

class	RenameFileAction : public LAction
{
public:
				RenameFileAction(HORef<MFileSpec>& ioSpec, ConstStrFileNameParam inNewName);
				~RenameFileAction();

protected:
	HORef<MFileSpec>&	mSpec;
	MPString	oldName;
	MPString	newName;

	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
};


RenameFileAction::RenameFileAction(HORef<MFileSpec>& inSpec, ConstStrFileNameParam inNewName) 
	: LAction(str_Redo, str_RenameFileAction)
	, mSpec (inSpec)
	, oldName (inSpec->Name())
	, newName (inNewName)
{}

RenameFileAction::~RenameFileAction()
{}

void
RenameFileAction::RedoSelf() {
	mSpec->Rename(newName);
	}//end RedoSelf
	
void
RenameFileAction::UndoSelf() {
	mSpec->Rename(oldName);
	}//end UndoSelf



#pragma mark -

// ---------------------------------------------------------------------------
//	¥ LEditText								Stream Constructor		  [public]
// ---------------------------------------------------------------------------

FileEditText::FileEditText(
	LStream*	inStream,
	ClassIDT	inImpID)

	: LEditText(inStream, inImpID)
{
}//tiny ct


FileEditText::FileEditText(
	const SPaneInfo&	inPaneInfo,
	LCommander*			inSuperCommander,
	ConstStringPtr		inInitialText,
	ResIDT				inTextTraitsID,
	MessageT			inMessage,
	SInt16				inMaxChars,
	UInt8				inAttributes,
	TEKeyFilterFunc		inKeyFilter,
	bool				inPasswordField,
	ClassIDT			inImpID)
	
	:LEditText(inPaneInfo,
				inSuperCommander,
				inInitialText,
				inTextTraitsID,
				inMessage,
				inMaxChars,
				inAttributes,
				inKeyFilter,
				inPasswordField,
				inImpID)
{
}//big ct


FileEditText::~FileEditText()
{}//dt


void		
FileEditText::DontBeTarget() {
	LEditText::DontBeTarget();
	}//end DontBeTarget



Boolean
FileEditText::HandleKeyPress(const EventRecord&	inKeyEvent) {
	UInt16		theChar		 = (UInt16) (inKeyEvent.message & charCodeMask);
	
	if (!UKeyFilters::IsActionKey(theChar))
		return LEditText::HandleKeyPress(inKeyEvent);
	else {
		try {
			StDisableDebugThrow_() ;
			Str255 newName;
			GetDescriptor(newName);
			PostAction(new RenameFileAction(mItem->GetFileSpec(), newName));
			}//end try]
		catch (LException e) {
			if (e.GetErrorCode() == dupFNErr)
				::StopAlert(alrt_DuplicateFilename, NULL);
			else {
				MPString errorNumber (e.GetErrorCode());
				::ParamText(errorNumber, NULL, NULL, NULL);
				::StopAlert(alrt_RenameFailure, NULL);
				}//else
			}//catch
		}//else it's actionable, so try to action it

	return true;
	}//end HandleKeyPress
	
	
void
FileEditText::SetItem(PhotoItemRef inItem) {
	mItem = inItem;
	SetDescriptor(mItem->GetFileSpec()->Name());
	}//end
		

	
	
	
