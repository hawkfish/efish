#include "ColorPal.h"
#include "ADMDrawer.h"

pascal void  
ColorPal::DrawHotChip(ADMItemRef inItem, ADMDrawerRef inDrawer) {
	A_Err err;
	ADMItemSuite	*adm_itemP = NULL;
	ADMDrawerSuite	*adm_drawerP = NULL;
	err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void**)&adm_itemP);
	if (!err)
		err = sP->AcquireSuite( kADMDrawerSuite, kADMDrawerSuiteVersion, (const void**)&adm_drawerP);
	if (!err && adm_itemP && adm_drawerP) {
		ASRect r;
		adm_itemP->GetBoundsRect(inItem, &r);
		adm_itemP->DefaultDraw(inItem, inDrawer);
		ASRect rDrawer;
		adm_drawerP->GetBoundsRect(inDrawer, &rDrawer);
		short id ((short)(adm_itemP->GetUserData(inItem)));
		adm_drawerP->SetRGBColor(inDrawer, &(ColorPal::Singleton()->mHotColors[id]));
		short top (r.top);
		short left (r.left);
////		r.top -= top;
////		r.bottom -= top;
		r.left -= left;
		r.right -= left;
		adm_drawerP->FillRect(inDrawer, &r);
		sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
		sP->ReleaseSuite(kADMDrawerSuite, kADMDrawerSuiteVersion);
		}//end
	
	}//end DrawHotChip
	
pascal void 
ColorPal::NotifyHotChip(ADMItemRef inItem, ADMNotifierRef inNotifier) {
	A_Err err;
	ADMNotifierSuite	*adm_notifierP = NULL;
	ADMBasicSuite		*adm_basicP = NULL;
	ADMItemSuite		*adm_itemP = NULL;
	err = sP->AcquireSuite(kADMNotifierSuite, kADMNotifierSuiteVersion, (const void**)&adm_notifierP);	
	if (!err && adm_notifierP) {
		if (adm_notifierP->IsNotifierType(inNotifier, kADMIntermediateChangedNotifier)) {
			ADMItemRef theItem (adm_notifierP->GetItem(inNotifier));
			ADMDialogRef dialog (adm_notifierP->GetDialog(inNotifier));
			err = sP->AcquireSuite(kADMBasicSuite, kADMBasicSuiteVersion, (const void**)&adm_basicP);
			if (!err)
				err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void**)&adm_itemP);
			if (!err && adm_basicP && adm_itemP) {
				short id ((short)(adm_itemP->GetUserData(inItem)));
				ASRGBColor old;
				ASRGBColor nouveau;				
				old = ColorPal::Singleton()->mHotColors[id];
				ASRect r;
				adm_itemP->GetBoundsRect(inItem, &r);
				ASPoint p;
				p.v = r.top; p.h = r.left;
				adm_basicP->ChooseColor(p, &old, &nouveau);	
				ColorPal::Singleton()->mHotColors[id] = nouveau;
				sP->ReleaseSuite(kADMBasicSuite, kADMBasicSuiteVersion);
				sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
				}//endif got basic + item suite ok			
			}//endif someone pushed us
		}//endif got suite ok
}//end NotifyHotChip

pascal  ASBoolean  
ColorPal::TrackHotChip(ADMItemRef inItem, ADMTrackerRef inTracker) {
	ASBoolean result (false);
	ADMTrackerSuite	*adm_trackerP = NULL;
	ADMItemSuite	*adm_itemP = NULL;
	A_Err err;
	err = sP->AcquireSuite(kADMTrackerSuite, kADMTrackerSuiteVersion, (const void**)&adm_trackerP);
	if (!err)
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void**)&adm_itemP);
	if (!err && adm_trackerP && adm_itemP) {
//		ASPoint where;
		ASRect r;
//		adm_trackerP->GetPoint(inTracker, &where);
		adm_itemP->GetBoundsRect(inItem, &r);
//		if (where.v >= r.top && where.v <= r.bottom &&
//			where.h >= r.left && where.h <= r.right)
		result = true;
		adm_trackerP->Abort(inTracker);
		sP->ReleaseSuite(kADMTrackerSuite, kADMTrackerSuiteVersion);
		sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
		}//endif

	return result;
	}//end TrackHotChip

