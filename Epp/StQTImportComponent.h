/*
	File:		StQTImportComponent.h

	Contains:	Object which manages a QTImportComponent
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first)

		21 Aug 2001 	rmgw	Add inline members.
		19 feb 2001 	dml		add inTryWithoutValidation option to ct, for faster loads on well behaved files

*/

#pragma once

#include <ImageCompression.h>

/*******************************
* StQTImportComponent
*	utility class to open/close an import component for a file
*	usage ensures that the QT component is closed if object destructs (even if user's ct fails)
********************************/
class StQTImportComponent 
	
	{
	
		GraphicsImportComponent	mGI;
		
		//	Illegal
					StQTImportComponent		(const	StQTImportComponent&	other);
		StQTImportComponent&	operator=	(const	StQTImportComponent&	other);
		
	public:

		static	void	CheckComponentResult	(ComponentResult		inResult);
	
		explicit	StQTImportComponent		(const FSSpec& 				inSpec, 
											 bool 						inTryWithoutValidationFirst = true);
					StQTImportComponent		(Handle 					dataRef,
											 OSType 					MIMEType);
					StQTImportComponent		(GraphicsImportComponent	gi);
		virtual 	~StQTImportComponent	(void);

		
		operator 	GraphicsImportComponent (void);

		//	Read/Write Accessors
		void		GetBoundsRect			(Rect&						bounds) const;
		void		SetBoundsRect 			(const	Rect&				bounds);

		RgnHandle	GetClip 				(void) const;
		void		SetClip 				(RgnHandle					clipRgn);

		void		GetDataFile				(FSSpec&					theFile);
		void		SetDataFile				(const	FSSpec&				theFile);

		Handle		GetDataHandle 			(void) const;
		void		SetDataHandle 			(Handle						dataRef);

		void		GetDataReference 		(Handle&					dataRef,
 									 		 OSType&					dataReType) const;
		void		SetDataReference 		(Handle						dataRef,
 									 		 OSType						dataReType);

		void		GetDataReferenceOffsetAndLimit	
											(UInt32&					offset,
											 UInt32&					limit) const;
		void		SetDataReferenceOffsetAndLimit	
											(UInt32						offset,
											 UInt32						limit);

		void		GetDataReferenceOffsetAndLimit	
											(wide&						offset,
											 wide&						limit) const;
		void		SetDataReferenceOffsetAndLimit	
											(const	wide&				offset,
											 const	wide&				limit);

		void		GetDestRect				(Rect&						destRect) const;
		void		SetDestRect 			(const	Rect&				destRect);

		void		GetExportSettings		(void*						qtAtomContainerPtr) const;
		void		SetExportSettings		(void*						qtAtomContainerPtr);

		long		GetFlags				(void) const;
		void		SetFlags 				(long						flags);

		void		GetGraphicsMode 		(long&						graphicsMode,
 									 		 RGBColor&					opColor) const;
		void		SetGraphicsMode 		(long						graphicsMode,
 									 		 const RGBColor&			opColor);

		void		GetGWorld 				(CGrafPtr&					port,
 									 		 GDHandle&					gd) const;
		void		SetGWorld 				(CGrafPtr					port,
 									 		 GDHandle					gd);

		UInt32		GetImageIndex			(void) const;
		void		SetImageIndex 			(UInt32						imageIndex);

		void		GetMatrix				(MatrixRecord&				matrix) const;
		void		SetMatrix 				(const	MatrixRecord&		matrix);

		void		GetProgressProc			(ICMProgressProcRecordPtr	progressProc) const;
		void		SetProgressProc 		(ICMProgressProcRecordPtr	progressProc);

		CodecQ		GetQuality				(void) const;
		void		SetQuality 				(CodecQ						quality);

		void		GetSourceRect			(Rect&						sourceRect) const;
		void		SetSourceRect 			(const	Rect&				sourceRect);

		//	Read-Only Accessors
		short		DoesDrawAllPixels		(void) const;

		void		GetAliasedDataReference (Handle&					dataRef,
 									 		 OSType&					dataReType) const;
		PicHandle	GetAsPicture			(void) const;
		Handle		GetColorSyncProfile		(void) const;
		void		GetDataOffsetAndSize	(UInt32&					outOffset,
											 UInt32&					outSize) const;
		void		GetDataOffsetAndSize	(wide&						outOffset,
											 wide&						outSize) const;

		RgnHandle	GetDefaultClip 			(void) const;
		void		GetDefaultGraphicsMode 	(long&						graphicsMode,
 									 		 RGBColor&					opColor) const;
		void		GetDefaultMatrix		(MatrixRecord&				matrix) const;
		void		GetDefaultSourceRect	(Rect&						sourceRect) const;
		
		void		GetGetExportImageTypeList
											(void*						qtAtomContainerPtr) const;
		UInt32		GetImageCount			(void) const;
		ImageDescriptionHandle		
					GetImageDescription		(void) const;
		void		GetMetaData				(void*						userData) const;
		void		GetMIMETypeList			(void*						qtAtomContainerPtr) const;
		void		GetNaturalBounds		(Rect&						naturalBounds) const;

		//	Operations
		void		Draw					(void) const;
		void		DoExportImageFileDialog	(OSType&                    outExportedType,
											 FSSpec&					outExportedSpec,
											 ScriptCode&				outScriptTag,
											 const	FSSpec*				inDefaultSpec = nil,
											 StringPtr					inPrompt = nil,
											 ModalFilterYDUPP			inFilterProc = nil) const;
		void		ExportImageFile			(const	FSSpec&				fss,
											 OSType                     fileType,
											 OSType                     fileCreator = 'ogle',
											 ScriptCode					scriptTag = smDefault) const;
		void		ReadData				(void*						dataPtr,
											 UInt32						dataOffset,
											 UInt32						dataSize) const;
		void		ReadData				(void*						dataPtr,
											 const	wide&				dataOffset,
											 UInt32						dataSize) const;
		void		SaveAsPicture			(const	FSSpec&				fss,
											 ScriptCode					scriptTag = smDefault) const;
		void		SaveAsQuickTimeImageFile(const	FSSpec&				fss,
											 ScriptCode					scriptTag = smDefault) const;
		Boolean		Validate				(void) const;
	}; //end class StQTImportComponent

