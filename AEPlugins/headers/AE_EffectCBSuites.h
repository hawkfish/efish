#ifndef _H_AE_EffectCBSuites
#define _H_AE_EffectCBSuites


#include <AE_EffectCB.h>
#include <SPBasic.h>

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#endif

#ifdef _WINDOWS
	#pragma pack( push, AEalign, 2 )
#endif

#ifdef __cplusplus
	extern "C" {
#endif

// note: many of these suites are not SPAPI because they are shared with the
// old-style PF_UtilCallback definitions and we want calls to them to
// be object compatible

#define kPFHandleSuite			"PF Handle Suite"
#define kPFHandleSuiteVersion1	1

typedef struct PF_HandleSuite1 {
	PF_Handle (*host_new_handle)(
		long					size);

	void * (*host_lock_handle)(
		PF_Handle				pf_handle);

	void (*host_unlock_handle)(
		PF_Handle				pf_handle);

	void (*host_dispose_handle)(
		PF_Handle				pf_handle);

	long (*host_get_handle_size)(
		PF_Handle				pf_handle);

	PF_Err (*host_resize_handle)(
		long				new_sizeL,		/* >> */
		PF_Handle			*handlePH);		/* <> Handle Value May Change */
	
} PF_HandleSuite1;


#define kPFANSISuite			"PF ANSI Suite"
#define kPFANSISuiteVersion1	1

typedef struct PF_ANSICallbacksSuite1 {
		double	(*atan)(double);
		double	(*atan2)(double y, double x);	/* returns atan(y/x) - note param order! */
		double	(*ceil)(double);				/* returns next int above x */
		double	(*cos)(double);
		double	(*exp)(double);					/* returns e to the x power */
		double	(*fabs)(double);				/* returns absolute value of x */
		double	(*floor)(double);				/* returns closest int below x */
		double	(*fmod)(double x, double y);	/* returns x mod y */
		double	(*hypot)(double x, double y);	/* returns sqrt(x*x + y*y) */
		double	(*log)(double);					/* returns natural log of x */
		double	(*log10)(double);				/* returns log base 10 of x */
		double	(*pow)(double x, double y);		/* returns x to the y power */
		double	(*sin)(double);
		double	(*sqrt)(double);
		double	(*tan)(double);

		int		(*sprintf)(char *, const char *, ...);
		char *	(*strcpy)(char *, const char *);

		double (*asin)(double);
		double (*acos)(double);
	
} PF_ANSICallbacksSuite1;

#define kPFColorCallbacksSuite			"PF Color Suite"
#define kPFColorCallbacksSuiteVersion1	1


typedef struct PF_ColorCallbacksSuite1 {
		PF_Err (*RGBtoHLS)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		PF_HLS_Pixel	hls);

		PF_Err (*HLStoRGB)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_HLS_Pixel	hls,
		PF_Pixel		*rgb);
		
		PF_Err (*RGBtoYIQ)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		PF_YIQ_Pixel	yiq);

		PF_Err (*YIQtoRGB)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_HLS_Pixel	yiq,
		PF_Pixel		*rgb);
		
		PF_Err (*Luminance)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		long			*lum100);		/* << 100 * luminance */
		
		PF_Err (*Hue)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		long			*hue);			/* << 0-255 maps to 0-360  */
		
		PF_Err (*Lightness)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		long			*lightness);		/* <<  goes from 0-255 */
		
		PF_Err (*Saturation)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel		*rgb,
		long			*saturation);		/* <<  goes from 0-255 */
		
} PF_ColorCallbacksSuite1;

#define kPFColorCallbacks16Suite			"PF Color16 Suite"
#define kPFColorCallbacks16SuiteVersion1	1


typedef struct PF_ColorCallbacks16Suite1 {
		PF_Err (*RGBtoHLS)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		PF_HLS_Pixel	hls);

		PF_Err (*HLStoRGB)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_HLS_Pixel	hls,
		PF_Pixel16		*rgb);
		
		PF_Err (*RGBtoYIQ)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		PF_YIQ_Pixel	yiq);

		PF_Err (*YIQtoRGB)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_HLS_Pixel	yiq,
		PF_Pixel16		*rgb);
		
		PF_Err (*Luminance)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		long			*lum100);		/* << 100 * luminance */
		
		PF_Err (*Hue)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		long			*hue);			/* << 0-255 maps to 0-360  */
		
		PF_Err (*Lightness)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		long			*lightness);		/* <<  goes from 0-32768 */
		
		PF_Err (*Saturation)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Pixel16		*rgb,
		long			*saturation);		/* <<  goes from 0-32768 */
		
} PF_ColorCallbacks16Suite1;


#define kPFBatchSamplingSuite			"PF Batch Sampling Suite"
#define kPFBatchSamplingSuiteVersion1	1


