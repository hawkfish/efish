//ColorPal.h

#include "Easy_Cheese.h"

typedef short IndexT;
const short kHotChipCount = 8;
	
class ColorPal {
protected:
	static ColorPal*	sSingleton;
	
	static SPBasicSuite	*sP;
	static AEGP_PluginID S_my_id;
	ADMDialogRef	mDialog;

	short mNumColors;
	short mNumRows;
	short mNumCols;
	short mChipHeight;
	short mChipWidth;
	short mScrollbarWidth;
	short mScrollbarHeight;
	AEGP_MemHandle	hChips;

	ADMItemRef	mList;

	ASRGBColor	mHotColors[kHotChipCount];
	ADMItemRef	mHotChips[kHotChipCount];
	
	A_Err AllocateChips(short inNum);
	
public:
	ColorPal(SPBasicSuite	*sP, AEGP_PluginID inPluginID, ADMDialogRef dialog);
	virtual ~ColorPal();
	static ColorPal	*Singleton(void) {return sSingleton;};
	
	static pascal void CustomListDraw ( ADMEntryRef entry, ADMDrawerRef drawer );
	static pascal  ASBoolean  CustomListTrack(ADMEntryRef entry, ADMTrackerRef inTracker);
	static pascal void CustomListNotify(ADMItemRef inItem, ADMNotifierRef inNotifier);
	
	static pascal void  DrawChip(ADMItemRef inItem, ADMDrawerRef inDrawer);
	static pascal  ASBoolean  TrackChip(ADMItemRef inItem, ADMTrackerRef inTracker);
	static pascal void NotifyChip(ADMItemRef inItem, ADMNotifierRef inNotifier);

	 A_Err LoadChips(void);
	 A_Err SaveChips(void);

	 void DrawChip( ADMDrawerRef drawer, IndexT index);
	 A_Err ColorForIndex(IndexT index, ASRGBColor& outColor);
	 void RectForIndex(IndexT index, ASRect& outRect);
	 IndexT IndexForPoint(const ASPoint& inPoint);
	 A_Err	SetIndexColor(IndexT index, const ASRGBColor& inColor, bool refresh = true);

	 void SetupDefaultColors(void);

	 void Layout(const ASRect& rParentBounds);

// accessors
	short GetScrollbarWidth(void) {return mScrollbarWidth;};
	short GetScrollbarHeight(void) {return mScrollbarHeight;};
	short GetNumColors(void)	{return mNumColors;};
	short GetNumRows(void)		{return mNumRows;};
	short GetNumCols(void)		{return mNumCols;};
	short GetChipHeight(void)	{return mChipHeight;};
	short GetChipWidth(void)	{return mChipWidth;};
	const AEGP_MemHandle GetChips(void) {return hChips;};
	
	};