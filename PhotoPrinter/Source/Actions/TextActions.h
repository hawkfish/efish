/*
	File:		TextActions.h

	Contains:	various actions associated with fonts (face, size, style)

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	13 mar 2001		dml		created
*/

#pragma once
#include "ImageActions.h"
#include <Quickdraw.h>
#include <map>
#include "PhotoPrintItem.h"

typedef std::map<PhotoItemRef, SInt16>		SInt16Map;
typedef std::map<PhotoItemRef, Style>		StyleMap;


class FontAction : public MultiImageAction {
	protected:
		SInt16Map	oldMap;
		SInt16	newFont;
		
	public:
					FontAction(PhotoPrintDoc*	inDoc,
								const SInt16	inStringIndex,
								SInt16 			inFontID);
			virtual	~FontAction();
			
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	};//end FontAction
	
	
	
class SizeAction : public MultiImageAction {
	protected:
		SInt16Map	oldMap;
		SInt16	newSize;
		
	public:
					SizeAction(PhotoPrintDoc*	inDoc,
								const SInt16	inStringIndex,
								SInt16 			inSizeID);
			virtual	~SizeAction();
			
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	};//end SizeAction	
	
	
	
class FaceAction : public MultiImageAction {
	protected:
		StyleMap	oldMap;
		Style		newFace;
		
	public:
					FaceAction(PhotoPrintDoc*	inDoc,
								const SInt16	inStringIndex,
								Style 			inStyle);
			virtual	~FaceAction();
			
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	};//end FaceAction
	
	
class ModeAction : public MultiImageAction {
	protected:
		StyleMap	oldMap;
		Style	newMode;
		
	public:
					ModeAction(PhotoPrintDoc*	inDoc,
								const SInt16	inStringIndex,
								short 			inMode);
			virtual	~ModeAction();
			
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	};//end ModeAction
	
	
	