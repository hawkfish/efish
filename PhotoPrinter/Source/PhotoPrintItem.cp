/*
	File:		PhotoPrintItem.cp

	Contains:	Definition of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	03 Jul 2000		drd		MakeProxy doesn't clip as much
	03 Jul 2000		drd		SetFile sends DeleteProxy; added gUseProxies, DrawImage; redo MakeProxy
	30 Jun 2000		drd		SetFile copies QTI (very handy for SchoolLayout)
	29 jun 2000		dml		add proxy support
	29 jun 2000		dml		clean up ownership issues in ResolveCropStuff
	27 jun 2000		dml		added SetScreenBounds, removed CROP_BY_REGION ifdefs, fixed DrawEmpty
	27 Jun 2000		drd		IsLandscape, IsPortrait
	27 jun 2000		dml		doh!  pass reference to cropRgn to ResolveRegionStuff (cropping works again)
	26 Jun 2000		drd		SetFile; improved default constructor
	21 june 2000 	dml		initial crop should be EmptyRect, and we should special case it in Draw()
	20 june 2000	dml		work on cropping.  
	19 june 2000	dml		copy ct copies crop
	19 june 2000	dml		added cropping, alphabetized
*/

#include "PhotoPrintItem.h"
#include <algorithm.h>
#include "xmlinput.h"
#include "xmloutput.h"
#include "PhotoUtility.h"
#include "MOpenPicture.h"
#include "MNewRegion.h"

// Globals
SInt16	PhotoPrintItem::gProxyBitDepth = 16;
bool	PhotoPrintItem::gUseProxies = true;				// For debug purposes

// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(const MFileSpec* inSpec) {
	ComponentResult		res;
	
	res = ::GetGraphicsImporterForFile((const FSSpec*)inSpec, &mGI);
	ThrowIfNil_(mGI);
}//end ct


// ---------------------------------------------------------------------------
// ~StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::~StQTImportComponent() {
	::CloseComponent(mGI);
}//end dt


#pragma mark -

// ---------------------------------------------------------------------------
// PhotoPrintItem constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(const MFileSpec& inSpec)
	: mSpec (new MFileSpec(inSpec))
	, mRot (0.0)
	, mSkew (0.0)
	, mQTI (new StQTImportComponent(&inSpec))
{
	ComponentResult res;
	res = ::GraphicsImportGetNaturalBounds (*mQTI, &mNaturalBounds);
	ThrowIfOSErr_(res);			

	::SetIdentityMatrix(&mMat);
}//end ct
	


// ---------------------------------------------------------------------------
// PhotoPrintItem copy constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(PhotoPrintItem& other) 
	: mSpec (other.mSpec)
	, mNaturalBounds (other.GetNaturalBounds())
	, mCrop (other.GetCrop())
	, mDest (other.GetDestRect())
	, mRot (other.GetRotation())
	, mSkew (other.GetSkew())
	, mQTI (other.mQTI)
	, mProperties (other.GetProperties())
{
	// could recompute, but hey, it's a copy constructor
	::CopyMatrix(&(other.mMat), &mMat);	
}//end copy ct


// ---------------------------------------------------------------------------
// PhotoPrintItem empty constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem()
	: mRot (0.0)
	, mSkew (0.0)
	, mQTI(nil)
{
}//end empty ct



// ---------------------------------------------------------------------------
// ~PhotoPrintItem destructor
// ---------------------------------------------------------------------------
PhotoPrintItem::~PhotoPrintItem() {
	}//end dt

// ---------------------------------------------------------------------------
// SetFile
//	This method copies everything needed to make the two items refer to the
//	same file.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::SetFile(const PhotoPrintItem& inOther)
{
	mSpec = inOther.mSpec;
	mQTI = inOther.mQTI;						// This has already imported the file
	mNaturalBounds = inOther.mNaturalBounds;	// Likewise, the bounds will be the same

	this->DeleteProxy();						// Picture has changed, get rid of cached low-res
} // SetFile

#pragma mark -

