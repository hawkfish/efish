//PhotoPrintItem.cp
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#include "PhotoPrintItem.h"
#include <algorithm.h>
#include "xmlinput.h"
#include "xmloutput.h"

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
	}//end ct
	


// ---------------------------------------------------------------------------
// PhotoPrintItem copy constructor
// ---------------------------------------------------------------------------
PhotoPrintItem::PhotoPrintItem(PhotoPrintItem& other) 
	: mSpec (other.mSpec)
	, mNaturalBounds (other.GetNaturalBounds())
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



// ---------------------------------------------------------------------------
// MapDestRect.  Used to map mDest from one rect to another
// so that rotation/skewing won't be affected by the transoform 
// otherwise we could accomplish this with a matrix operation
// ---------------------------------------------------------------------------
void
PhotoPrintItem::MapDestRect(const MRect& sourceRect, const MRect& destRect)
{
	::MapRect(&mDest, &sourceRect, &destRect);
}//end MapDestRect

	
	
// ---------------------------------------------------------------------------
// PhotoPrintItem::SetupDestMatrix 
// create the matrix based on destbounds + rotation (SKEW NOT YET IMPLEMENTED)
// ---------------------------------------------------------------------------
void 
PhotoPrintItem::SetupDestMatrix(MatrixRecord* pMat) {
	// is it necessary to set the component's matrix to be identity here?
	ThrowIfOSErr_(::GraphicsImportSetBoundsRect(*mQTI, &mDest));

	ThrowIfOSErr_(GraphicsImportGetMatrix(*mQTI, pMat));
	Point midPoint (mDest.MidPoint());
	::RotateMatrix (pMat, Long2Fix((long)mRot), Long2Fix(midPoint.h), Long2Fix(midPoint.v));
	}//end



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
// PhotoPrintItem::Draw
//			set the qt matrix + have the component render
// ---------------------------------------------------------------------------
void
PhotoPrintItem::Draw(MatrixRecord* worldSpace,
					CGrafPtr destPort,
					GDHandle destDevice,
					RgnHandle	inClip) {
	MatrixRecord	localSpace;

	SetupDestMatrix(&mMat);

	::CopyMatrix(&mMat, &localSpace);
	if (worldSpace)
		::ConcatMatrix(worldSpace, &localSpace);
	
	ThrowIfOSErr_(::GraphicsImportSetMatrix (*mQTI, &localSpace));

	if (destPort && destDevice) 
		ThrowIfOSErr_(::GraphicsImportSetGWorld(*mQTI, destPort, destDevice));
		
	OSErr e = ::GraphicsImportSetClip(*mQTI, inClip);
	::GraphicsImportDraw(*mQTI);
	}//end Draw




ConstStr255Param
PhotoPrintItem::GetName() {
	Assert_(mSpec);
	return mSpec->Name();
	}//end GetName



void 	
PhotoPrintItem::ParseBounds(XML::Element &elem) {
	XML::Char tmp[64];
	size_t len = elem.ReadData(tmp, sizeof(tmp));
	tmp[len] = 0;

	SInt16 howMany = sscanf(tmp, "%hd,%hd,%hd,%hd", &mDest.top, &mDest.left, &mDest.bottom, &mDest.right);
	if (howMany != 4) {
		int line = elem.GetInput().GetLine();
		int col = elem.GetInput().GetColumn();
		throw new XML::InvalidValue(elem.GetInput(), line, col);
		}//endif unhappy		
	
	}//end ParseBounds


void
PhotoPrintItem::sParseBounds(XML::Element &elem, void *userData) {
	((PhotoPrintItem *)userData)->ParseBounds(elem);
	}// StaticParseBoundsXML
	

void PhotoPrintItem::Write(XML::Output &out) const
{
	HORef<char> path (mSpec->MakePath());
	out.WriteElement("filename", path);

	out.BeginElement("bounds", XML::Output::terse);
	out << mDest.top << "," << mDest.left << "," << mDest.bottom << "," << mDest.right;
	out.EndElement(XML::Output::terse);

	out.WriteElement("rotation", mRot);
	out.WriteElement("skew", mSkew);

	out.BeginElement("properties", XML::Output::indent);
	mProperties.Write(out);
	out.EndElement(XML::Output::indent);
}//end Write



void PhotoPrintItem::Read(XML::Element &elem)
{
	char	filename[256];
	double	minVal (-360.0);
	double	maxVal (360.0);
	
	XML::Handler handlers[] = {
		XML::Handler("bounds", PhotoPrintItem::sParseBounds, (void*)this),
		XML::Handler("filename", filename, sizeof(filename)),
		XML::Handler("rotation", &mRot, &minVal, &maxVal),
		XML::Handler("skew", &mSkew, &minVal, &maxVal),
		XML::Handler("properties", PhotoItemProperties::sParseProperties, (void*)&mProperties),
		XML::Handler::END
	};
	elem.Process(handlers, this);

	mSpec = new MFileSpec(filename);	
	mQTI = new StQTImportComponent(&*mSpec);
}//end Read

