/*
	File:		EPrintSpec.cp

	Contains:	Implementation of print spec wrapper.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		05 Oct 2000		drd		Use IsInSession (instead of SessionIsOpen); removed constructors
								which are no longer inherited; commented SetToSysDefault
		14 Jul 2000		drd		Avoid warning in GetOrientation
		13 jul 2000		dml		GetPageRect more optimized for Carbon, add GetPaperRect
		03 jul 2000		dml		add GetOrientation
		29 Jun 2000		drd		Fixed for non-Carbon builds
		28 jun 2000		dml		SetOrientation now calls ::PMValidatePageFormat (it woiks!)
		27 jun 2000		dml		investigate SetOrientation
		27 Jun 2000		drd		SetOrientation (for Carbon)
		27 jun 2000		dml		changes to operator !=
		27 jun 2000		dml		add operator==
		26 jun 2000 	dml		fix ppc compile (ifdef target_carbon in GetPageRect)
		23 jun 2000		dml		more StPrintSessions, and a call to GetPrintSettings in GetPageRect
								to force (stupid) LPrintSpec to create the damn print settings
		22 jun 2000		dml		added HORef<StPrintSession> all over
		22 Jun 2000		drd		Compile under Carbon
*/

#include "EPrintSpec.h"
#include "HORef.h"
#include <string.h>
#include "MNewHandle.h"
#include "PhotoUtility.h"
#include "ERect32.h"

//---------------------------------------------
//
//---------------------------------------------
EPrintSpec::EPrintSpec()
	:LPrintSpec()
{
}//end

//---------------------------------------------
//
//---------------------------------------------
EPrintSpec::EPrintSpec(EPrintSpec& other)
{
#if PP_Target_Carbon
	Handle		hFormat (other.GetFlatPageFormat());
	Handle		hSettings (other.GetFlatPrintSettings());
	::HandToHand(&hFormat);
	::HandToHand(&hSettings);
	SetFlatPageFormat(hFormat);
	SetFlatPrintSettings(hSettings);
#else
	Handle		h ((Handle)(other.GetPrintRecord()));
	::HandToHand(&h);
	SetPrintRecord((THPrint)h);
#endif
}//end


//---------------------------------------------
//
//---------------------------------------------
EPrintSpec::~EPrintSpec()
{
}//end

// Override PP's behavior to get something useful
// (PP returns the page rect from TPrint.prInfoPT.rPage, which
// is not necessarily the same resolution basis as TPrint.rPaper)
// strangely, this could be correct for carbon.  (see UCarbonPrinting.cp)
//---------------------------------------------
//
//---------------------------------------------
void 
EPrintSpec::GetPageRect(Rect&	outPageRect)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);

#if PP_Target_Carbon
	PMRect pageRect;
	::PMGetAdjustedPageRect(GetPageFormat(), &pageRect);
	// return is in Points (72dpi)
	
	RectCopy(outPageRect, pageRect);
#else
	outPageRect = (*GetPrintRecord())->prInfo.rPage;
#endif
} // GetPageRect



//---------------------------------------------
//GetPaperRect
//---------------------------------------------
void
EPrintSpec::GetPaperRect(Rect&			outPaperRect)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);

#if PP_Target_Carbon
	PMRect paperRect;
	::PMGetAdjustedPaperRect(GetPageFormat(), &paperRect);

	RectCopy(outPaperRect, paperRect);
	
#else
	outPaperRect = (**mPrintRecordH).rPaper;

#endif
}//end GetPaperRect









//---------------------------------------------
//GetResolutions
//---------------------------------------------
void	
EPrintSpec::GetResolutions		(SInt16& outVRes, SInt16& outHRes)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);
#if PP_Target_Carbon
		
	OSStatus ret = ::PMGetResolution(GetPageFormat(), &mResolution);
	if (kPMNoError != ret) Throw_(ret);
	outVRes = (SInt16)mResolution.vRes;
	outHRes = (SInt16)mResolution.hRes;
#else
	outVRes = (*GetPrintRecord())->prInfo.iVRes;
	outHRes = (*GetPrintRecord())->prInfo.iHRes;
#endif
}//end GetResolutions