// ---------------------------------------------------------------------------
// PhotoPrintItem::Draw
//			set the qt matrix + have the component render
// ---------------------------------------------------------------------------
void
PhotoPrintItem::Draw(
	const PhotoDrawingProperties&	props,
	MatrixRecord*					worldSpace,
	CGrafPtr						inDestPort,
	GDHandle						inDestDevice,
	RgnHandle						inClip)
{
	MatrixRecord	localSpace;
	SetupDestMatrix(&mMat);
	
	::CopyMatrix(&mMat, &localSpace);
	if (worldSpace)
		::ConcatMatrix(worldSpace, &localSpace);
	
	HORef<MRegion>	cropRgn;
	RgnHandle		workingCrop(this->ResolveCropStuff(cropRgn, inClip));

	do {
		if (this->IsEmpty()) {
			if (!props.GetPrinting()) {
				this->DrawEmpty(props, &localSpace, inDestPort, inDestDevice, workingCrop);
			} //endif we're not printing
			break;
		} //endif empty

		if (this->CanUseProxy(props)) {
			// if there is no proxy, make one!
			if (mProxy == nil)
				this->MakeProxy(&localSpace);
			
			if (mProxy != nil) {
				this->DrawProxy(props, &localSpace, inDestPort, inDestDevice, workingCrop);
				break;
			}
		}
		{ //otherwise we can draw normally
			this->DrawImage(&localSpace, inDestPort, inDestDevice, inClip);
		} //end normal drawing block
	} while (false);
}//end Draw

void
PhotoPrintItem::DrawEmpty(const PhotoDrawingProperties& /*props*/,
						 MatrixRecord* localSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip) {

	MRect	bounds (GetDestRect());
	if (!bounds) // if no bounds, use the maxBounds
		bounds = GetMaxBounds();

	enum cornerType {
		kTopLeft = 0,
		kTopRight,
		kBotRight,
		kBotLeft,
		kFnordCorner};
		
	Point	corners[kFnordCorner];
	corners[kTopLeft] = bounds.TopLeft();
	corners[kBotRight] = bounds.BotRight();
	corners[kTopRight].h = corners[kBotRight].h;
	corners[kTopRight].v = corners[kTopLeft].v;	corners[kBotLeft].h = corners[kTopLeft].h;
	corners[kBotLeft].v = corners[kBotRight].v;
	
	// transform those corners by composite matrix
	::TransformPoints(localSpace, corners, kFnordCorner);

	HORef<StGDeviceSaver> saveDevice;
	CGrafPtr	destPort;
	if (inDestPort != nil)
		destPort = inDestPort;
	else
		::GetPort((GrafPtr*)&destPort);
		
	StColorPortState	saveState ((GrafPtr)destPort);
	if (inDestDevice != nil) {
		saveDevice = new StGDeviceSaver;
		::SetGWorld(destPort, inDestDevice);
		}//endif device specified 

	saveState.Normalize();
	::RGBForeColor(&PhotoUtility::sNonReproBlue);
	
	HORef<StClipRgnState>	saveClip;
	if (inClip != nil) {
		saveClip = new StClipRgnState (inClip);
		}//endif clipping to do

	::MoveTo(corners[kTopLeft].h, corners[kTopLeft].v);
	::LineTo(corners[kTopRight].h, corners[kTopRight].v);
	::LineTo(corners[kBotRight].h, corners[kBotRight].v);
	::LineTo(corners[kBotLeft].h, corners[kBotLeft].v);
	::LineTo(corners[kTopLeft].h, corners[kTopLeft].v);
	::LineTo(corners[kBotRight].h, corners[kBotRight].v);
	::MoveTo(corners[kBotLeft].h, corners[kBotLeft].v);
	::LineTo(corners[kTopRight].h, corners[kTopRight].v);
	
	}//end DrawEmpty

// ---------------------------------------------------------------------------
// DrawImage
//	Does the basic drawing. Called by Draw and MakeProxy.
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawImage(
	 MatrixRecord*	inLocalSpace, // already composited and ready to use
	 CGrafPtr		inDestPort,
	 GDHandle		inDestDevice,
	 RgnHandle		inClip) const
{
	OSErr			e;

	ThrowIfOSErr_(::GraphicsImportSetMatrix (*mQTI, inLocalSpace));

	if (inDestPort && inDestDevice) {
		e =::GraphicsImportSetGWorld(*mQTI, inDestPort, inDestDevice);
		ThrowIfOSErr_(e);
	}//endif
		
	e = ::GraphicsImportSetClip(*mQTI, inClip);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportSetQuality (*mQTI, codecMaxQuality);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportDraw(*mQTI);
	ThrowIfOSErr_(e);
} // DrawImage

