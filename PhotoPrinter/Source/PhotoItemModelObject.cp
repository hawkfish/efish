/*
	File:		PhotoItemModelObject.cp

	Contains:	Implementation of the photo item model object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved

	Change History (most recent first):

         <3>      7/6/01    rmgw    Fix string and file parsing.
         <2>      7/6/01    rmgw    Implement cloning properties.
         <1>      7/2/01    rmgw    first checked in.
*/


#include "PhotoItemModelObject.h"

#include "MAEDescIterator.h"

#include "MAEDescExtractors.h"

template<>
struct MAEDescExtractorTraits<MRect> {typedef MRect value_type; enum {ae_type = typeRectangle};};

template<>
struct MAEDescExtractorTraits<double> {typedef double value_type; enum {ae_type = typeIEEE64BitFloatingPoint};};

template<>
struct MAEDescExtractorTraits<PhotoItemProperiesModelObject::AECaptionType> {typedef PhotoItemProperiesModelObject::AECaptionType value_type ; enum {ae_type = typeEnumerated};};

template<>
struct MAEDescExtractorTraits<PhotoItemProperiesModelObject::AEShapeType> {typedef PhotoItemProperiesModelObject::AEShapeType value_type ; enum {ae_type = typeEnumerated};};

template<>
struct MAEDescExtractorTraits<PhotoItemProperiesModelObject::AEFrameType> {typedef PhotoItemProperiesModelObject::AEFrameType value_type ; enum {ae_type = typeEnumerated};};


#include "MAEDescInserters.h"

template<>
struct MAEDescInserterTraits<MRect> {typedef MRect value_type; enum {ae_type = typeRectangle};};

template<>
struct MAEDescInserterTraits<double> {typedef double value_type; enum {ae_type = typeIEEE64BitFloatingPoint};};

template<>
struct MAEDescInserterTraits<PhotoItemProperiesModelObject::AECaptionType> {typedef PhotoItemProperiesModelObject::AECaptionType value_type ; enum {ae_type = typeEnumerated};};

template<>
struct MAEDescInserterTraits<PhotoItemProperiesModelObject::AEShapeType> {typedef PhotoItemProperiesModelObject::AEShapeType value_type ; enum {ae_type = typeEnumerated};};

template<>
struct MAEDescInserterTraits<PhotoItemProperiesModelObject::AEFrameType> {typedef PhotoItemProperiesModelObject::AEFrameType value_type ; enum {ae_type = typeEnumerated};};


//	Slightly modified variant of Bjarne's c_array from p. 496.  This one allows static initialization.

template<class T> 
struct c_array {
	typedef	T			value_type;
	typedef	size_t		size_type;

	typedef	T*			iterator;
	typedef	const	T*	const_iterator;
	
	typedef	T&			reference;
	typedef	const	T&	const_reference;
	
	value_type*			v;
	size_type			v_size;
	
	operator			T*	(void) {return v;}
	size_type			size	(void) const {return v_size;}
	
	reference			operator[] (size_type i) {return v[i];}
	const_reference		operator[] (size_type i) const {return v[i];}
	
	iterator			begin (void) {return v;}
	const_iterator		begin (void) const {return v;}
	
	iterator			end (void) {return v + size ();}
	const_iterator		end (void) const {return v + size ();}
	};
	
//	Mapping table for CaptionT

typedef std::pair<PhotoItemProperiesModelObject::AECaptionType, CaptionT> AECaptionPair;
typedef c_array<AECaptionPair> AECaptionMap;

static AECaptionPair 
sAECaptionData [] = {
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionNone, caption_None),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionBottom, caption_Bottom),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionRightHorizontal, caption_RightHorizontal),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionRightVertical, caption_RightVertical),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionLeftHorizontal, caption_LeftHorizontal),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionLeftVertical, caption_LeftVertical),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionTop, caption_Top),
	AECaptionPair (PhotoItemProperiesModelObject::eCaptionInside, caption_Inside)
	};

