/*
	File:		EPrintSpec.cp

	Contains:	Implementation of print spec wrapper.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 jun 2000		dml		whoops.  add yet another one (to GetPageRect)
		22 jun 2000		dml		added HORef<StPrintSession> all over
		22 Jun 2000		drd		Compile under Carbon
*/

#include "EPrintSpec.h"
#include "HORef.h"
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
EPrintSpec::EPrintSpec(THPrint			inPrintRecordH)
	:LPrintSpec(inPrintRecordH)
{
	
}//end


#if PP_Target_Carbon	// Carbon Printing API
//---------------------------------------------
//
//---------------------------------------------
EPrintSpec::EPrintSpec(Handle			inFlatPageFormat)
	:LPrintSpec(inFlatPageFormat)
{
}//end

#endif

//---------------------------------------------
//
//---------------------------------------------
EPrintSpec::~EPrintSpec(){
}//end

// Override PP's behavior to get something useful
// (PP returns the page rect from TPrint.prInfoPT.rPage, which
// is not necessarily the same resolution basis as TPrint.rPaper)
// strangely, this could be correct for carbon.  (see UCarbonPrinting.cp)
//---------------------------------------------
//
//---------------------------------------------
void 
EPrintSpec::GetPageRect(Rect&	outPageRect){
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
		possibleSession = new StPrintSession(*this);

	GetPrintRecord();				// Will create Print Record if it
									//   one doesn't exist
									
	outPageRect = (*GetPrintRecord())->prInfo.rPage;
} // GetPageRect

//---------------------------------------------
//
//---------------------------------------------
void	
EPrintSpec::GetResolutions		(SInt16& outVRes, SInt16& outHRes)
{
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
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
//
//---------------------------------------------
void	
EPrintSpec::SetResolutions		(SInt16 destV, SInt16 destH){
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
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
//
//---------------------------------------------
void	
EPrintSpec::GetPageRange 		(SInt16& outFirst, SInt16& outLast){
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
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

//---------------------------------------------
//
//---------------------------------------------
void	
EPrintSpec::SetPageRange		(SInt16	inFirst, SInt16 inLast){
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
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
//
//---------------------------------------------
OSStatus	
EPrintSpec::Validate		(Boolean& outChanged){
	HORef<StPrintSession> possibleSession;
	if (!UPrinting::SessionIsOpen())
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
	if (!UPrinting::SessionIsOpen())
		possibleSession = new StPrintSession(*this);

	// make sure a session is open
	HORef<StPrintSession> driverScope;
	if (!UPrinting::SessionIsOpen()) {
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