// ---------------------------------------------------------------------------
// DrawProxy
// ---------------------------------------------------------------------------
void
PhotoPrintItem::DrawProxy(const PhotoDrawingProperties& /*props*/,
						 MatrixRecord* /*localSpace*/, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip)
{
	StClipRgnState saveClip (inClip);
	HORef<StGDeviceSaver> saveDevice;
	CGrafPtr	destPort;
	if (inDestPort != nil)
		destPort = inDestPort;
	else
		::GetPort((GrafPtr*)&destPort);
		
	StColorPortState	saveState ((GrafPtr)destPort);
	if (inDestDevice != nil) {
		saveDevice = new StGDeviceSaver;
		::SetGWorld(destPort, inDestDevice);
		}//endif device specified 
	
	::DrawPicture(mProxy, &mDest);						 						 
}//end DrawProxy				


// ---------------------------------------------------------------------------
// GetMatrix 
// (might be stale but faster if don't force recompute)
// ---------------------------------------------------------------------------
void
PhotoPrintItem::GetMatrix(MatrixRecord* pDestMatrix,
							bool inForceRecompute) {
	if (inForceRecompute)
		SetupDestMatrix(&mMat);

	::CopyMatrix(&mMat, pDestMatrix);
	}//end GetMatrix	
	

ConstStr255Param
PhotoPrintItem::GetName() {
	Assert_(mSpec);
	return mSpec->Name();
	}//end GetName



// ---------------------------------------------------------------------------
// GetTransformedBounds 
// 
// ---------------------------------------------------------------------------
MRect
PhotoPrintItem::GetTransformedBounds() {
	Point	corners[4];
	corners[0] = mDest.TopLeft();
	corners[3] = mDest.BotRight();
	corners[1].v = corners[0].v; // topRight
	corners[1].h = corners[3].h;
	corners[2].v = corners[3].v; // bottomLeft
	corners[2].h = corners[0].h;
	
	SetupDestMatrix(&mMat);
	::TransformPoints(&mMat, corners, 4);
	
	MRect bounds;
	bounds.left = min(min(corners[0].h, corners[1].h), min(corners[2].h, corners[3].h));
	bounds.right = max(max(corners[0].h, corners[1].h), max(corners[2].h, corners[3].h));
	bounds.top = min(min(corners[0].v, corners[1].v), min(corners[2].v, corners[3].v));
	bounds.bottom = max(max(corners[0].v, corners[1].v), max(corners[2].v, corners[3].v));

	return bounds;
	}//end	 

// ---------------------------------------------------------------------------
// IsLandscape
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::IsLandscape() const
{
	MRect		test;
	if (mCrop.IsEmpty())
		test = mDest;
	else
		test = mCrop;

	return test.Width() >= test.Height();
} // IsLandscape

// ---------------------------------------------------------------------------
// IsPortrait
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::IsPortrait() const
{
	MRect		test;
	if (mCrop.IsEmpty())
		test = mDest;
	else
		test = mCrop;

	return test.Height() > test.Width();
} // IsPortrait

// ---------------------------------------------------------------------------
// MapDestRect.  Used to map mDest from one rect to another
// so that rotation/skewing won't be affected by the transoform 
// otherwise we could accomplish this with a matrix operation
//
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MapDestRect(const MRect& sourceRect, const MRect& destRect)
{
	::MapRect(&mDest, &sourceRect, &destRect);
	::MapRect(&mCrop, &sourceRect, &destRect);
// don't map since now using crop on source bounds	::MapRect(&mCrop, &sourceRect, &destRect);
}//end MapDestRect