const AECaptionMap 
sAECaptionMap = {sAECaptionData, sizeof (sAECaptionData) / sizeof (sAECaptionData[0])};

//	Mapping table for ShapeT
typedef std::pair<PhotoItemProperiesModelObject::AEShapeType, ShapeT> AEShapePair;
typedef c_array<AEShapePair> AEShapeMap;

static AEShapePair 
sAEShapeData [] = {
	AEShapePair (PhotoItemProperiesModelObject::eShapeRectangle, shape_Rectangle),
	AEShapePair (PhotoItemProperiesModelObject::eShapeOval, shape_Oval),
	AEShapePair (PhotoItemProperiesModelObject::eShapeDiamond, shape_Diamond)
	};

const AEShapeMap 
sAEShapeMap = {sAEShapeData, sizeof (sAEShapeData) / sizeof (sAEShapeData[0])};

//	Mapping table for FrameT
typedef std::pair<PhotoItemProperiesModelObject::AEFrameType, FrameT> AEFramePair;
typedef c_array<AEFramePair> AEFrameMap;

static AEFramePair 
sAEFrameData [] = {
	AEFramePair (PhotoItemProperiesModelObject::eFrameNone, frame_None),
	AEFramePair (PhotoItemProperiesModelObject::eFrameHairline, frame_Hairline),
	AEFramePair (PhotoItemProperiesModelObject::eFrameThin, frame_Thin),
	AEFramePair (PhotoItemProperiesModelObject::eFrameThick, frame_Thick)
	};

const AEFrameMap 
sAEFrameMap = {sAEFrameData, sizeof (sAEFrameData) / sizeof (sAEFrameData[0])};

// ---------------------------------------------------------------------------------
//	¥ find_pair_first
// ---------------------------------------------------------------------------------

template <class F, class S>
inline
const std::pair<F, S>*
find_pair_first (const std::pair<F, S>* start, const std::pair<F, S>* stop, const F& value)
	{while (start != stop && !(start->first == value)) ++start; return start;}
	

// ---------------------------------------------------------------------------------
//	¥ find_pair_second
// ---------------------------------------------------------------------------------

template <class F, class S>
inline
const std::pair<F, S>*
find_pair_second (const std::pair<F, S>* start, const std::pair<F, S>* stop, const S& value)
	{while (start != stop && !(start->second == value)) ++start; return start;}

// ---------------------------------------------------------------------------------
//	¥ operator<<
// ---------------------------------------------------------------------------------

inline AEDesc&
operator<< (AEDesc& inDesc, const bool& inT)
	{Boolean b (inT); return inDesc << b;}

#pragma mark -

// ---------------------------------------------------------------------------------
//	¥ PhotoItemProperiesModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemProperiesModelObject::PhotoItemProperiesModelObject (
	
	LModelObject*			inSuperModel,
	PhotoItemProperties*	inProps,
	DescType				inKind)
	
	: LModelObject (inSuperModel, inKind)
	
	, mProps (inProps)
	
	{ // begin PhotoItemProperiesModelObject
		
		Assert_(mProps);
		
	} // end PhotoItemProperiesModelObject

// ---------------------------------------------------------------------------------
//	¥ ~PhotoItemProperiesModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemProperiesModelObject::~PhotoItemProperiesModelObject (void)

	{ // begin ~PhotoItemProperiesModelObject
		
	} // end ~PhotoItemProperiesModelObject

// ---------------------------------------------------------------------------
//	¥ GetAEProperty
// ---------------------------------------------------------------------------
//	Return a descriptor for the specified Property
//
//	Subclasses which have Properties must override this function