#pragma mark -

pascal void 
ColorPal::DrawMainChip ( ADMEntryRef entry, ADMDrawerRef drawer )  {
	A_Err err;
	
	ADMEntrySuite	*adm_entryP = NULL;
	err = sP->AcquireSuite(kADMEntrySuite, kADMEntrySuiteVersion, (const void**)&adm_entryP);
	if (!err && adm_entryP) {
		ADMDrawerSuite*	adm_drawerP;
		err = sP->AcquireSuite( kADMDrawerSuite, kADMDrawerSuiteVersion, (const void**)&adm_drawerP);
		if (!err && adm_drawerP) {
			ASRect r;
			int index (adm_entryP->GetIndex(entry));
			ASRGBColor evenColor;
			ASRGBColor oddColor;
			evenColor.red = evenColor.green = evenColor.blue = 16384;
			oddColor.red = 12345; oddColor.green = 23456; oddColor.blue = 01234;
			if (index % 2 == 0)
				adm_drawerP->SetRGBColor(drawer, &evenColor);
			else
				adm_drawerP->SetRGBColor(drawer, &oddColor);			
			adm_entryP->GetLocalRect(entry, &r);
			adm_drawerP->FillRect(drawer, &r);			
			sP->ReleaseSuite( kADMDrawerSuite, kADMDrawerSuiteVersion);
			}//endif got drawing suite
		sP->ReleaseSuite(kADMEntrySuite, kADMEntrySuiteVersion);
		}//endif got entry suite
	}//end DrawMainChip

pascal void 
ColorPal::NotifyMainChip(ADMItemRef inItem, ADMNotifierRef inNotifier) {
	A_Err err;
	ADMNotifierSuite	*adm_notifierP = NULL;
	ADMItemSuite		*adm_itemP = NULL;
	
	err = sP->AcquireSuite(kADMNotifierSuite, kADMNotifierSuiteVersion, (const void**)&adm_notifierP);	
	if (!err)
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void**)&adm_itemP);	
	if (!err && adm_itemP)
		adm_itemP->Invalidate(inItem);

	if (!err && adm_notifierP) {
		if (adm_notifierP->IsNotifierType(inNotifier, kADMIntermediateChangedNotifier)) {
			ADMItemRef theItem (adm_notifierP->GetItem(inNotifier));
			ADMDialogRef dialog (adm_notifierP->GetDialog(inNotifier));
			
			//figure out which chip
			// bring up the color choose
			// install new color for that chip
			// invalidate that chip (row)
			}//endif someone pushed us
		}//endif got suite ok	
	}//end NotifyMainChip

pascal ASBoolean  
ColorPal::TrackMainList(ADMEntryRef entry, ADMTrackerRef inTracker) {
	ADMTrackerSuite	*adm_trackerP = NULL;
	A_Err err;
	err = sP->AcquireSuite(kADMTrackerSuite, kADMTrackerSuiteVersion, (const void**)&adm_trackerP);
	if (!err && adm_trackerP)
		adm_trackerP->Abort(inTracker);
	sP->ReleaseSuite(kADMTrackerSuite, kADMTrackerSuiteVersion);	
	return true;
	}//end

A_Err
ColorPal::AllocateChips(short inNum) {
	A_Err err;
	AEGP_SuiteHandler	suites(sP);

	// free old chips memory if used
	if (mMainColors != NULL) {
		err = suites.MemorySuite1()->AEGP_FreeMemHandle(mMainColors);
		mMainColors = NULL;
		}//endif

	// and allocate new memory
	err = suites.MemorySuite1()->AEGP_NewMemHandle( S_my_id,			/* >> */
												   	"palette colors",			/* >> */
												   	inNum * sizeof(ASRGBColor),			/* >> */
												  	AEGP_MemFlag_CLEAR,			/* >> */
												   	&mMainColors);	/* << */

	// make sure that if there was an error, mMainColors is NULL
	if (err)
		mMainColors = NULL;

	return err;
	}//end AllocateChips