//---------------------------------------------
//SetResolutions
//---------------------------------------------
void	
EPrintSpec::SetResolutions		(SInt16 destV, SInt16 destH){
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);
#if PP_Target_Carbon
	PMResolution	newRes;
	newRes.vRes = (double)destV;
	newRes.hRes = (double)destH;
	::PMSetResolution(GetPageFormat(), &newRes);
#else
	TSetRslBlk setResRec;
	setResRec.iOpCode = setRslOp;
	setResRec.hPrint = GetPrintRecord();
	setResRec.iXRsl = destH;
	setResRec.iYRsl = destV;
	::PrGeneral(Ptr(&setResRec));
	::PrSetError(noErr);
#endif
} // SetResolutions


//---------------------------------------------
//GetPageRange
//---------------------------------------------
void	
EPrintSpec::GetPageRange 		(SInt16& outFirst, SInt16& outLast)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);
#if PP_Target_Carbon
	UInt32 first;
	UInt32 last;
	OSStatus ret = ::PMGetFirstPage(GetPrintSettings(), &first);
	if (kPMNoError != ret) Throw_(ret);
	ret = ::PMGetLastPage(GetPrintSettings(), &last);
	if (kPMNoError != ret) Throw_(ret);
	outFirst = (SInt16)first;
	outLast = (SInt16)last;
#else
	outFirst = (*GetPrintRecord())->prJob.iFstPage;
	outLast = (*GetPrintRecord())->prJob.iLstPage;
#endif
}//end

/*
SetOrientation
*/
void
EPrintSpec::SetOrientation(const OSType inOrientation)
{
	// not it internally
	mOrientation = inOrientation;

	// make sure we have a session so we can make PM calls
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);

#if PP_Target_Carbon
	if (this->IsInSession()) {
		PMOrientation	orient;

		if (mOrientation == 'land')
			orient = kPMLandscape;
		else
			orient = kPMPortrait;

		OSStatus status (::PMSetOrientation(GetPageFormat(), orient, true));
		Boolean anyChanges;
		status = ::PMValidatePageFormat(GetPageFormat(), &anyChanges);
		}//endif session is open 
#else
	
#endif
} // SetOrientation



/*
GetOrientation
*/
OSType 
EPrintSpec::GetOrientation(void)
{
	// make sure we have a session so we can make PM calls
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);

#if PP_Target_Carbon
	PMOrientation	orient;
	OSStatus status (::PMGetOrientation(GetPageFormat(), &orient));
	switch (orient) {
		case kPMLandscape:
			return kLandscape;
			break;
		case kPMPortrait:
			return kPortrait;
			break;
		}//switch
#else
#endif
	// !!! We don't really support non-Carbon, but return something
	return 'doh!';
}//end GetOrientation


//---------------------------------------------
//
//---------------------------------------------
void	
EPrintSpec::SetPageRange		(SInt16	inFirst, SInt16 inLast)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);
#if PP_Target_Carbon
	UInt32 first (inFirst);
	UInt32 last (inLast);
	OSStatus ret = ::PMSetFirstPage(GetPrintSettings(), first, false);
	if (kPMNoError != ret) Throw_(ret);
	ret = ::PMSetLastPage(GetPrintSettings(), last, false);
	if (kPMNoError != ret) Throw_(ret);
	
#else
	(*GetPrintRecord())->prJob.iFstPage = inFirst;
	(*GetPrintRecord())->prJob.iLstPage = inLast;
#endif
}//end


//---------------------------------------------
//SetToSysDefault
//---------------------------------------------
void			
EPrintSpec::SetToSysDefault()
{
	// !!! SetToSysDefault is no longer an inherited method
#warning	drd		LPrintSpec::SetToSysDefault();
#if PP_Target_Carbon	// Carbon Printing API
	this->GetPrintSettings();
#endif
	}//end SetToSysDefault


