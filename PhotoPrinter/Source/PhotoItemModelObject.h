/*
	File:		PhotoItemModelObject.h

	Contains:	Interface to the photo item model object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved

	Change History (most recent first):

         <4>     7/11/01    rmgw    Implement HandleDelete.
         <3>     7/11/01    rmgw    Move MakeNewAEXXXItem to PhotoItemModelObject.
         <2>      7/6/01    rmgw    Implement cloning properties.
         <1>      7/2/01    rmgw    first checked in.
*/


#pragma once

#include <LModelObject.h>

#include "PhotoPrintItem.h"

class PhotoItemProperiesModelObject : public LModelObject

	{
		
		PhotoItemProperties*				mProps;
		
	public:

		enum AETypes {
			enumCaptionT		=	FOUR_CHAR_CODE('Ecap'),
			enumShapeT			=	FOUR_CHAR_CODE('Esha'),
			enumFrameT			=	FOUR_CHAR_CODE('Efra'),
			
			cClass				=	FOUR_CHAR_CODE('phip')
			};
			
		enum AEProperties {
			// what are we allowed to do
			pPICanMove			=	FOUR_CHAR_CODE('PIPm'),
			pPICanResize		=	FOUR_CHAR_CODE('PIP+'),
			pPICanRotate		=	FOUR_CHAR_CODE('PIP¡'),
			pPIMaintainAspect	=	FOUR_CHAR_CODE('PIP|'),
			pPIMaximize			=	FOUR_CHAR_CODE('PIP>'),

			// how do we want to do it?
			pPIAlignment		=	FOUR_CHAR_CODE('PIPa'),
			pPIFullSize			=	FOUR_CHAR_CODE('PIPf'),
			pPICaption			=	FOUR_CHAR_CODE('PIPt'),		//	"Title"
			pPICaptionStyle		=	FOUR_CHAR_CODE('PIPs'),
			pPICaptionLinkedRotation	=	FOUR_CHAR_CODE('PIPl'),
			pPIShowDate			=	FOUR_CHAR_CODE('PIPd'),
			pPIShowName			=	FOUR_CHAR_CODE('PIPn'),
			pPIFontNumber		=	FOUR_CHAR_CODE('PIP#'),
			pPIFontSize			=	FOUR_CHAR_CODE('PIPp'),		//	"Point size"

			// Image shape
			pPIImageShape		=	FOUR_CHAR_CODE('PIP['),
			pPIShadow			=	FOUR_CHAR_CODE('PIP_'),
			pPIShadowColor		=	FOUR_CHAR_CODE('PIP-'),
			pPIBlurEdges		=	FOUR_CHAR_CODE('PIPb'),

			// Decorative frame
			pPIFrameColor		=	FOUR_CHAR_CODE('PIP]'),
			pPIFrameStyle		=	FOUR_CHAR_CODE('PIP}')
			};
	
		enum AECaptionType {
			eCaptionNone			=	FOUR_CHAR_CODE('Kctn'),
			eCaptionBottom			=	FOUR_CHAR_CODE('Kctb'),
			eCaptionRightHorizontal	=	FOUR_CHAR_CODE('Kctr'),
			eCaptionRightVertical	=	FOUR_CHAR_CODE('KctR'),
			eCaptionLeftHorizontal	=	FOUR_CHAR_CODE('Kctl'),
			eCaptionLeftVertical	=	FOUR_CHAR_CODE('KctL'),
			eCaptionTop				=	FOUR_CHAR_CODE('Kctt'),
			eCaptionInside			=	FOUR_CHAR_CODE('Kcti')
			};
			
		enum AEShapeType {
			eShapeRectangle			=	FOUR_CHAR_CODE('Kstr'),
			eShapeOval				=	FOUR_CHAR_CODE('Ksto'),
			eShapeDiamond			=	FOUR_CHAR_CODE('Kstd')
			};
			
		enum AEFrameType {
			eFrameNone				=	FOUR_CHAR_CODE('Kfrn'),
			eFrameHairline			=	FOUR_CHAR_CODE('Kfrh'),
			eFrameThin				=	FOUR_CHAR_CODE('Kfrt'),
			eFrameThick				=	FOUR_CHAR_CODE('KfrT')
			};
			
							PhotoItemProperiesModelObject	(LModelObject*			inSuperModel,
															 PhotoItemProperties*	inProps,
														 	 DescType				inKind = cClass);
		virtual				~PhotoItemProperiesModelObject	(void);

		PhotoItemProperties*	
							GetPhotoItemProperties		(void) const
														{return mProps;};
														
		virtual void		GetAEProperty				(DescType				inProperty,
														 const AEDesc			&inRequestedType,
														 AEDesc					&outPropertyDesc) const;
		virtual void		SetAEPropertyValue			(DescType				inProperty,
														 const AEDesc			&inValue,
														 AEDesc					&outAEReply);
		virtual void		SetAEProperty				(DescType				inProperty,
														 const AEDesc			&inValue,
														 AEDesc					&outAEReply);
										
		virtual bool		AEPropertyExists			(DescType				inProperty) const;
										
		virtual	bool		GetImportantAEProperty		(AERecord&				outRecord,
														 DescType				inProperty) const;
		virtual	void		GetImportantAEProperties	(AERecord&				outRecord) const;
										  
		};