A_Err
ColorPal::LoadChips()  {
	A_Err ret (A_Err_NONE);
	return ret;
}//LoadChips



A_Err
ColorPal::SaveChips() {
	A_Err ret (A_Err_NONE);
	return ret;
	}//SaveChips
	
	

A_Err
ColorPal::ColorForIndex(IndexT index, ASRGBColor& outColor) {
	A_Err err (A_Err_PARAMETER);
	if ((index < mNumColors) &&
		(mMainColors != NULL)) {
		AEGP_SuiteHandler	suites(sP);
		ASRGBColor	*pChips;
		err = suites.MemorySuite1()->AEGP_LockMemHandle(mMainColors, (void**)&pChips);
		outColor = pChips[index];
		err = suites.MemorySuite1()->AEGP_UnlockMemHandle(mMainColors);
		}//endif sane

	return err;
	}//end ColorForIndex

A_Err
ColorPal::SetIndexColor(IndexT index, const ASRGBColor& inColor, bool refresh) {
	A_Err err (A_Err_PARAMETER);

	if ((index < mNumColors) &&
		(mMainColors != NULL)) {
		AEGP_SuiteHandler	suites(sP);
		ASRGBColor	*pChips;
		err = suites.MemorySuite1()->AEGP_LockMemHandle(mMainColors, (void**)&pChips);
		if (!err) {
			pChips[index] = inColor;
			err = suites.MemorySuite1()->AEGP_UnlockMemHandle(mMainColors);
			}//endif locked happily
		}//endif sane
	return err;
}//end SetIndexColor

void
ColorPal::SetupDefaultColors() {
	mNumColors = 256;
	mNumRows = 16;
	mNumCols = 16;
	AllocateChips(mNumColors);
	
	ASRGBColor rgb;
	for (short i = 0; i < mNumColors; ++i) {
		SetIndexColor(i, rgb);
		}//end for all colors
	}//end SetupDefaultColors

#pragma mark -

ColorPal*	ColorPal::sSingleton = NULL;
SPBasicSuite	*ColorPal::sP = NULL;
AEGP_PluginID 	ColorPal::S_my_id = NULL;