void
PhotoItemProperiesModelObject::GetAEProperty (

	DescType		inProperty,
	const AEDesc&	inRequestedType,
	AEDesc			&outPropertyDesc) const
	
	{ // begin GetAEProperty
		
		ConstStr255Param	sp;
		
		switch (inProperty) {
			case pProperties:
				GetImportantAEProperties (outPropertyDesc);
				break;

			// what are we allowed to do
			case pPICanMove:
				outPropertyDesc << GetPhotoItemProperties ()->GetMove ();
				break;
				
			case pPICanResize:
				outPropertyDesc << GetPhotoItemProperties ()->GetResize ();
				break;

			case pPICanRotate:
				outPropertyDesc << GetPhotoItemProperties ()->GetRotate ();
				break;

			case pPIMaintainAspect:
				outPropertyDesc << GetPhotoItemProperties ()->GetAspect ();
				break;

			case pPIMaximize:
				outPropertyDesc << GetPhotoItemProperties ()->GetMaximize ();
				break;

			// how do we want to do it?
			case pPIAlignment:
				outPropertyDesc << GetPhotoItemProperties ()->GetAlignment ();
				break;
				
			case pPIFullSize:
				outPropertyDesc << GetPhotoItemProperties ()->GetFullSize ();
				break;

			case pPICaption:
				sp = GetPhotoItemProperties ()->GetCaption ().AsPascalString ();
				ThrowIfOSErr_(::AECreateDesc(typeChar, sp + 1, sp[0], &outPropertyDesc));
				break;

			case pPICaptionStyle:
				outPropertyDesc << find_pair_second (sAECaptionMap.begin (), sAECaptionMap.end (), GetPhotoItemProperties ()->GetCaptionStyle ())->first;
				break;
				
			case pPICaptionLinkedRotation:
				outPropertyDesc << GetPhotoItemProperties ()->GetCaptionLinkedRotation ();
				break;

			case pPIShowDate:
				outPropertyDesc << GetPhotoItemProperties ()->GetShowDate ();
				break;

			case pPIShowName:
				outPropertyDesc << GetPhotoItemProperties ()->GetShowName ();
				break;

			case pPIFontNumber:
				outPropertyDesc << GetPhotoItemProperties ()->GetFontNumber ();
				break;

			case pPIFontSize:
				outPropertyDesc << GetPhotoItemProperties ()->GetFontSize ();
				break;


			// Image shape
			case pPIImageShape:
				outPropertyDesc << find_pair_second (sAEShapeMap.begin (), sAEShapeMap.end (), GetPhotoItemProperties ()->GetImageShape ())->first;
				break;

			case pPIShadow:
				outPropertyDesc << GetPhotoItemProperties ()->GetShadow ();
				break;

			case pPIShadowColor:
				outPropertyDesc << GetPhotoItemProperties ()->GetShadowColor ();
				break;

			case pPIBlurEdges:
				outPropertyDesc << GetPhotoItemProperties ()->GetBlurEdges ();
				break;


			// Decorative frame
			case pPIFrameColor:
				outPropertyDesc << GetPhotoItemProperties ()->GetFrameColor ();
				break;

			case pPIFrameStyle:
				outPropertyDesc << find_pair_second (sAEFrameMap.begin (), sAEFrameMap.end (), GetPhotoItemProperties ()->GetFrameStyle ())->first;
				break;

			default:
				LModelObject::GetAEProperty (inProperty, inRequestedType, outPropertyDesc);
				break;
			} // switch
			
	} // end GetAEProperty

// ---------------------------------------------------------------------------
//	¥ SetAEPropertyValue
// ---------------------------------------------------------------------------
//	Only change the value - no actions