class MAEList;
class PhotoPrintDoc;
class PhotoPrintView;
class PhotoPrintModel;
class Layout;

class PhotoItemModelObject : public LModelObject

	{
		
		PhotoItemRef				mItem;
		
	public:

		enum AEProperties {
			//	pFile
			//	pName
			//	pProperties
			//	pRotation
			
			pCaptionRect	=	FOUR_CHAR_CODE('PIcr'),
			pImageRect		=	FOUR_CHAR_CODE('PIir'),
			pFrameRect		=	FOUR_CHAR_CODE('PIfr'),
			pDestRect		=	FOUR_CHAR_CODE('PIdr'),
			
			pMaxBounds		=	FOUR_CHAR_CODE('PImb'),
			pImageMaxBounds	=	FOUR_CHAR_CODE('PIim'),
			pNaturalBounds	=	FOUR_CHAR_CODE('PInb'),

			pXScale			=	FOUR_CHAR_CODE('PIx%'),
			pYScale			=	FOUR_CHAR_CODE('PIy%'),
			pTopCrop		=	FOUR_CHAR_CODE('PItc'),
			pLeftCrop		=	FOUR_CHAR_CODE('PIlc'),
			pBottomCrop		=	FOUR_CHAR_CODE('PIbc'),
			pRightCrop		=	FOUR_CHAR_CODE('PIrc'),
			pTopOffset		=	FOUR_CHAR_CODE('PIto'),
			pLeftOffset		=	FOUR_CHAR_CODE('PIlo'),
			
			pPIProperties	=	FOUR_CHAR_CODE('PIpr'),
			pSkew			=	FOUR_CHAR_CODE('PIsk')
			
			};
	
		enum AETypes {
			cClass			=	FOUR_CHAR_CODE('phoi')
			};
		
		static	void		MakeNewAEFileItem 			(MAEList&				outProps,
														 const MFileSpec&		inSpec);
		static	void		MakeNewAEFolderItem 		(MAEList&				outProps,
														 const MFileSpec&		inSpec);
														 
							PhotoItemModelObject		(PhotoPrintDoc*			inSuperModel,
														 const	PhotoItemRef&	inItem,
														 DescType				inKind = cClass);
		virtual				~PhotoItemModelObject		(void);
		
		PhotoPrintDoc*		GetDoc						(void) const;
		PhotoPrintModel*	GetDocModel					(void) const;
		PhotoPrintView*		GetView						(void) const;
		Layout*				GetLayout					(void) const;

		const	PhotoItemRef&	
							GetPhotoItem				(void) const
														{return mItem;};
														
		virtual StringPtr	GetModelName				(Str255					outModelName) const;
		
		virtual void		GetAEProperty				(DescType				inProperty,
														 const AEDesc			&inRequestedType,
														 AEDesc					&outPropertyDesc) const;
		virtual void		SetAEPropertyValue			(DescType				inProperty,
														 const AEDesc			&inValue,
														 AEDesc					&outAEReply);
		virtual void		SetAEProperty				(DescType				inProperty,
														 const AEDesc			&inValue,
														 AEDesc					&outAEReply);
										
		virtual bool		AEPropertyExists			(DescType				inProperty) const;
										
		virtual	bool		GetImportantAEProperty		(AERecord&				outRecord,
														 DescType				inProperty) const;
		virtual	void		GetImportantAEProperties	(AERecord&				outRecord) const;
										  
		virtual void		HandleDelete				(AppleEvent				&outAEReply,
														 AEDesc					&outResult);
		};