ColorPal::ColorPal(SPBasicSuite	*inSp, AEGP_PluginID inPluginID, ADMDialogRef dialog)
	: mDialog (dialog)
	, mNumColors (0)
	, mNumRows (0)
	, mNumCols (0)
	, mChipHeight (16)
	, mChipWidth (16)
	, mScrollbarWidth (0)
	, mScrollbarHeight (16)
	, mMainColors (NULL)
{
	A_Err	err;
	ADMDialogSuite			*adm_dialogP;

	sSingleton = this;
	sP = inSp;
	S_my_id = inPluginID;
	
	SetupDefaultColors();

	// setup default geometries
	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);
	if (!err && adm_dialogP) {
		adm_dialogP->Size(mDialog, (mNumCols * mChipWidth) + mScrollbarWidth, 
						(mNumRows * mChipHeight) + mScrollbarHeight);
		adm_dialogP->SetMinWidth(mDialog, (mNumCols * mChipWidth) + mScrollbarWidth);
		adm_dialogP->SetMinHeight(mDialog, mChipHeight * 3);
		adm_dialogP->SetMaxWidth(mDialog, (mNumCols * mChipWidth) + mScrollbarWidth);
		adm_dialogP->SetMaxHeight(mDialog, (mChipHeight * mNumRows) + mScrollbarHeight);
		}//endif
	err = sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);
	
	// setup members
	ADMListSuite	*adm_listP = NULL;
	ADMItemSuite	*adm_itemP = NULL;
	ADMEntrySuite	*adm_entryP = NULL;
	err = sP->AcquireSuite(kADMListSuite, kADMListSuiteVersion, (const void**)&adm_listP);
	if (!err)
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void**)&adm_itemP);
	if (!err)
		err = sP->AcquireSuite(kADMEntrySuite, kADMEntrySuiteVersion, (const void**)&adm_entryP);
	
	if (!err && adm_listP && adm_itemP && adm_entryP) {

		// make scrolling list
		ASRect rc;
		rc.top = rc.left = 0;
		rc.right = (short)(mNumCols * mChipWidth);
		rc.bottom = (short)(mNumRows * mChipHeight);
		mMainChips = adm_itemP->Create(mDialog, 0, kADMListBoxType, &rc, NULL, NULL, 0);
		ADMListRef theItemsList = adm_itemP->GetList(mMainChips);
		for (int i = 0; i < 6; ++i) {
			ADMEntryRef entry = adm_listP->InsertEntry(theItemsList, i);
			adm_entryP->SetID(entry, i);
			}//for


		adm_listP->SetEntryHeight(theItemsList, mChipHeight);
		adm_listP->SetEntryWidth(theItemsList, mChipWidth * mNumCols);
		adm_listP->SetDrawProc(theItemsList, ColorPal::DrawMainChip);
		adm_listP->SetTrackProc(theItemsList, ColorPal::TrackMainList);

		// make hot chips
		rc.top = rc.bottom;
		rc.bottom += mScrollbarHeight;
		// yes, we subtract height below.  it's because width is prob 0.  tolerable visual results
		short hotChipWidth ((rc.right - rc.left - mScrollbarHeight) / kHotChipCount);
		for (int j = 0; j < kHotChipCount; ++j) {
			rc.left = j * hotChipWidth;
			rc.right = rc.left + hotChipWidth;
			mHotChips[j] = adm_itemP->Create(mDialog, j, kADMFrameType, &rc, NULL, (ADMUserData)j, 0);
			adm_itemP->SetDrawProc(mHotChips[j], ColorPal::DrawHotChip);
			adm_itemP->SetTrackProc(mHotChips[j], ColorPal::TrackHotChip);
			adm_itemP->SetNotifyProc(mHotChips[j], ColorPal::NotifyHotChip);
			}//end for all quickChips		

		sP->ReleaseSuite(kADMListSuite, kADMListSuiteVersion);
		sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
		sP->ReleaseSuite(kADMEntrySuite, kADMEntrySuiteVersion);
		}//endif got suite ok

	}//end ct
	

ColorPal::~ColorPal() {
	}//end dt


void
ColorPal::Layout(const ASRect& inParentBounds) {
	A_Err err;
	ADMItemSuite	*adm_itemP;

	if (mMainChips != NULL) {
		err = sP->AcquireSuite(kADMItemSuite, kADMItemSuiteVersion, (const void **)&adm_itemP);
		if (!err && adm_itemP) {
			ASRect trimmedR (inParentBounds);
			trimmedR.right -= mScrollbarWidth;
			trimmedR.bottom -= mScrollbarHeight;
			adm_itemP->SetBoundsRect(mMainChips, &trimmedR);
			sP->ReleaseSuite(kADMItemSuite, kADMItemSuiteVersion);
			}//endif suite aquired
		}//endif scroller exists
		
	for (int i = 0; i < kHotChipCount; ++i) {
		ASRect	r;
		adm_itemP->GetBoundsRect(mHotChips[i], &r);
		r.top = inParentBounds.bottom - mScrollbarHeight;
		r.bottom = r.top + mScrollbarHeight;
		adm_itemP->SetBoundsRect(mHotChips[i], &r);
		}//for all hot chips	
	}//end Layout

IndexT
ColorPal::IndexForPoint(const ASPoint& inPoint) {
	IndexT index (0);
	
	return index;
	}//end IndexForPoint

void
ColorPal::RectForIndex(IndexT index, ASRect& outRect) {
	A_Err err;
	ADMDialogSuite*	adm_dialogP;
	
	err = sP->AcquireSuite(kADMDialogSuite, kADMDialogSuiteVersion6, (const void**)&adm_dialogP);	
	if (!err && adm_dialogP) {
		adm_dialogP->GetLocalRect(mDialog, &outRect);
		sP->ReleaseSuite(kADMDialogSuite, kADMDialogSuiteVersion6);	
		}//endif got suite ok
	
	}//end RectForIndex	
	