void
PhotoItemProperiesModelObject::SetAEPropertyValue (

	DescType		inProperty,
	const AEDesc&	inValue,
	AEDesc&			outAEReply)

	{ // begin SetAEPropertyValue
		
		//	Typed buffers
		Boolean		b;
		SInt16		si16;
		Str255		s;
		StringPtr	sp = s;
		RGBColor	rgb;
		
		switch (inProperty) {
			// what are we allowed to do
			case pPICanMove:
				inValue >> b; GetPhotoItemProperties ()->SetMove (b);
				break;

			case pPICanResize:
				inValue >> b; GetPhotoItemProperties ()->SetResize (b);
				break;

			case pPICanRotate:
				inValue >> b; GetPhotoItemProperties ()->SetRotate (b);
				break;

			case pPIMaintainAspect:
				inValue >> b; GetPhotoItemProperties ()->SetAspect (b);
				break;

			case pPIMaximize:
				inValue >> b; GetPhotoItemProperties ()->SetMaximize (b);
				break;


			// how do we want to do it?
			case pPIAlignment:
				{
				AlignmentType	at;
				inValue >> at; GetPhotoItemProperties ()->SetAlignment (at);
				break;
				} // case
				
			case pPIFullSize:
				inValue >> b; GetPhotoItemProperties ()->SetFullSize (b);
				break;

			case pPICaption:
				inValue >> sp; GetPhotoItemProperties ()->SetCaption (sp);
				break;

			case pPICaptionStyle:
				{
				AECaptionType	ct;
				inValue >> ct;
				AECaptionMap::const_iterator i = find_pair_first (sAECaptionMap.begin (), sAECaptionMap.end (), ct);
				if (i == sAECaptionMap.end ()) ThrowOSErr_(errAENoSuchObject);
				
				GetPhotoItemProperties ()->SetCaptionStyle (i->second);
				break;
				} // case
				
			case pPICaptionLinkedRotation:
				inValue >> b; GetPhotoItemProperties ()->SetCaptionLinkedRotation (b);
				break;

			case pPIShowDate:
				inValue >> b; GetPhotoItemProperties ()->SetShowDate (b);
				break;

			case pPIShowName:
				inValue >> b; GetPhotoItemProperties ()->SetShowName (b);
				break;

			case pPIFontNumber:
				inValue >> si16; GetPhotoItemProperties ()->SetFontNumber (si16);
				break;

			case pPIFontSize:
				inValue >> si16; GetPhotoItemProperties ()->SetFontSize (si16);
				break;


			// Image shape
			case pPIImageShape:
				{
				AEShapeType	st;
				inValue >> st;
				AEShapeMap::const_iterator i = find_pair_first (sAEShapeMap.begin (), sAEShapeMap.end (), st);
				if (i == sAEShapeMap.end ()) ThrowOSErr_(errAENoSuchObject);
				
				GetPhotoItemProperties ()->SetImageShape (i->second);
				break;
				} // case

			case pPIShadow:
				inValue >> b; GetPhotoItemProperties ()->SetShadow (b);
				break;

			case pPIShadowColor:
				inValue >> rgb; GetPhotoItemProperties ()->SetShadowColor (rgb);
				break;

			case pPIBlurEdges:
				inValue >> b; GetPhotoItemProperties ()->SetBlurEdges (b);
				break;


			// Decorative frame
			case pPIFrameColor:
				inValue >> rgb; GetPhotoItemProperties ()->SetFrameColor (rgb);
				break;

			case pPIFrameStyle:
				{
				AEFrameType	ft;
				inValue >> ft;
				AEFrameMap::const_iterator i = find_pair_first (sAEFrameMap.begin (), sAEFrameMap.end (), ft);
				if (i == sAEFrameMap.end ()) ThrowOSErr_(errAENoSuchObject);
				
				GetPhotoItemProperties ()->SetFrameStyle (i->second);
				break;
				} // case
				
			default:
				LModelObject::SetAEProperty (inProperty, inValue, outAEReply);
				break;
			} // switch
			
	} // end SetAEPropertyValue

// ---------------------------------------------------------------------------
//	¥ SetAEProperty
// ---------------------------------------------------------------------------
//	Set a Property using data from a descriptor
//
//	Subclasses which have modifiable Properties must override this function
//
//	The best way to do this is actually to factor the object into property 
//	model objects.  Each property then creates an action for the property setting.
//	So if we start to add anything here, we should change to that architecture.
//		- rmgw 7/6/01

