/*
	File:		TN120_Gworld.c

	Contains:	THE CODE BELOW HERE WAS TAKEN STRAIGHT FROM TECH NOTE #120.
				SEE THAT TECH NOTE FOR MORE INFORMATION.  
				(I did remove one parameter from set up pix map.)

	Written by: 	

	Copyright:	Copyright © 1999 by Apple Computer, Inc., All Rights Reserved.

				You may incorporate this Apple sample source code into your program(s) without
				restriction. This Apple sample source code has been provided "AS IS" and the
				responsibility for its operation is yours. You are not permitted to redistribute
				this Apple sample source code as "Apple sample source code" after having made
				changes. If you're going to re-distribute the source, we require that you make
				it clear in the source that the code was descended from Apple sample source
				code, but that you've made changes.

	Change History (most recent first):
				07 Sep 2000		drd			Fixed padding of bytesPerRow
				06 Sep 2000		drd			Carbonized
				7/9/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1
				

*/
#include <QDOffscreen.h>
#include <Memory.h>

#ifndef topLeft
#define topLeft(r)              		(((Point *) &(r))[0])
#endif

#ifndef botRight
#define botRight(r)             		(((Point *) &(r))[1])
#endif

GWorldPtr CreateOffScreen (WindowPtr basePort, short depth, Boolean UseTempMem);

OSErr SetUpPixMap(
    short        depth,       /* Desired number of bits/pixel in off-screen */
    Rect         *bounds,     /* Bounding rectangle of off-screen */
    CTabHandle   colors,      /* Color table to assign to off-screen */
    PixMapHandle aPixMap      /* Handle to the PixMap being initialized */
    );
    
#define kDefaultRes 0x00480000 /* Default resolution is 72 DPI; Fixed type */

OSErr SetUpPixMap(
    short        depth,       /* Desired number of bits/pixel in off-screen */
    Rect         *bounds,     /* Bounding rectangle of off-screen */
    CTabHandle   colors,      /* Color table to assign to off-screen */
    PixMapHandle aPixMap      /* Handle to the PixMap being initialized */
    )
{
    CTabHandle newColors;   	/* Color table used for the off-screen PixMap */
    Ptr        offBaseAddr; 	/* Pointer to the off-screen pixel image */
    OSErr      error;       	/* Returns error code */
    short      bytesPerRow;		/* Number of bytes per row in the PixMap */

    error = noErr;
    newColors = nil;
    offBaseAddr = nil;

  	bytesPerRow = ((depth * (bounds->right - bounds->left) + 15) / 16) * 2;

   /* Clone the clut if indexed color; allocate a dummy clut if direct color*/
    if (depth <= 8)  {
        newColors = colors;
        error = HandToHand((Handle *) &newColors);
    }  else {
        newColors = (CTabHandle) NewHandle(sizeof(ColorTable) -
                sizeof(CSpecArray));
        error = MemError();
   	}
    if (error == noErr) {
        /* Allocate pixel image; long integer multiplication avoids overflow */
        offBaseAddr = NewPtr((unsigned long) bytesPerRow * (bounds->bottom -
                bounds->top));
        if (offBaseAddr != nil)
            {
            /* Initialize fields common to indexed and direct PixMaps */
            (**aPixMap).baseAddr = offBaseAddr;  /* Point to image */
            (**aPixMap).rowBytes = bytesPerRow | /* MSB set for PixMap */
                    0x8000;
            (**aPixMap).bounds = *bounds;        /* Use given bounds */
            (**aPixMap).pmVersion = 0;           /* No special stuff */
            (**aPixMap).packType = 0;            /* Default PICT pack */
            (**aPixMap).packSize = 0;            /* Always zero in mem */
            (**aPixMap).hRes = kDefaultRes;      /* 72 DPI default res */
            (**aPixMap).vRes = kDefaultRes;      /* 72 DPI default res */
            (**aPixMap).pixelSize = depth;       /* Set # bits/pixel */
#if OLDPIXMAPSTRUCT
            (**aPixMap).planeBytes = 0;          /* Not used */
            (**aPixMap).pmReserved = 0;          /* Not used */
#else
            (**aPixMap).pixelFormat = 0;			/* Not used */
            (**aPixMap).pmExt = 0;					/* Not used */
#endif

            /* Initialize fields specific to indexed and direct PixMaps */
            if (depth <= 8)
                {
                /* PixMap is indexed */
                (**aPixMap).pixelType = 0;       /* Indicates indexed */
                (**aPixMap).cmpCount = 1;        /* Have 1 component */
                (**aPixMap).cmpSize = depth;     /* Component size=depth */
                (**aPixMap).pmTable = newColors; /* Handle to CLUT */
            } else {
                /* PixMap is direct */
                (**aPixMap).pixelType = RGBDirect; /* Indicates direct */
                (**aPixMap).cmpCount = 3;          /* Have 3 components */
                if (depth == 16)
                    (**aPixMap).cmpSize = 5;       /* 5 bits/component */
                else
                    (**aPixMap).cmpSize = 8;       /* 8 bits/component */
                (**newColors).ctSeed = 3 * (**aPixMap).cmpSize;
                (**newColors).ctFlags = 0;
                (**newColors).ctSize = 0;
                (**aPixMap).pmTable = newColors;
                }
            }
        else
            error = MemError();
        }
    else
        newColors = nil;

    /* If no errors occured, return a handle to the new off-screen PixMap */
    if (error != noErr)
        {
        if (newColors != nil)
            DisposeCTable(newColors);
        }

    /* Return the error code */
    return error;
    }
