/*
	File:		BadgeGroup.h

	Contains:	tab-group commander for photo badges
	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	14 mar 2001		dml			override RotateTarget to handle single badge submit
	26 feb 2001		dml			created

*/

#include <LTabGroup.h>

class	BadgeGroup : public LTabGroup {
	protected:
	
	public:

	enum {
		class_ID = FOUR_CHAR_CODE('Bgrp')
	};

						BadgeGroup();
						BadgeGroup(const BadgeGroup&	inOriginal);
						BadgeGroup(LStream*			inStream);
						BadgeGroup(LCommander*			inSuper);
	virtual				~BadgeGroup();

	virtual Boolean		HandleKeyPress(
								const EventRecord&	inKeyEvent);
	virtual void		RotateTarget(
								Boolean				inBackward);
	};//end BadgeGroup