void
PhotoItemProperiesModelObject::SetAEProperty (

	DescType		inProperty,
	const AEDesc&	inValue,
	AEDesc&			outAEReply)

	{ // begin SetAEProperty
	
		switch (inProperty) {
			case pProperties:
				{
				//	Special case for parsing a property list
				MAEDescIterator	end (&inValue);
				for (MAEDescIterator i = end; ++i != end; ) 
					SetAEPropertyValue (i.GetKeyword (), *i, outAEReply);
				break;
				} // case
				
			default:
				LModelObject::SetAEProperty (inProperty, inValue, outAEReply);
				break;
			} // switch
			
	} // end SetAEProperty

// ---------------------------------------------------------------------------
//	¥ AEPropertyExists
// ---------------------------------------------------------------------------

bool
PhotoItemProperiesModelObject::AEPropertyExists(
	
	DescType	inProperty) const
	
	{ // begin AEPropertyExists
	
		bool	exists = false;
	
		switch (inProperty) {
			// what are we allowed to do
			case pPICanMove:
			case pPICanResize:
			case pPICanRotate:
			case pPIMaintainAspect:
			case pPIMaximize:

			// how do we want to do it?
			case pPIAlignment:
			case pPIFullSize:
			case pPICaption:
			case pPICaptionStyle:
			case pPICaptionLinkedRotation:
			case pPIShowDate:
			case pPIShowName:
			case pPIFontNumber:
			case pPIFontSize:

			// Image shape
			case pPIImageShape:
			case pPIShadow:
			case pPIShadowColor:
			case pPIBlurEdges:

			// Decorative frame
			case pPIFrameColor:
			case pPIFrameStyle:
				exists = true;
				break;

			default:
				exists = LModelObject::AEPropertyExists (inProperty);
				break;
			} // switch
	
		return exists;
	
	} // end AEPropertyExists

// ---------------------------------------------------------------------------
//		¥ GetImportantAEProperty
// ---------------------------------------------------------------------------

bool
PhotoItemProperiesModelObject::GetImportantAEProperty (

	AERecord&			outRecord,
	DescType			inProperty) const
	
	{ // begin GetImportantAEProperty
		
		if (!AEPropertyExists (inProperty)) return false;
		
		StAEDescriptor	typeDesc;
		
		StAEDescriptor	aProp;
		GetAEProperty (inProperty, typeDesc, aProp);
		UAEDesc::AddKeyDesc (&outRecord, inProperty, aProp);
		
		return true;
		
	} // end GetImportantAEProperty
	
// ---------------------------------------------------------------------------
//	¥ GetImportantAEProperties
// ---------------------------------------------------------------------------

void
PhotoItemProperiesModelObject::GetImportantAEProperties (

	AERecord &outRecord) const

	{ // begin GetImportantAEProperties
		
			// what are we allowed to do
		GetImportantAEProperty (outRecord, pPICanMove);
		GetImportantAEProperty (outRecord, pPICanResize);
		GetImportantAEProperty (outRecord, pPICanRotate);
		GetImportantAEProperty (outRecord, pPIMaintainAspect);
		GetImportantAEProperty (outRecord, pPIMaximize);

			// how do we want to do it?
		GetImportantAEProperty (outRecord, pPIAlignment);
		GetImportantAEProperty (outRecord, pPIFullSize);
		GetImportantAEProperty (outRecord, pPICaption);
		GetImportantAEProperty (outRecord, pPICaptionStyle);
		GetImportantAEProperty (outRecord, pPICaptionLinkedRotation);
		GetImportantAEProperty (outRecord, pPIShowDate);
		GetImportantAEProperty (outRecord, pPIShowName);
		GetImportantAEProperty (outRecord, pPIFontNumber);
		GetImportantAEProperty (outRecord, pPIFontSize);

			// Image shape
		GetImportantAEProperty (outRecord, pPIImageShape);
		GetImportantAEProperty (outRecord, pPIShadow);
		GetImportantAEProperty (outRecord, pPIShadowColor);
		GetImportantAEProperty (outRecord, pPIBlurEdges);

			// Decorative frame
		GetImportantAEProperty (outRecord, pPIFrameColor);
		GetImportantAEProperty (outRecord, pPIFrameStyle);

	} // end GetImportantAEProperties