// ---------------------------------------------------------------------------
// ResolveCropStuff
// ---------------------------------------------------------------------------
RgnHandle
PhotoPrintItem::ResolveCropStuff(HORef<MRegion>& cropRgn, RgnHandle inClip)
{
	RgnHandle	rh;
	
	// do we have intrinsic cropping?
	if (mCrop) {
		cropRgn = new MNewRegion;
		rh = *cropRgn;
		*cropRgn = mCrop;
		
		// fake out clip bug
		// by creating a tiny region on the topline, union'ed with the rest
		MNewRegion fakey;
		fakey = MRect (mDest.top, mDest.left, mDest.top + 1, mDest.left + 1);

		cropRgn->Union(*cropRgn, fakey);
		}//endif we have some intrinsic cropping

	// combine it with any incoming clipping
	if (inClip != nil) {
		if (cropRgn) 
			cropRgn->Intersect(*cropRgn, inClip);
		else {
			cropRgn = new MNewRegion ();
			cropRgn->SetRegion(inClip);
			}//else make a copy of the incoming clip region 

		// QTI for SGI crashes if clip outside of dest
		// crop to destRect (which by definition is within NaturalBounds rect)
		MNewRegion destRgn;
		destRgn = mDest;
		cropRgn->Intersect(*cropRgn, destRgn);
		}//endif there is some incoming clipping

	// we hold an HORef<MRegion>
	// bool(HORef<>) may be false if nothing is stored
	// and bool(*HORef) may be false if a nil crop region is present
	// here's the two step process in figuring that out.
	RgnHandle workingCrop ((RgnHandle)nil);
	if (cropRgn) 
		workingCrop =  *cropRgn ? *cropRgn : (RgnHandle)nil;

	return workingCrop;
} // ResolveCropStuff


// ---------------------------------------------------------------------------
// SetCrop
//
//		new crop bounds.  If new crop outside of destBounds, remove crop !!!
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetCrop(const MRect& inCrop) {
	mCrop = inCrop;
} // SetCrop


// ---------------------------------------------------------------------------
// SetDest
//
// ---------------------------------------------------------------------------
void 			
PhotoPrintItem::SetDest(const MRect& inDest) {
	mDest = inDest;
}//end SetDest


// ---------------------------------------------------------------------------
// SetScreenDest
//
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::SetScreenDest(const MRect& inDest) {
	MatrixRecord	inverse;
	
	SetupDestMatrix(&mMat);
	Boolean happy (::InverseMatrix (&mMat, 
                                    &inverse));
    if (happy) {
		Point	corners[2];
		corners[0] = inDest.TopLeft();
		corners[1] = inDest.BotRight();
		::TransformPoints(&inverse, corners, 2);
		
		MRect xformed (corners[0], corners[1]);

		// now:  move xformed so it's midpoint is same as original midpoint
		
		xformed.Offset(inDest.MidPoint().h - xformed.MidPoint().h,
						inDest.MidPoint().v - xformed.MidPoint().v);
		SetDest(xformed);
		
		return true;
    	}
	else
		return false;                    
}//end SetScreenDest

// ---------------------------------------------------------------------------
// PhotoPrintItem::SetupDestMatrix 
// create the matrix based on destbounds + rotation (SKEW NOT YET IMPLEMENTED)
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::SetupDestMatrix(MatrixRecord* pMat) {
	if (!this->IsEmpty()) {
		ThrowIfOSErr_(::GraphicsImportSetBoundsRect(*mQTI, &mDest));
		ThrowIfOSErr_(GraphicsImportGetMatrix(*mQTI, pMat));
		}//endif there is a component
	else {
		::SetIdentityMatrix(pMat);
		}
	Point midPoint (mDest.MidPoint());
	::RotateMatrix (pMat, Long2Fix((long)mRot), Long2Fix(midPoint.h), Long2Fix(midPoint.v));
	}//end

#pragma mark -

// ---------------------------------------------------------------------------
// 					P R O X Y L A N D
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// CanUseProxy
// ---------------------------------------------------------------------------
bool
PhotoPrintItem::CanUseProxy(const PhotoDrawingProperties& props) const {
	bool happy (false);

	if (!props.GetPrinting() && gUseProxies)
		happy = true;

	return happy;
} // CanUseProxy

