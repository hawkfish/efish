/*
	File:		PhotoPrintItem.h

	Contains:	Definition of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include "MNewRegion.h"

// note that there are currently two code paths for cropping
// one uses Rectangles, and calls GraphicsImportSetSourceRect
// the other uses Regions and calls GraphicsImportSetClip
// There appear to be some bugs in some QTI's handling of both,
// (jpeg's won't print/print-previw (printer-driver's preview) unless
// crop touches top line of dest)
// so we are currently using regions with some hackery to work around the bug
// see Draw()
#define CROP_BY_REGION 1 // if zero then use rects

// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(const MFileSpec* inSpec) {
	ComponentResult res;
	
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
	mCrop = MRect ();
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
PhotoPrintItem::PhotoPrintItem() {
	
	}//end empty ct



// ---------------------------------------------------------------------------
// ~PhotoPrintItem destructor
// ---------------------------------------------------------------------------
PhotoPrintItem::~PhotoPrintItem() {
	}//end dt


#pragma mark -
// ---------------------------------------------------------------------------
// PhotoPrintItem::Draw
//			set the qt matrix + have the component render
// ---------------------------------------------------------------------------
void
PhotoPrintItem::Draw(const PhotoDrawingProperties& props,
					MatrixRecord* worldSpace,
					CGrafPtr destPort,
					GDHandle destDevice,
					RgnHandle	inClip) {
					
	
	MatrixRecord	localSpace;
	SetupDestMatrix(&mMat);
	OSErr e;
	
	::CopyMatrix(&mMat, &localSpace);
	if (worldSpace)
		::ConcatMatrix(worldSpace, &localSpace);
	
	HORef<MRegion> cropRgn;
	RgnHandle workingCrop (ResolveCropStuff(cropRgn, inClip));
	

	if (Empty()) {
		if (!props.GetPrinting()) {
			DrawEmpty(props, &localSpace, destPort, destDevice, workingCrop);
			}//endif we're not printing
		}//endif empty
	else {
		ThrowIfOSErr_(::GraphicsImportSetMatrix (*mQTI, &localSpace));

		if (destPort && destDevice) {
			e =::GraphicsImportSetGWorld(*mQTI, destPort, destDevice);
			ThrowIfOSErr_(e);
			}//endif
			
#ifdef CROP_BY_REGION
		e = ::GraphicsImportSetClip(*mQTI, workingCrop);
		ThrowIfOSErr_(e);
#else
		e = ::GraphicsImportSetSourceRect (*mQTI, &mCrop);
		ThrowIfOSErr_(e);
#endif
		e = ::GraphicsImportSetQuality (*mQTI, codecMaxQuality);
		ThrowIfOSErr_(e);
		e = ::GraphicsImportDraw(*mQTI);
		ThrowIfOSErr_(e);
		}//else we have something to draw
	}//end Draw



void
PhotoPrintItem::DrawEmpty(const PhotoDrawingProperties& /*props*/,
						 MatrixRecord* localSpace, // already composited and ready to use
						 CGrafPtr inDestPort,
						 GDHandle inDestDevice,
						 RgnHandle inClip) {

	MRect	bounds;
	if (GetDestRect()) // if there is something there, use it.  otherwise, use the maxBounds
		bounds = GetDestRect();
	else
		bounds = GetMaxBounds();


	enum cornerType {
		kTopLeft = 0,
		kTopRight,
		kBotRight,
		kBotLeft,
		kFnordCorner};
		
	Point	corners[kFnordCorner];
	corners[kTopLeft] = GetMaxBounds().TopLeft();
	corners[kBotRight] = GetMaxBounds().BotRight();
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
// MapDestRect.  Used to map mDest from one rect to another
// so that rotation/skewing won't be affected by the transoform 
// otherwise we could accomplish this with a matrix operation
//
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MapDestRect(const MRect& sourceRect, const MRect& destRect)
{
	::MapRect(&mDest, &sourceRect, &destRect);
#ifdef CROP_BY_REGION
	::MapRect(&mCrop, &sourceRect, &destRect);
#endif
// don't map since now using crop on source bounds	::MapRect(&mCrop, &sourceRect, &destRect);
}//end MapDestRect





// ---------------------------------------------------------------------------
// ResolveCropStuff
// ---------------------------------------------------------------------------
RgnHandle
PhotoPrintItem::ResolveCropStuff(HORef<MRegion> cropRgn, RgnHandle inClip)
{
#ifdef CROP_BY_REGION
	// do we have intrinsic cropping?
	if (mCrop) {
		cropRgn = new MNewRegion;
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
		else
			cropRgn = new MRegion (inClip);


		// QTI for SGI crashes if clip outside of dest
		// crop to destRect (which by definition is within NaturalBounds rect)
		MNewRegion destRgn;
		destRgn = mDest;
		cropRgn->Intersect(*cropRgn, destRgn);
		}//endif there is some incoming clipping
#else
	if (inClip != nil)
		*cropRgn = inClip;
#endif		

	// we hold an HORef<MRegion>
	// bool(HORef<>) may be false if nothing is stored
	// and bool(*HORef) may be false if a nil crop region is present
	// here's the two step process in figuring that out.
	RgnHandle workingCrop ((RgnHandle)nil);
	if (cropRgn) 
		workingCrop =  *cropRgn ? *cropRgn : (RgnHandle)nil;

	return workingCrop;
}



// ---------------------------------------------------------------------------
// SetCrop
//
//		new crop bounds.  If new crop outside of destBounds, remove crop
// ---------------------------------------------------------------------------
void			
PhotoPrintItem::SetCrop(const MRect& inCrop) {
	mCrop = inCrop;
	}//end




// ---------------------------------------------------------------------------
// SetDest
//
// ---------------------------------------------------------------------------
void 			
PhotoPrintItem::SetDest(const MRect& inDest) {
	mDest = inDest;
}//end SetDest





// ---------------------------------------------------------------------------
// PhotoPrintItem::SetupDestMatrix 
// create the matrix based on destbounds + rotation (SKEW NOT YET IMPLEMENTED)
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::SetupDestMatrix(MatrixRecord* pMat) {
	if (!Empty()) {
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
	if (!Empty()) {
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