#pragma mark -

// ---------------------------------------------------------------------------------
//	¥ PhotoItemModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemModelObject::PhotoItemModelObject (
	
	LModelObject*			inSuperModel,
	const	PhotoItemRef&	inItem,
	DescType				inKind)
	
	: LModelObject (inSuperModel, inKind)
	
	, mItem (inItem)
	
	{ // begin PhotoItemModelObject
		
		Assert_(mItem);
		
	} // end PhotoItemModelObject

// ---------------------------------------------------------------------------------
//	¥ ~PhotoItemModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemModelObject::~PhotoItemModelObject (void)

	{ // begin ~PhotoItemModelObject
		
	} // end ~PhotoItemModelObject

// ---------------------------------------------------------------------------------
//	¥ GetModelName												[public, virtual]
// ---------------------------------------------------------------------------------

StringPtr
PhotoItemModelObject::GetModelName (

	Str255			outModelName) const

	{ // begin GetModelName
		
		GetPhotoItem ()->GetName (outModelName);
		
		return outModelName;
		
	} // end GetModelName

// ---------------------------------------------------------------------------
//	¥ GetAEProperty
// ---------------------------------------------------------------------------
//	Return a descriptor for the specified Property
//
//	Subclasses which have Properties must override this function

void
PhotoItemModelObject::GetAEProperty (

	DescType		inProperty,
	const AEDesc&	inRequestedType,
	AEDesc			&outPropertyDesc) const
	
	{ // begin GetAEProperty
		
		double	top;
		double	left;
		double	bottom;
		double	right;
		
		switch (inProperty) {
			case pProperties:
				GetImportantAEProperties (outPropertyDesc);
				break;
				
			case pFile:
				{
				HORef<MFileSpec>	spec (GetPhotoItem ()->GetFileSpec ());
				if (!spec) ThrowOSErr_(errAENoSuchObject);
				
				FSSpec				aeSpec (*spec);
				ThrowIfOSErr_(::AECreateDesc(typeFSS, &aeSpec, sizeof (aeSpec), &outPropertyDesc));
				break;
				}
			
			case pName: 
				{
				Str255	modelName;
				GetModelName (modelName);
				ThrowIfOSErr_(::AECreateDesc(typeChar, modelName + 1, modelName[0], &outPropertyDesc));
				break;
				} // case
	
			case pCaptionRect:
				outPropertyDesc << GetPhotoItem ()->GetCaptionRect ();
				break;
				
			case pImageRect:
				outPropertyDesc << GetPhotoItem ()->GetImageRect ();
				break;

			case pFrameRect:
				outPropertyDesc << GetPhotoItem ()->GetFrameRect ();
				break;

			case pDestRect:
				outPropertyDesc << GetPhotoItem ()->GetDestRect ();
				break;
			
			case pMaxBounds:
				outPropertyDesc << GetPhotoItem ()->GetMaxBounds ();
				break;

			case pImageMaxBounds:
				outPropertyDesc << GetPhotoItem ()->GetImageMaxBounds ();
				break;

			case pXScale:
				GetPhotoItem ()->GetCropZoomScales (left, top);
				outPropertyDesc << left;
				break;

			case pYScale:
				GetPhotoItem ()->GetCropZoomScales (left, top);
				outPropertyDesc << top;
				break;

			case pTopCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				outPropertyDesc << top;
				break;

			case pLeftCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				outPropertyDesc << left;
				break;

			case pBottomCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				outPropertyDesc << bottom;
				break;

			case pRightCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				outPropertyDesc << right;
				break;

			case pTopOffset:
				GetPhotoItem ()->GetCropZoomOffset (top, left);
				outPropertyDesc << top;
				break;

			case pLeftOffset:
				GetPhotoItem ()->GetCropZoomOffset (top, left);
				outPropertyDesc << left;
				break;
			
			case pPIProperties:
				{
				PhotoItemProperties				props (GetPhotoItem ()->GetProperties ());
				PhotoItemProperiesModelObject	propsModel (0, &props);
				propsModel.GetImportantAEProperties (outPropertyDesc);
				break;
				} // case
				
			case pRotation:
				outPropertyDesc << GetPhotoItem ()->GetRotation ();
				break;
			
			case pSkew:
				outPropertyDesc << GetPhotoItem ()->GetSkew ();
				break;

			default:
				LModelObject::GetAEProperty (inProperty, inRequestedType, outPropertyDesc);
				break;
			} // switch
			
	} // end GetAEProperty