//	===	Inlines ===

inline StQTImportComponent::operator GraphicsImportComponent (void)
	{return mGI;}


//	Read/Write Accessors
inline void
StQTImportComponent::GetBoundsRect (Rect& bounds) const
	{CheckComponentResult (::GraphicsImportGetBoundsRect (mGI, &bounds));}

inline void
StQTImportComponent::SetBoundsRect (const Rect& bounds)
	{CheckComponentResult (::GraphicsImportSetBoundsRect (mGI, &bounds));}

inline RgnHandle
StQTImportComponent::GetClip (void) const
	{RgnHandle r; CheckComponentResult (::GraphicsImportGetClip (mGI, &r)); return r;}

inline void
StQTImportComponent::SetClip (RgnHandle clipRgn)
	{CheckComponentResult (::GraphicsImportSetClip (mGI, clipRgn));}

inline void
StQTImportComponent::GetDataFile (FSSpec&	theFile)
	{CheckComponentResult (::GraphicsImportGetDataFile (mGI, &theFile));}

inline void
StQTImportComponent::SetDataFile (const	FSSpec&	theFile)
	{CheckComponentResult (::GraphicsImportSetDataFile (mGI, &theFile));}

inline Handle
StQTImportComponent::GetDataHandle (void) const
	{Handle h; CheckComponentResult (::GraphicsImportGetDataHandle (mGI, &h)); return h;}

inline void
StQTImportComponent::SetDataHandle (Handle dataRef)
	{CheckComponentResult (::GraphicsImportSetDataHandle (mGI, dataRef));}

inline void
StQTImportComponent::GetDataReference (Handle& dataRef, OSType& dataReType) const
	{CheckComponentResult (::GraphicsImportGetDataReference (mGI, &dataRef, &dataReType));}

inline void
StQTImportComponent::SetDataReference (Handle dataRef, OSType dataReType)
	{CheckComponentResult (::GraphicsImportSetDataReference (mGI, dataRef, dataReType));}

inline void
StQTImportComponent::GetDataReferenceOffsetAndLimit (UInt32& offset, UInt32& limit) const
	{CheckComponentResult (::GraphicsImportGetDataReferenceOffsetAndLimit (mGI, &offset, &limit));}

inline void
StQTImportComponent::SetDataReferenceOffsetAndLimit (UInt32 offset, UInt32 limit)
	{CheckComponentResult (::GraphicsImportSetDataReferenceOffsetAndLimit (mGI, offset, limit));}

inline void
StQTImportComponent::GetDataReferenceOffsetAndLimit (wide& offset, wide& limit) const
	{CheckComponentResult (::GraphicsImportGetDataReferenceOffsetAndLimit64 (mGI, &offset, &limit));}

inline void
StQTImportComponent::SetDataReferenceOffsetAndLimit (const wide& offset, const wide& limit)
	{CheckComponentResult (::GraphicsImportSetDataReferenceOffsetAndLimit64 (mGI, &offset, &limit));}

