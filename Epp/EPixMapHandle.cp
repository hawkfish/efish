#include "EPixMapHandle.h"

// ---------------------------------------------------------------------------
//		¥ EPixMapHandle
// ---------------------------------------------------------------------------

	/* See Tech Note #120 - for info on creating a PixMap by hand as SetUpPixMap
		does.  SetUpPixMap was taken from that Tech Note....
	*/

EPixMapHandle::EPixMapHandle (
    
    const	Rect&	inBounds,		/* Bounding rectangle of off-screen */
    short			inDepth,		/* Desired number of bits/pixel in off-screen */
    CTabHandle		inColors,		/* Handle to the PixMap being initialized */
    SInt16			inHRes,
    SInt16			inVRes)
	
	: MPixMap ((PixMapHandle) ::NewHandle (sizeof (PixMap)))
	, mPixRef (new DisposePixMapHandle (GetPixMap ()))
	
	{ // begin EPixMapHandle
	
		ThrowIfNil_(GetPixMap ());
		
		if (!inColors) inColors = ::GetCTable (inDepth);
		ThrowIfResFail_(inColors);
		
		UInt16		bytesPerRow (CalcRowBytes (inBounds, inDepth));
		
        mColors = new DisposeCTabHandle (inColors);
        
        /* Allocate pixel image; long integer multiplication avoids overflow */
        mBaseAddr = new MNewPtr (CalcImageSize (inBounds, inDepth));

            /* Initialize fields common to indexed and direct PixMaps */
		PixMapHandle	aPixMap (GetPixMap ());
		(**aPixMap).baseAddr = *mBaseAddr;   /* Point to image */
		(**aPixMap).rowBytes = bytesPerRow | /* MSB set for PixMap */
		0x8000;
		(**aPixMap).bounds = inBounds;       /* Use given bounds */
		(**aPixMap).pmVersion = 0;           /* No special stuff */
		(**aPixMap).packType = 0;            /* Default PICT pack */
		(**aPixMap).packSize = 0;            /* Always zero in mem */
		(**aPixMap).hRes = inHRes;      	 /* 72 DPI default res */
		(**aPixMap).vRes = inVRes;      	 /* 72 DPI default res */
		(**aPixMap).pixelSize = inDepth;     /* Set # bits/pixel */
#if OLDPIXMAPSTRUCT
		(**aPixMap).planeBytes = 0;          /* Not used */
		(**aPixMap).pmReserved = 0;          /* Not used */
#else
		(**aPixMap).pixelFormat = 0;		 /* Not used */
		(**aPixMap).pmExt = 0;				 /* Not used */
#endif

		/* Initialize fields specific to indexed and direct PixMaps */
		if (inDepth <= 8) {
			/* PixMap is indexed */
			(**aPixMap).pixelType = 0;       /* Indicates indexed */
			(**aPixMap).cmpCount = 1;        /* Have 1 component */
			(**aPixMap).cmpSize = inDepth;   /* Component size=depth */
			(**aPixMap).pmTable = inColors;  /* Handle to CLUT */
			} // if
			
		else {
			/* PixMap is direct */
			(**aPixMap).pixelType = RGBDirect; /* Indicates direct */
			(**aPixMap).cmpCount = 3;          /* Have 3 components */
			(**aPixMap).cmpSize = (inDepth == 16) ? 5 : 8;
			
			(**inColors).ctSeed = 3 * (**aPixMap).cmpSize;
			(**inColors).ctFlags = 0;
			(**inColors).ctSize = 0;
			(**aPixMap).pmTable = inColors;
			} // else

	} // end EPixMapHandle

// ---------------------------------------------------------------------------
//		¥ ~EPixMapHandle
// ---------------------------------------------------------------------------

EPixMapHandle::~EPixMapHandle (void)

	{ // begin ~EPixMapHandle
	
	} // end ~EPixMapHandle