//---------------------------------------------
//
//---------------------------------------------
OSStatus	
EPrintSpec::Validate		(Boolean& outChanged)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);
#if PP_Target_Carbon
	OSStatus ret (kPMOutOfScope);
	Boolean changedFormat;
	Boolean changedSettings;

	ret = ::PMValidatePageFormat(GetPageFormat(), &changedFormat);
	if (ret != kPMNoError)
		Throw_(ret);
	if (ret == kPMNoError)
		ret = ::PMValidatePrintSettings(GetPrintSettings(), &changedSettings);
	
	outChanged = changedFormat || changedSettings;		// ??? Dav had just |
	return ret;
#else
	outChanged = ::PrValidate(GetPrintRecord());
	return (noErr);
#endif
}//end

//---------------------------------------------
//WalkResolutions
//---------------------------------------------
void
EPrintSpec::WalkResolutions(SInt16& minX, SInt16& minY, SInt16& maxX, SInt16& maxY)
{
	HORef<StPrintSession> possibleSession;
	if (!this->IsInSession())
		possibleSession = new StPrintSession(*this);

	// make sure a session is open
	HORef<StPrintSession> driverScope;
	if (!this->IsInSession()) {
		driverScope = new StPrintSession (*this);
		}//endif need to open session

	minX = minY = 72; // all printers seem to support 72dpi (draft == screen) mode
	maxX = maxY = 0; // min positive
		
#if PP_Target_Carbon
	PMResolution res;
		UInt32	count;
		OSStatus status;

		PMGetPrinterResolutionCount(&count);
		for (; count; --count) {
			status = PMGetIndexedPrinterResolution(count, &res);
			if (status == kPMNoError) {
				if (res.vRes == res.hRes) {
					if (res.vRes > maxY) {
						maxX = maxY = res.vRes;
						}//endif it's better resolution
					else {
						if (res.vRes < minX) {
							minY = minX = res.vRes;
							}//endif worse
						}//else check for worse resolution
					}//endif square
				}//endif happy
			}//for
#else
	TGetRslBlk	getResRec;

	getResRec.iOpCode = getRslDataOp;//{get printer resolution info}
	::PrGeneral(Ptr(&getResRec));

	if ((getResRec.iError == noErr) && (::PrError() == noErr)) {
		for (SInt16 i = 0; i < getResRec.iRslRecCnt; ++i) {
	     	if (getResRec.rgRslRec[i].iXRsl == getResRec.rgRslRec[i].iYRsl) {
	     		if (getResRec.rgRslRec[i].iXRsl > maxX) {
	     			maxY = maxX = getResRec.rgRslRec[i].iXRsl;
	     			}//endif better resolution
     			if (getResRec.rgRslRec[i].iXRsl < minX) {
	     			minX = minY = getResRec.rgRslRec[i].iXRsl;
	     			}//endif worse resolution 
	     		}//endif square resolution
			}//for all valid record
		}//endif happy

#endif
}//end WalkResolutions

//---------------------------------------------------------
// operator !=
//
// this is tremendously slow under Carbon, since we 
// fudge by converting to old-style TPrint's and compare those
// attempts to compare flattenned format + flattenned settings failed
// since settings appear to contain insignificant (!?) differences at
// the end of their data.
//---------------------------------------------------------
#if PP_Target_Carbon
int
EPrintSpec::operator!=		(EPrintSpec	&other)
{
	Handle	hMyPrintRec;
	Handle	hOtherPrintRec;

	::PMMakeOldPrintRecord(GetPrintSettings(), GetPageFormat(),&hMyPrintRec);
	::PMMakeOldPrintRecord(other.GetPrintSettings(), other.GetPageFormat(), &hOtherPrintRec);

	MNewHandle hThis (hMyPrintRec);
	MNewHandle hOther (hOtherPrintRec);
	
	StHandleLocker l1 ((Handle)hThis);
	StHandleLocker l2 ((Handle)hOther);
	
	return ::memcmp(*hThis, *hOther, hThis.GetSize());
	}//end operator != carbon
#else
int
EPrintSpec::operator!=		(EPrintSpec	&other)  {
	THPrint	hThis (GetPrintRecord());
	THPrint hOther (other.GetPrintRecord());
	StHandleLocker l1 ((Handle)hThis);
	StHandleLocker l2 ((Handle)hOther);
	return ::memcmp(*hThis, *hOther, sizeof(hThis));
	}//end operator != classic
#endif