inline void
StQTImportComponent::GetDestRect (Rect& destRect) const
	{CheckComponentResult (::GraphicsImportGetDestRect (mGI, &destRect));}

inline void
StQTImportComponent::SetDestRect (const Rect& destRect)
	{CheckComponentResult (::GraphicsImportSetDestRect (mGI, &destRect));}

inline void
StQTImportComponent::GetExportSettings (void* qtAtomContainerPtr) const
	{CheckComponentResult (::GraphicsImportGetExportSettingsAsAtomContainer (mGI, qtAtomContainerPtr));}

inline void
StQTImportComponent::SetExportSettings (void* qtAtomContainerPtr)
	{CheckComponentResult (::GraphicsImportSetExportSettingsFromAtomContainer (mGI, qtAtomContainerPtr));}

inline long
StQTImportComponent::GetFlags (void) const
	{long f; CheckComponentResult (::GraphicsImportGetFlags (mGI, &f)); return f;}

inline void
StQTImportComponent::SetFlags (long imageIndex)
	{CheckComponentResult (::GraphicsImportSetFlags (mGI, imageIndex));}

inline void
StQTImportComponent::GetGraphicsMode (long& graphicsMode, RGBColor& opColor) const
	{CheckComponentResult (::GraphicsImportGetGraphicsMode (mGI, &graphicsMode, &opColor));}

inline void
StQTImportComponent::SetGraphicsMode (long graphicsMode, const RGBColor& opColor)
	{CheckComponentResult (::GraphicsImportSetGraphicsMode (mGI, graphicsMode, &opColor));}

inline void
StQTImportComponent::GetGWorld (CGrafPtr& port, GDHandle& gd) const
	{CheckComponentResult (::GraphicsImportGetGWorld (mGI, &port, &gd));}

inline void
StQTImportComponent::SetGWorld (CGrafPtr port, GDHandle gd)
	{CheckComponentResult (::GraphicsImportSetGWorld (mGI, port, gd));}

inline UInt32
StQTImportComponent::GetImageIndex (void) const
	{UInt32 i; CheckComponentResult (::GraphicsImportGetImageIndex (mGI, &i)); return i;}

inline void
StQTImportComponent::SetImageIndex (UInt32 imageIndex)
	{CheckComponentResult (::GraphicsImportSetImageIndex (mGI, imageIndex));}

inline void
StQTImportComponent::GetMatrix (MatrixRecord& matrix) const
	{CheckComponentResult (::GraphicsImportGetMatrix (mGI, &matrix));}

inline void
StQTImportComponent::SetMatrix (const MatrixRecord& matrix)
	{CheckComponentResult (::GraphicsImportSetMatrix (mGI, &matrix));}

inline void
StQTImportComponent::GetProgressProc (ICMProgressProcRecordPtr progressProc) const
	{CheckComponentResult (::GraphicsImportGetProgressProc (mGI, progressProc));}

inline void
StQTImportComponent::SetProgressProc (ICMProgressProcRecordPtr progressProc)
	{CheckComponentResult (::GraphicsImportSetProgressProc (mGI, progressProc));}

inline CodecQ
StQTImportComponent::GetQuality (void) const
	{CodecQ q; CheckComponentResult (::GraphicsImportGetQuality (mGI, &q)); return q;}

inline void
StQTImportComponent::SetQuality (CodecQ quality)
	{CheckComponentResult (::GraphicsImportSetQuality (mGI, quality));}

inline void
StQTImportComponent::GetSourceRect (Rect& sourceRect) const
	{CheckComponentResult (::GraphicsImportGetSourceRect (mGI, &sourceRect));}

inline void
StQTImportComponent::SetSourceRect (const Rect& sourceRect)
	{CheckComponentResult (::GraphicsImportSetSourceRect (mGI, &sourceRect));}


//	Read-Only Accessors
inline short
StQTImportComponent::DoesDrawAllPixels (void) const
	{short s; CheckComponentResult (::GraphicsImportDoesDrawAllPixels (mGI, &s)); return s;}

inline void
StQTImportComponent::GetAliasedDataReference (Handle& dataRef, OSType& dataReType) const
	{CheckComponentResult (::GraphicsImportGetAliasedDataReference (mGI, &dataRef, &dataReType));}

inline PicHandle
StQTImportComponent::GetAsPicture (void) const
	{PicHandle p; CheckComponentResult (::GraphicsImportGetAsPicture (mGI, &p)); return p;}

inline Handle
StQTImportComponent::GetColorSyncProfile (void) const
	{Handle h; CheckComponentResult (::GraphicsImportGetColorSyncProfile (mGI, &h)); return h;}

