/*
	File:		TransparentGrafPortView.cp

	Contains:	An LGrafPortView which doesn't erase its background

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

	07 Aug 01	drd		Created
*/

#include "TransparentGrafPortView.h"
#include <UAppearance.h>

/*
TransparentGrafPortView
	Stream Constructor
*/
TransparentGrafPortView::TransparentGrafPortView(
	LStream*	inStream)
	: LGrafPortView(inStream)
	, fBackgroundBrush(kThemeBrushWhite)
{
}

TransparentGrafPortView::~TransparentGrafPortView()
{
} // ~TransparentGrafPortView

/*
DrawSelf {OVERRIDE}
*/
void
TransparentGrafPortView::DrawSelf()
{
	// Don't draw. This allows the theme background to show through.
} // DrawSelf

/*
UpdatePort {OVERRIDE}
	Redraw invalidated area of the GrafPortView. We make sure that we will erase
	in the correct theme.
*/
void
TransparentGrafPortView::UpdatePort()
{
	SInt16	bitDepth;
	bool	hasColor;
	this->GetDeviceInfo(bitDepth, hasColor);

//	UAppearance::SetThemeBackground(fBackgroundBrush, bitDepth, hasColor);
	::SetThemeBackground(fBackgroundBrush, bitDepth, hasColor);

	inherited::UpdatePort();
} // UpdatePort