// ---------------------------------------------------------------------------
//	¥ SetAEPropertyValue
// ---------------------------------------------------------------------------
//	Only change the value - no actions

void
PhotoItemModelObject::SetAEPropertyValue (

	DescType		inProperty,
	const AEDesc&	inValue,
	AEDesc&			outAEReply)

	{ // begin SetAEPropertyValue
		
		MRect	r;
		
		double	top;
		double	left;
		double	bottom;
		double	right;
		
		FSSpec	fss;
		
		switch (inProperty) {
			case pCaptionRect:
				inValue >> r; GetPhotoItem ()->SetCaptionRect (r);
				break;
				
			case pImageRect:
				inValue >> r; GetPhotoItem ()->SetImageRect (r);
				break;

			case pFrameRect:
				inValue >> r; GetPhotoItem ()->SetFrameRect (r);
				break;

			case pDestRect:
				inValue >> r; GetPhotoItem ()->SetDest (r);
				break;
			
			case pXScale:
				GetPhotoItem ()->GetCropZoomScales (left, top);
				inValue >> left;
				GetPhotoItem ()->SetCropZoomScales (left, top);
				break;

			case pYScale:
				GetPhotoItem ()->GetCropZoomScales (left, top);
				inValue >> top;
				GetPhotoItem ()->SetCropZoomScales (left, top);
				break;

			case pTopCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				inValue >> top;
				GetPhotoItem ()->SetCrop (top, left, bottom, right);
				break;

			case pLeftCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				inValue >> left;
				GetPhotoItem ()->SetCrop (top, left, bottom, right);
				break;

			case pBottomCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				inValue >> bottom;
				GetPhotoItem ()->SetCrop (top, left, bottom, right);
				break;

			case pRightCrop:
				GetPhotoItem ()->GetCrop (top, left, bottom, right);
				inValue >> right;
				GetPhotoItem ()->SetCrop (top, left, bottom, right);
				break;

			case pTopOffset:
				GetPhotoItem ()->GetCropZoomOffset (top, left);
				inValue >> top;
				GetPhotoItem ()->SetCropZoomOffset (top, left);
				break;

			case pLeftOffset:
				GetPhotoItem ()->GetCropZoomOffset (top, left);
				inValue >> left;
				GetPhotoItem ()->SetCropZoomOffset (top, left);
				break;
			
			case pPIProperties:
				{
				PhotoItemProperties				props (GetPhotoItem ()->GetProperties ());
				PhotoItemProperiesModelObject	propsModel (0, &props);
				propsModel.SetAEProperty (pProperties, inValue, outAEReply);
				GetPhotoItem ()->GetProperties () = props;
				break;
				} // case
				
			case pRotation:
				inValue >> left; GetPhotoItem ()->SetRotation (left);
				break;
			
			case pSkew:
				inValue >> left; GetPhotoItem ()->SetSkew (left);
				break;

			case pFile:
				inValue >> fss; GetPhotoItem ()->SetFileSpec (fss);
				break;

			default:
				LModelObject::SetAEProperty (inProperty, inValue, outAEReply);
				break;
			} // switch
			
	} // end SetAEPropertyValue

// ---------------------------------------------------------------------------
//	¥ SetAEProperty
// ---------------------------------------------------------------------------
//	Set a Property using data from a descriptor
//
//	Subclasses which have modifiable Properties must override this function
//
//	The best way to do this is actually to factor the object into property 
//	model objects.  Each property then creates an action for the property setting.
//	So if we start to add anything here, we should change to that architecture.
//		- rmgw 7/6/01

