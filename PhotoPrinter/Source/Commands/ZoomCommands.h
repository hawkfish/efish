/*
	File:		ZoomCommands.h

	Contains:	Definition of the ZoomIn and ZoomOut command.

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Aug 2000		dml		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"


// ZoomInCommand
class ZoomInCommand : public PhotoDocCommandAttachment
{
public:
						ZoomInCommand		(const CommandT		inCommand,
											 PhotoPrintDoc*		inDoc);
	virtual 			~ZoomInCommand		();

protected:
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};



//ZoomOutCommand
class ZoomOutCommand : public PhotoDocCommandAttachment
{
public:
						ZoomOutCommand		(const CommandT		inCommand,
											 PhotoPrintDoc*		inDoc);
	virtual 			~ZoomOutCommand		();

protected:
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};



//FitInWindowCommand
class FitInWindowCommand : public PhotoDocCommandAttachment
{
public:
						FitInWindowCommand		(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~FitInWindowCommand		();

protected:
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
	virtual SInt16		CalcFitResolution(void);
};


//ViewFullSizeCommand
class ViewFullSizeCommand : public PhotoDocCommandAttachment
{
public:
						ViewFullSizeCommand		(const CommandT		inCommand,
												 PhotoPrintDoc*		inDoc);
	virtual 			~ViewFullSizeCommand		();

protected:
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};


