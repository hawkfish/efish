//PhotoPrintView.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#include "PhotoPrintView.h"
#include "PhotoPrintModel.h"

const double kRad2Degrees = 57.2958;
const PaneIDT pane_Debug1 = 'dbg1';
const PaneIDT pane_Debug2 = 'dbg2';


//-----------------------------------------------
// PhotoPrintView empty constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView()
	:LPane ()
{
	
	}
	
	
//-----------------------------------------------
// PhotoPrintView copy constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const PhotoPrintView		&inOriginal)
	:LPane(inOriginal)
	{
	}
		
//-----------------------------------------------
// PhotoPrintView SPaneInfo constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	const SPaneInfo	&inPaneInfo)
	:LPane(inPaneInfo)
{
}
	
//-----------------------------------------------
// PhotoPrintView Stream constructor
//-----------------------------------------------
PhotoPrintView::PhotoPrintView(	LStream			*inStream)
	:LPane (inStream)
{
	mController = new PhotoPrintController();
	mModel = new PhotoPrintModel(this, mController);
	mController->SetModel(mModel);
}


//-----------------------------------------------
// ~PhotoPrintView 
//-----------------------------------------------
PhotoPrintView::~PhotoPrintView()
{}//end dt



//-----------------------------------------------
// FinishCreateSelf.  Find all those handy panes which ought to be setup by now
//-----------------------------------------------
void
PhotoPrintView::FinishCreateSelf() {
	mDebug1 = GetSuperView()->FindPaneByID(pane_Debug1);
	mDebug2 = GetSuperView()->FindPaneByID(pane_Debug2);
	}//FinishCreateSelf



//-----------------------------------------------
// DrawSelf  if there is a selection, then select it
//-----------------------------------------------
void
PhotoPrintView::DrawSelf() {
	GrafPtr	curPort;
	GDHandle curDevice;
	::GetPort(&curPort);
	curDevice = ::GetGDevice();

	if (mModel)
		mModel->Draw(0,
					(CGrafPtr)curPort,
					curDevice);
		
	if (mController && mModel)
		mController->Select(mModel->GetSelection());
}//end DrawSelf


//-----------------------------------------------
// ClickSelf.  Handle 'dat event
//-----------------------------------------------
void		
PhotoPrintView::ClickSelf(const SMouseDownEvent &inMouseDown) {
	FocusDraw ();

	MRect rFrame;
	CalcPortFrameRect(rFrame);

	mController->HandleClick(inMouseDown, rFrame);
}//end ClickSelf