inline void
StQTImportComponent::GetDataOffsetAndSize (UInt32& outOffset, UInt32& outSize) const
	{CheckComponentResult (::GraphicsImportGetDataOffsetAndSize (mGI, &outOffset, &outSize));}

inline void
StQTImportComponent::GetDataOffsetAndSize (wide& outOffset, wide& outSize) const
	{CheckComponentResult (::GraphicsImportGetDataOffsetAndSize64 (mGI, &outOffset, &outSize));}

inline RgnHandle
StQTImportComponent::GetDefaultClip (void) const
	{RgnHandle r; CheckComponentResult (::GraphicsImportGetDefaultClip (mGI, &r)); return r;}

inline void
StQTImportComponent::GetDefaultGraphicsMode (long& graphicsMode, RGBColor& opColor) const
	{CheckComponentResult (::GraphicsImportGetDefaultGraphicsMode (mGI, &graphicsMode, &opColor));}

inline void
StQTImportComponent::GetDefaultMatrix (MatrixRecord& matrix) const
	{CheckComponentResult (::GraphicsImportGetDefaultMatrix (mGI, &matrix));}

inline void
StQTImportComponent::GetDefaultSourceRect (Rect& sourceRect) const
	{CheckComponentResult (::GraphicsImportGetDefaultSourceRect (mGI, &sourceRect));}

inline void
StQTImportComponent::GetGetExportImageTypeList (void* qtAtomContainerPtr) const
	{CheckComponentResult (::GraphicsImportGetExportImageTypeList (mGI, qtAtomContainerPtr));}

inline UInt32
StQTImportComponent::GetImageCount (void) const
	{UInt32 c; CheckComponentResult (::GraphicsImportGetImageCount (mGI, &c)); return c;}

inline ImageDescriptionHandle
StQTImportComponent::GetImageDescription (void) const
	{ImageDescriptionHandle i; CheckComponentResult (::GraphicsImportGetImageDescription (mGI, &i)); return i;}

inline void
StQTImportComponent::GetMetaData (void* userData) const
	{CheckComponentResult (::GraphicsImportGetMetaData (mGI, userData));}

inline void
StQTImportComponent::GetMIMETypeList (void* qtAtomContainerPtr) const
	{CheckComponentResult (::GraphicsImportGetMIMETypeList (mGI, qtAtomContainerPtr));}

inline void
StQTImportComponent::GetNaturalBounds (Rect& naturalBounds) const
	{CheckComponentResult (::GraphicsImportGetNaturalBounds (mGI, &naturalBounds));}


//	Operations
inline void
StQTImportComponent::Draw (void) const
	{CheckComponentResult (::GraphicsImportDraw (mGI));}

inline void
StQTImportComponent::DoExportImageFileDialog (OSType& outExportedType, FSSpec& outExportedSpec, ScriptCode& outScriptTag, const	FSSpec* inDefaultSpec, StringPtr prompt, ModalFilterYDUPP filterProc) const
	{CheckComponentResult (::GraphicsImportDoExportImageFileDialog (mGI, inDefaultSpec, prompt, filterProc, &outExportedType, &outExportedSpec, &outScriptTag));}

inline void
StQTImportComponent::ExportImageFile (const FSSpec& fss, OSType fileType, OSType fileCreator, ScriptCode scriptTag) const
	{CheckComponentResult (::GraphicsImportExportImageFile (mGI, fileType, fileCreator, &fss, scriptTag));}

inline void
StQTImportComponent::ReadData (void* dataPtr, UInt32 dataOffset, UInt32 dataSize) const
	{CheckComponentResult (::GraphicsImportReadData (mGI, dataPtr, dataOffset, dataSize));}

inline void
StQTImportComponent::ReadData (void* dataPtr, const wide& dataOffset, UInt32 dataSize) const
	{CheckComponentResult (::GraphicsImportReadData64 (mGI, dataPtr, &dataOffset, dataSize));}

inline void
StQTImportComponent::SaveAsPicture (const FSSpec& fss, ScriptCode scriptTag) const
	{CheckComponentResult (::GraphicsImportSaveAsPicture (mGI, &fss, scriptTag));}

inline void
StQTImportComponent::SaveAsQuickTimeImageFile (const FSSpec& fss, ScriptCode scriptTag) const
	{CheckComponentResult (::GraphicsImportSaveAsQuickTimeImageFile (mGI, &fss, scriptTag));}

inline Boolean
StQTImportComponent::Validate (void) const
	{Boolean b; CheckComponentResult (::GraphicsImportValidate (mGI, &b)); return b;}

