/*
	File:		PhotoItemModelObject.h

	Contains:	Interface to the photo item model object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved

	Change History (most recent first):

         <1>      7/2/01    rmgw    first checked in.
*/


#pragma once

#include <LModelObject.h>

#include "PhotoPrintItem.h"

class PhotoItemModelObject : public LModelObject

	{
		
		PhotoItemRef				mItem;
		
	public:

		enum {
			cClass		=	FOUR_CHAR_CODE('phoi')
			};
			
							PhotoItemModelObject		(LModelObject*			inSuperModel,
														 const	PhotoItemRef&	inItem,
														 DescType				inKind = cClass);
		virtual				~PhotoItemModelObject		(void);

		const	PhotoItemRef&	
							GetPhotoItem				(void) const
														{return mItem;};
														
		virtual StringPtr	GetModelName				(Str255					outModelName) const;
		
		virtual void		GetAEProperty				(DescType				inProperty,
														 const AEDesc			&inRequestedType,
														 AEDesc					&outPropertyDesc) const;
		virtual void		SetAEProperty				(DescType				inProperty,
														 const AEDesc			&inValue,
														 AEDesc					&outAEReply);
										
		virtual bool		AEPropertyExists			(DescType				inProperty) const;
										
		virtual	void		GetImportantAEProperties	(AERecord&				outRecord) const;
										  
		};
