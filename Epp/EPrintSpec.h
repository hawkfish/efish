/*
	File:		EPrintSpec.h

	Contains:	Definition of print spec wrapper.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		03 jul 2000		dml		add GetOrientation
		28 jun 2000		dml		changes to orientation handling.
		27 Jun 2000		drd		SetOrientation
 		27 jun 2000 	dml		add operator==, operator !=
*/

#pragma once

#include "UPrinting.h"

class	EPrintSpec : public LPrintSpec {
	protected:
#if PP_Target_Carbon	// Carbon Printing API
		PMResolution	mResolution;
#endif
		OSType			mOrientation;	
		
	public:
					EPrintSpec();
					EPrintSpec(EPrintSpec& other);
					EPrintSpec(THPrint			inPrintRecordH);
#if PP_Target_Carbon	// Carbon Printing API
					EPrintSpec(
							Handle			inFlatPageFormat);
#endif
			virtual	~EPrintSpec();

		void			SetToSysDefault();

		void	GetResolutions		(SInt16& outVRes, SInt16& outHRes);
		void	SetResolutions		(SInt16 destV, SInt16 destH);
		
		void	GetPageRange 		(SInt16& outFirst, SInt16& outLast);
		void	SetPageRange		(SInt16	first, SInt16 last);

		void	SetOrientation		(const OSType inOrientation);	// 'land' or 'port'
		OSType	GetOrientation		(void);

		OSStatus	Validate		(Boolean& outChanged);
	
	// Override PP's behavior to get something useful
	// (PP returns the page rect from TPrint.prInfoPT.rPage, which
	// is not necessarily the same resolution basis as TPrint.rPaper)
	virtual void GetPageRect(Rect&	outPageRect);


	void	WalkResolutions(SInt16& minX, SInt16& minY, SInt16& maxX, SInt16& maxY);


		int			operator!=		(EPrintSpec	&other) ;
		int			operator==		(EPrintSpec	&other)  {return (*this != other) == 0;};

	}; //end class EPrintSpec