// ---------------------------------------------------------------------------
// MakeProxy
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MakeProxy(
	 MatrixRecord*	inLocalSpace)				// already composited and ready to use
{
	StDisableDebugThrow_();
	StGrafPortSaver				savePort;		// Be sure we're in the right port even if there's a throw

	HORef<MRegion>				cropRgn;
	RgnHandle					workingCrop(this->ResolveCropStuff(cropRgn, nil));

	try {
		//	Create the offscreen GWorld
		MRect					bounds(mDest);	// !!! Assume rectangular
		LGWorld					offscreen(bounds, gProxyBitDepth, useTempMem);

		//	Draw into it
		if (offscreen.BeginDrawing ()) {
			this->DrawImage(inLocalSpace, offscreen.GetMacGWorld(), ::GetGDevice(), workingCrop);
			offscreen.EndDrawing();
		} else
			return;

		//	Now we need to record the GWorld's bits in a PICT; 
		//	we'll blit them to another GWorld
		LGWorld			offscreen2(bounds, gProxyBitDepth);
		if (offscreen2.BeginDrawing()) {
			MNewPicture			pict;			// Creates a PICT and destroys it
			{
				MOpenPicture	openPicture(pict, bounds); 
				offscreen.CopyImage(UQDGlobals::GetCurrentPort(), bounds);
			}
			offscreen2.EndDrawing();

			mProxy.Attach(pict.Detach());		// Transfer ownership of the PICT
		} // if
	} catch (...) {
	}
} // MakeProxy

#pragma mark -

void 	
// ---------------------------------------------------------------------------
//ParseBounds
// ---------------------------------------------------------------------------
PhotoPrintItem::ParseBounds(XML::Element &elem, void *userData) {
	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;
	
	Rect*	pRect ((Rect*)userData);

	SInt16 howMany = sscanf(tmp, "%hd,%hd,%hd,%hd", &pRect->top, &pRect->left, &pRect->bottom, &pRect->right);
	if (howMany != 4) {
		int line = elem.GetInput().GetLine();
		int col = elem.GetInput().GetColumn();
		throw new XML::InvalidValue(elem.GetInput(), line, col);
		}//endif unhappy		
	
	}//end ParseBounds


// ---------------------------------------------------------------------------
//Read
// ---------------------------------------------------------------------------
void PhotoPrintItem::Read(XML::Element &elem)
{
	char	filename[256];
	filename[0] = 0;
	double	minVal (-360.0);
	double	maxVal (360.0);
	
	XML::Handler handlers[] = {
		XML::Handler("bounds", PhotoPrintItem::sParseBounds, (void*)&mDest),
		XML::Handler("crop", PhotoPrintItem::sParseBounds, (void*)&mCrop),
		XML::Handler("maxBounds", PhotoPrintItem::sParseBounds, (void*)&mMaxBounds),
		XML::Handler("filename", filename, sizeof(filename)),
		XML::Handler("rotation", &mRot, &minVal, &maxVal),
		XML::Handler("skew", &mSkew, &minVal, &maxVal),
		XML::Handler("properties", PhotoItemProperties::sParseProperties, (void*)&mProperties),
		XML::Handler::END
	};
	elem.Process(handlers, this);

	if (strlen(filename)) {
		mSpec = new MFileSpec(filename);	
		mQTI = new StQTImportComponent(&*mSpec);
		}//endif a file was specified (empty means template/placeholder)
}//end Read


// ---------------------------------------------------------------------------
//sParseBounds
// ---------------------------------------------------------------------------
void
PhotoPrintItem::sParseBounds(XML::Element &elem, void *userData) {
	((PhotoPrintItem *)userData)->ParseBounds(elem, userData);
	}// StaticParseBoundsXML
	





// ---------------------------------------------------------------------------
//Write
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::Write(XML::Output &out) const
{
	if (!this->IsEmpty()) {
		HORef<char> path (mSpec->MakePath());
		out.WriteElement("filename", path);
		}//endif

	out.BeginElement("bounds", XML::Output::terse);
	out << mDest.top << "," << mDest.left << "," << mDest.bottom << "," << mDest.right;
	out.EndElement(XML::Output::terse);

	out.BeginElement("crop", XML::Output::terse);
	out << mCrop.top << "," << mCrop.left << "," << mCrop.bottom << "," << mCrop.right;
	out.EndElement(XML::Output::terse);

	out.WriteElement("rotation", mRot);
	out.WriteElement("skew", mSkew);

	out.BeginElement("properties", XML::Output::indent);
	mProperties.Write(out);
	out.EndElement(XML::Output::indent);
}//end Write