void
PhotoItemModelObject::SetAEProperty (

	DescType		inProperty,
	const AEDesc&	inValue,
	AEDesc&			outAEReply)

	{ // begin SetAEProperty
	
		switch (inProperty) {
			case pProperties:
				{
				//	Special case for parsing a property list
				MAEDescIterator	end (&inValue);
				for (MAEDescIterator i = end; ++i != end; ) 
					SetAEPropertyValue (i.GetKeyword (), *i, outAEReply);
				break;
				} // case
				
			default:
				LModelObject::SetAEProperty (inProperty, inValue, outAEReply);
				break;
			} // switch
			
	} // end SetAEProperty

// ---------------------------------------------------------------------------
//	¥ AEPropertyExists
// ---------------------------------------------------------------------------
//	Subclasses should override to return true for properties supported
//	in overrides of GetAEProperty and SetAEProperty

bool
PhotoItemModelObject::AEPropertyExists(
	
	DescType	inProperty) const
	
	{ // begin AEPropertyExists
	
		bool	exists = false;
	
		switch (inProperty) {
			case pFile:
				exists = (0 != GetPhotoItem ()->GetFileSpec ());
				break;
				
			case pCaptionRect:
			case pImageRect:
			case pFrameRect:
			case pDestRect:
			
			case pMaxBounds:
			case pImageMaxBounds:
			case pNaturalBounds:

			case pXScale:
			case pYScale:
			case pTopCrop:
			case pLeftCrop:
			case pBottomCrop:
			case pRightCrop:
			case pTopOffset:
			case pLeftOffset:
			
			case pPIProperties:
			case pRotation:
			case pSkew:
			
			case pName: 
				exists = true;
				break;
			
			default:
				exists = LModelObject::AEPropertyExists (inProperty);
				break;
			} // switch
	
		return exists;
	
	} // end AEPropertyExists

// ---------------------------------------------------------------------------
//		¥ GetImportantAEProperty
// ---------------------------------------------------------------------------

bool
PhotoItemModelObject::GetImportantAEProperty (

	AERecord&			outRecord,
	DescType			inProperty) const
	
	{ // begin GetImportantAEProperty
		
		if (!AEPropertyExists (inProperty)) return false;
		
		StAEDescriptor	typeDesc;
		
		StAEDescriptor	aProp;
		GetAEProperty (inProperty, typeDesc, aProp);
		UAEDesc::AddKeyDesc (&outRecord, inProperty, aProp);
		
		return true;
		
	} // end GetImportantAEProperty
	
// ---------------------------------------------------------------------------
//	¥ GetImportantAEProperties
// ---------------------------------------------------------------------------

void
PhotoItemModelObject::GetImportantAEProperties (

	AERecord &outRecord) const

	{ // begin GetImportantAEProperties
		
		GetImportantAEProperty (outRecord, pDestRect);
		GetImportantAEProperty (outRecord, pCaptionRect);
		GetImportantAEProperty (outRecord, pImageRect);
		GetImportantAEProperty (outRecord, pFrameRect);
			
		GetImportantAEProperty (outRecord, pXScale);
		GetImportantAEProperty (outRecord, pYScale);
		GetImportantAEProperty (outRecord, pTopCrop);
		GetImportantAEProperty (outRecord, pLeftCrop);
		GetImportantAEProperty (outRecord, pBottomCrop);
		GetImportantAEProperty (outRecord, pRightCrop);
		GetImportantAEProperty (outRecord, pTopOffset);
		GetImportantAEProperty (outRecord, pLeftOffset);

		GetImportantAEProperty (outRecord, pPIProperties);
		GetImportantAEProperty (outRecord, pRotation);
		GetImportantAEProperty (outRecord, pSkew);

		GetImportantAEProperty (outRecord, pFile);

	} // end GetImportantAEProperties