typedef struct PF_BatchSamplingSuite1 {
	 PF_Err (*begin_sampling)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Quality		qual,
		PF_ModeFlags	mf,
		PF_SampPB		*params);
		
	 PF_Err (*end_sampling)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Quality		qual,
		PF_ModeFlags	mf,
		PF_SampPB		*params);
		
	 PF_Err (*get_batch_func)(
		PF_ProgPtr			effect_ref,		/* reference from in_data */
		PF_Quality			quality,
		PF_ModeFlags		mode_flags,
		const PF_SampPB		*params,
		PF_BatchSampleFunc	*batch);

	 PF_Err (*get_batch_func16)(
		PF_ProgPtr				effect_ref,		/* reference from in_data */
		PF_Quality				quality,
		PF_ModeFlags			mode_flags,
		const PF_SampPB			*params,
		PF_BatchSample16Func	*batch);
		
} PF_BatchSamplingSuite1;


#define kPFSampling8Suite			"PF Sampling8 Suite"
#define kPFSampling8SuiteVersion1	1

typedef struct PF_Sampling8Suite1 {

	 PF_Err (*nn_sample)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel		*dst_pixel);

	 PF_Err (*subpixel_sample)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel		*dst_pixel);

	 PF_Err (*area_sample)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel		*dst_pixel);
		

} PF_Sampling8Suite1;

#define kPFSampling16Suite			"PF Sampling16 Suite"
#define kPFSampling16SuiteVersion1	1


typedef struct PF_Sampling16Suite1 {

	 PF_Err (*nn_sample16)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel16		*dst_pixel);
		
	 PF_Err (*subpixel_sample16)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel16		*dst_pixel);

	 PF_Err (*area_sample16)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_Fixed		x,
		PF_Fixed		y,
		const PF_SampPB	*params,
		PF_Pixel16		*dst_pixel);
		
} PF_Sampling16Suite1;



#define kPFWorldSuite			"PF World Suite"
#define kPFWorldSuiteVersion1	1


typedef struct PF_WorldSuite1 {

	 PF_Err (*new_world)(
		PF_ProgPtr			effect_ref,		/* reference from in_data */
		long				width,
		long				height,
		PF_NewWorldFlags	flags,			/* should would be pre-cleared to zeroes */
		PF_World			*world);		/* always 32 bit */

	 PF_Err (*dispose_world)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_World		*world);


} PF_WorldSuite1;



#define kPFIterate8Suite			"PF Iterate8 Suite"
#define kPFIterate8SuiteVersion1	1


typedef struct PF_Iterate8Suite1 {
	 PF_Err (*iterate)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		long			refcon,
		 PF_Err			(*pix_fn)(long refcon, long x, long y, PF_Pixel *in, PF_Pixel *out),
		PF_World		*dst);


	 PF_Err (*iterate_origin)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		const PF_Point	*origin,
		long			refcon,
		 PF_Err			(*pix_fn)(long refcon, long x, long y, PF_Pixel *in, PF_Pixel *out),
		PF_World		*dst);

	 PF_Err (*iterate_lut)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		unsigned char	*a_lut0,		/* pass NULL for identity */
		unsigned char	*r_lut0,		/* pass NULL for identity */
		unsigned char	*g_lut0,		/* pass NULL for identity */
		unsigned char	*b_lut0,		/* pass NULL for identity */
		PF_World		*dst);

	 PF_Err	(*iterate_origin_non_clip_src)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,
		const PF_Point	*origin,
		register long	refcon,
		register PF_Err	(*pix_fn)(long refcon, long x, long y, PF_Pixel *in, PF_Pixel *out),
		PF_World		*dst);

	 PF_Err (*iterate_generic)(
		long			iterationsL,						/* >> */		// can be PF_Iteratations_ONCE_PER_PROCESSOR
		void			*refconPV,							/* >> */
		PF_Err			(*fn_func)(	void *refconPV,			/* >> */
									long thread_indexL,		// only call abort and progress from thread_indexL == 0.
									long i,
									long iterationsL));		// never sends PF_Iteratations_ONCE_PER_PROCESSOR

} PF_Iterate8Suite1;


#define kPFIterate16Suite			"PF iterate16 Suite"
#define kPFIterate16SuiteVersion1	1


typedef struct PF_iterate16Suite1 {
	 PF_Err (*iterate)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		long			refcon,
		 PF_Err			(*pix_fn)(long refcon, long x, long y, PF_Pixel16 *in, PF_Pixel16 *out),
		PF_World		*dst);


	 PF_Err (*iterate_origin)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		const PF_Point	*origin,
		long			refcon,
		 PF_Err			(*pix_fn)(long refcon, long x, long y, PF_Pixel16 *in, PF_Pixel16 *out),
		PF_World		*dst);

	 PF_Err	(*iterate_origin_non_clip_src)(
		PF_InData		*in_data,
		long			progress_base,
		long			progress_final,
		PF_World		*src,
		const PF_Rect	*area,
		const PF_Point	*origin,
		register long	refcon,
		register PF_Err	(*pix_fn)(long refcon, long x, long y, PF_Pixel16 *in, PF_Pixel16 *out),
		PF_World		*dst);
		
} PF_Iterate16Suite1;

#define kPFWorldTransformSuite			"PF World Transform Suite"
#define kPFWorldTransformSuiteVersion1	1

typedef struct PF_WorldTransformSuite1 {

	 PF_Err (*composite_rect)(
		PF_ProgPtr		effect_ref,		/* from in_data */
		PF_Rect			*src_rect,		/* rectangle in source image */
		long			src_opacity,	/* opacity of src */
		PF_World		*source_wld,	/* src PF world */
		long			dest_x,			/* upper left-hand corner of src rect...*/
		long			dest_y,			/* ... in composite image */
		PF_Field		field_rdr,		/* which scanlines to render (all, upper, lower) */
		PF_XferMode		xfer_mode,		/* Copy, Composite Behind, Composite In Front */
		PF_World		*dest_wld);		/* Destination buffer. Already filled */

	 PF_Err (*blend)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		const PF_World	*src1,
		const PF_World	*src2,
		PF_Fixed		ratio,			/* 0 == full src1, 0x00010000 == full src2 */
		PF_World		*dst);

	 PF_Err (*convolve)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_World		*src,
		const PF_Rect	*area,			/* pass NULL for all pixels */
		PF_KernelFlags	flags,
		long			kernel_size,
		void			*a_kernel,
		void			*r_kernel,
		void			*g_kernel,
		void			*b_kernel,
		PF_World		*dst);

	 PF_Err (*copy)(
		PF_ProgPtr		effect_ref,		/* reference from in_data	*/
		PF_World		*src,
		PF_World		*dst,
		PF_Rect 		*src_r,			/* pass NULL for whole world */
		PF_Rect			*dst_r);		/* pass NULL for whole world */

	 PF_Err (*copy_hq)(
		PF_ProgPtr		effect_ref,		/* reference from in_data	*/
		PF_World		*src,
		PF_World		*dst,
		PF_Rect 		*src_r,			/* pass NULL for whole world */
		PF_Rect			*dst_r);		/* pass NULL for whole world */


	 PF_Err	(*transfer_rect)(
		PF_ProgPtr				effect_ref,
		PF_Quality				quality,
		PF_ModeFlags			m_flags,
		PF_Field				field,
		const PF_Rect			*src_rec,
		const PF_World			*src_world,
		const PF_CompositeMode	*comp_mode,
		const PF_MaskWorld		*mask_world0,
		long					dest_x,
		long					dest_y,
		PF_World				*dst_world);

	 PF_Err	(*transform_world)(
		PF_ProgPtr				effect_ref,
		PF_Quality				quality,
		PF_ModeFlags			m_flags,
		PF_Field				field,
		const PF_World			*src_world,
		const PF_CompositeMode	*comp_mode,
		const PF_MaskWorld		*mask_world0,
		const PF_FloatMatrix	*matrices,
		long					num_matrices,
		PF_Boolean				src2dst_matrix,
		const PF_Rect			*dest_rect,
		PF_World				*dst_world);


} PF_WorldTransformSuite1;


#define kPFFillMatteSuite			"PF Fill Matte Suite"
#define kPFFillMatteSuiteVersion1	1

typedef struct PF_FillMatteSuite1 {

	 PF_Err (*fill)(
		PF_ProgPtr		effect_ref,		/* reference from in_data	*/
		const PF_Pixel	*color,
		const PF_Rect	*dst_rect,		/* pass NULL for whole world */
		PF_World		*world);

	 PF_Err (*fill16)(
		PF_ProgPtr			effect_ref,		/* reference from in_data	*/
		const PF_Pixel16	*color,
		const PF_Rect		*dst_rect,		/* pass NULL for whole world */
		PF_World			*world);

	 PF_Err (*premultiply)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		long			forward,		/* TRUE means convert non-premul to premul, FALSE mean reverse */
		PF_World		*dst);
		
	 PF_Err (*premultiply_color)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_World		*src,
		PF_Pixel		*color,			/* color to premultiply/unmultiply with */
		long			forward,		/* TRUE means convert non-premul to premul, FALSE mean reverse */
		PF_World		*dst);
		
	 PF_Err (*premultiply_color16)(
		PF_ProgPtr		effect_ref,		/* reference from in_data */
		PF_World		*src,
		PF_Pixel16		*color,			/* color to premultiply/unmultiply with */
		long			forward,		/* TRUE means convert non-premul to premul, FALSE mean reverse */
		PF_World		*dst);

} PF_FillMatteSuite1;



#ifdef __cplusplus
	}		// end extern "C"
#endif



#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#endif

#ifdef _WINDOWS
	#pragma pack( pop, AEalign)
#endif


#endif

