/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2000 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/



/** AEGP_SuiteHandler.h

	A very helpful class that manages demand loading and automatic, exception-safe freeing
	of AEGP suites.

	USAGE INSTRUCTIONS:

		The accompanying file, AEGP_SuiteHandler.cpp, is designed to be compiled right into 
		the client application or plug-in. 

		You'll get a link error. 

		This is because AEGP_SuiteHandler.cpp lacks a definition for the MissingSuiteError() 
		method. You must provide one to define the error handling behaviour of the class.
		This function may or may not display an error message etc. but it must end 
		by throwing an exception. It cannot return.
		
		Other than that, usage is pretty trivial. Construct with a pointer to the PICA 
		basic suite, and then call the public method to obtain lazily loaded pointers
		to other AEGP suites. Upon desctruction, all loaded suites are freed (so this class
		is really handy for writing exception-safe AEGP code.)

	Copyright (c) 2000 Adobe Systems Inc.

	created 9/11/2000 jms
**/
#ifndef _H_AEGP_SUITEHANDLER
#define _H_AEGP_SUITEHANDLER

#include <AE_GeneralPlug.h>
#include <AE_EffectSuites.h>
#include <AE_EffectCBSuites.h>


// Suite registration and handling object
class AEGP_SuiteHandler {

private:
	// basic suite pointer
	const SPBasicSuite				*i_pica_basicP;

	// Suites we can register. These are mutable because they are demand loaded using a const object.

	mutable AEGP_CompSuite1			*i_comp_suiteP;
	mutable AEGP_ProjSuite1			*i_proj_suiteP;
	mutable AEGP_LayerSuite1		*i_layer_suiteP;
	mutable AEGP_StreamSuite1		*i_stream_suiteP;
	mutable AEGP_KeyframeSuite1		*i_keyframe_suiteP;
	mutable AEGP_CanvasSuite1		*i_canvas_suiteP;
	mutable AEGP_CameraSuite1		*i_camera_suiteP;
	mutable AEGP_RegisterSuite1		*i_register_suiteP;
	mutable AEGP_MemorySuite1		*i_memory_suiteP;
	mutable AEGP_ItemSuite1			*i_item_suiteP;
	mutable AEGP_LightSuite1		*i_light_suiteP;
	mutable AEGP_EffectSuite1		*i_effect_suiteP;
	mutable AEGP_MaskSuite1			*i_mask_suiteP;
	mutable AEGP_MaskOutlineSuite1	*i_mask_outline_suiteP;
	mutable AEGP_FootageSuite1		*i_footage_suiteP;
	mutable AEGP_CommandSuite1		*i_command_suiteP;
	mutable AEGP_UtilitySuite1		*i_utility_suiteP;
	mutable PF_ANSICallbacksSuite1	*i_ansi_suiteP;
	mutable PF_HandleSuite1			*i_handle_suiteP;
	mutable PF_FillMatteSuite1		*i_fill_matte_suiteP;
	mutable PF_WorldTransformSuite1	*i_world_transform_suiteP;
//	mutable AEGP_ArtisanUtilSuite1	*i_util_suiteP;			these suites NYI
//	mutable AEGP_StreamSuite1		*i_stream_suiteP;
//	mutable AEGP_QueryXformSuite1	*i_xform_suiteP;
	mutable AEGP_CompositeSuite1	*i_composite_suiteP;
	mutable PF_WorldSuite1			*i_world_suiteP;
	mutable AEGP_PFInterfaceSuite1  *i_pf_interface_suiteP;

	mutable PF_PathQuerySuite1		*i_path_query_suiteP;
	mutable PF_PathDataSuite1		*i_path_data_suiteP;
	mutable PF_ParamUtilsSuite1		*i_param_utils_suiteP;
	mutable PFAppSuite2				*i_app_suiteP;
	mutable AEGP_IOInSuite1			*i_io_in_suiteP;
	mutable AEGP_IOOutSuite1		*i_io_out_suiteP;
	mutable AEGP_PersistentDataSuite1 *i_persistent_data_suiteP;
	mutable AEGP_RenderQueueSuite1	*i_render_queue_suiteP;
	mutable AEGP_FIMSuite1			*i_fim_suiteP;
	mutable PF_Sampling8Suite1		*i_sampling_8_suiteP;
	mutable PF_Sampling16Suite1		*i_sampling_16_suiteP;
	mutable	PF_Iterate8Suite1		*i_iterate_8_suiteP;
	mutable PF_iterate16Suite1		*i_iterate_16_suiteP;
	mutable AEGP_CollectionSuite1	*i_collection_suiteP;

	// private methods
	void *LoadSuite(char *nameZ, long versionL) const;
	void ReleaseSuite(char *nameZ, long versionL);
	void ReleaseAllSuites();

	// Here is the error handling function which must be defined.
	// It must exit by throwing an exception, it cannot return.
	void MissingSuiteError() const;

public:
	// To construct, pass pica_basicP
	AEGP_SuiteHandler(const SPBasicSuite *pica_basicP);
	~AEGP_SuiteHandler();

	// Suite accessor methods. All of these load on demand.
	AEGP_LayerSuite1 *LayerSuite1() const
	{
		if (!i_layer_suiteP) {
			i_layer_suiteP = (AEGP_LayerSuite1*)LoadSuite(kAEGPLayerSuite, kAEGPLayerSuiteVersion1);
		}
		return i_layer_suiteP;
	}

	AEGP_StreamSuite1 *StreamSuite1() const
	{
		if (!i_stream_suiteP) {
			i_stream_suiteP = (AEGP_StreamSuite1*)LoadSuite(kAEGPStreamSuite, kAEGPStreamSuiteVersion1);
		}
		return i_stream_suiteP;
	}

	AEGP_KeyframeSuite1 *KeyframeSuite1() const
	{
		if (!i_keyframe_suiteP) {
			i_keyframe_suiteP = (AEGP_KeyframeSuite1*)LoadSuite(kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion1);
		}
		return i_keyframe_suiteP;
	}

	AEGP_CompSuite1 *CompSuite1() const
	{
		if (!i_comp_suiteP) {
			i_comp_suiteP = (AEGP_CompSuite1*)LoadSuite(kAEGPCompSuite, kAEGPCompSuiteVersion1);
		}
		return i_comp_suiteP;
	}

	AEGP_ProjSuite1 *ProjSuite1() const
	{
		if (!i_proj_suiteP) {
			i_proj_suiteP = (AEGP_ProjSuite1*)LoadSuite(kAEGPProjSuite, kAEGPProjSuiteVersion1);
		}
		return i_proj_suiteP;
	}
	AEGP_CanvasSuite1 *CanvasSuite1() const
	{
		if (!i_canvas_suiteP) {
			i_canvas_suiteP = (AEGP_CanvasSuite1*)LoadSuite(kAEGPCanvasSuite, kAEGPCanvasSuiteVersion1);
		}
		return i_canvas_suiteP;
	}

	AEGP_CameraSuite1 *CameraSuite1() const
	{
		if (!i_camera_suiteP) {
			i_camera_suiteP = (AEGP_CameraSuite1*)LoadSuite(kAEGPCameraSuite, kAEGPCameraSuiteVersion1);
		}
		return i_camera_suiteP;
	}

	AEGP_RegisterSuite1 *RegisterSuite1() const
	{
		if (!i_register_suiteP) {
			i_register_suiteP = (AEGP_RegisterSuite1*)LoadSuite(kAEGPRegisterSuite, kAEGPRegisterSuiteVersion1);
		}
		return i_register_suiteP;
	}

	AEGP_MemorySuite1 *MemorySuite1() const
	{
		if (!i_memory_suiteP) {
			i_memory_suiteP = (AEGP_MemorySuite1*)LoadSuite(kAEGPMemorySuite, kAEGPMemorySuiteVersion1);
		}
		return i_memory_suiteP;
	}

	AEGP_ItemSuite1 *ItemSuite1() const
	{
		if (!i_item_suiteP) {
			i_item_suiteP = (AEGP_ItemSuite1*)LoadSuite(kAEGPItemSuite, kAEGPItemSuiteVersion1);
		}
		return i_item_suiteP;
	}

	AEGP_PFInterfaceSuite1 *PFInterfaceSuite1() const
	{
		if (!i_pf_interface_suiteP) {
			i_pf_interface_suiteP = (AEGP_PFInterfaceSuite1*)LoadSuite(kAEGPPFInterfaceSuite, kAEGPPFInterfaceSuiteVersion1);
		}
		return i_pf_interface_suiteP;
	}

	PF_PathQuerySuite1 *PathQuerySuite1() const
	{
		if (!i_path_query_suiteP) {
			i_path_query_suiteP = (PF_PathQuerySuite1*)LoadSuite(kPFPathQuerySuite, kPFPathQuerySuiteVersion1);
		}
		return i_path_query_suiteP;
	}

	PF_PathDataSuite1 *PathDataSuite1() const
	{
		if (!i_path_data_suiteP) {
			i_path_data_suiteP = (PF_PathDataSuite1*)LoadSuite(kPFPathDataSuite, kPFPathDataSuiteVersion1);
		}
		return i_path_data_suiteP;
	}

	PF_ParamUtilsSuite1 *ParamUtilsSuite1() const
	{
		if (!i_param_utils_suiteP) {
			i_param_utils_suiteP = (PF_ParamUtilsSuite1*)LoadSuite(kPFParamUtilsSuite, kPFParamUtilsSuiteVersion1);
		}
		return i_param_utils_suiteP;
	}

	PFAppSuite2 *AppSuite2() const
	{
		if (!i_app_suiteP) {
			i_app_suiteP = (PFAppSuite2*)LoadSuite(kPFAppSuite, kPFAppSuiteVersion2);
		}
		return i_app_suiteP;
	}

	AEGP_LightSuite1 *LightSuite1() const
	{
		if (!i_light_suiteP) {
			i_light_suiteP = (AEGP_LightSuite1*)LoadSuite(kAEGPLightSuite, kAEGPLightSuiteVersion1);
		}
		return i_light_suiteP;
	}

	AEGP_EffectSuite1 *EffectSuite1() const
	{
		if (!i_effect_suiteP) {
			i_effect_suiteP = (AEGP_EffectSuite1*)LoadSuite(kAEGPEffectSuite, kAEGPEffectSuiteVersion1);
		}
		return i_effect_suiteP;
	}
	AEGP_MaskSuite1 *MaskSuite1() const
	{
		if (!i_mask_suiteP) {
			i_mask_suiteP = (AEGP_MaskSuite1*)LoadSuite(kAEGPMaskSuite, kAEGPMaskSuiteVersion1);
		}
		return i_mask_suiteP;
	}

	AEGP_MaskOutlineSuite1 *MaskOutlineSuite1() const
	{
		if (!i_mask_outline_suiteP) {
			i_mask_outline_suiteP = (AEGP_MaskOutlineSuite1*)LoadSuite(kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion1);
		}
		return i_mask_outline_suiteP;
	}

	AEGP_FootageSuite1 *FootageSuite1() const
	{
		if (!i_footage_suiteP) {
			i_footage_suiteP = (AEGP_FootageSuite1*)LoadSuite(kAEGPFootageSuite, kAEGPFootageSuiteVersion1);
		}
		return i_footage_suiteP;
	}

	AEGP_CommandSuite1 *CommandSuite1() const
	{
		if (!i_command_suiteP) {
			i_command_suiteP = (AEGP_CommandSuite1*)LoadSuite(kAEGPCommandSuite, kAEGPCommandSuiteVersion1);
		}
		return i_command_suiteP;
	}

	AEGP_UtilitySuite1 *UtilitySuite1() const
	{
		if (!i_utility_suiteP) {
			i_utility_suiteP = (AEGP_UtilitySuite1*)LoadSuite(kAEGPUtilitySuite, kAEGPUtilitySuiteVersion1);
		}
		return i_utility_suiteP;
	}

	PF_ANSICallbacksSuite1 *AnsiSuite1() const
	{
		if (!i_ansi_suiteP) {
			i_ansi_suiteP = (PF_ANSICallbacksSuite1*)LoadSuite(kPFANSISuite, kPFANSISuiteVersion1);
		}
		return i_ansi_suiteP;
	}

	PF_HandleSuite1 *HandleSuite1() const
	{
		if (!i_handle_suiteP) {
			i_handle_suiteP = (PF_HandleSuite1*)LoadSuite(kPFHandleSuite, kPFHandleSuiteVersion1);
		}
		return i_handle_suiteP;
	}

	PF_FillMatteSuite1 *FillMatteSuite1() const
	{
		if (!i_fill_matte_suiteP) {
			i_fill_matte_suiteP = (PF_FillMatteSuite1*) LoadSuite(kPFFillMatteSuite, kPFFillMatteSuiteVersion1);
		}
		return i_fill_matte_suiteP;
	}

	PF_WorldTransformSuite1 *WorldTransformSuite1() const
	{
		if (!i_world_transform_suiteP) {
			i_world_transform_suiteP = (PF_WorldTransformSuite1*) LoadSuite(kPFWorldTransformSuite, kPFWorldTransformSuiteVersion1);
		}
		return i_world_transform_suiteP;
	}

	AEGP_CompositeSuite1 *CompositeSuite1() const
	{
		if (!i_composite_suiteP) 
		{
			i_composite_suiteP = (AEGP_CompositeSuite1*) LoadSuite(kAEGPCompositeSuite, kAEGPCompositeSuiteVersion1);
		}
		return i_composite_suiteP;
	}

	PF_WorldSuite1 *PFWorldSuite1() const
	{
		if (!i_world_suiteP) 
		{
			i_world_suiteP = (PF_WorldSuite1*) 
				LoadSuite(kPFWorldSuite, kPFWorldSuiteVersion1);
		}
		return i_world_suiteP;
	}

	AEGP_IOInSuite1 *IOInSuite1() const
	{
		if (!i_io_in_suiteP) 
		{
			i_io_in_suiteP = (AEGP_IOInSuite1*) 
				LoadSuite(kAEGPIOInSuite, kAEGPIOInSuiteVersion1);
		}
		return i_io_in_suiteP;
	}
	
	AEGP_IOOutSuite1 *IOOutSuite1() const
	{
		if (!i_io_out_suiteP) 
		{
			i_io_out_suiteP = (AEGP_IOOutSuite1*) 
				LoadSuite(kAEGPIOOutSuite, kAEGPIOOutSuiteVersion1);
		}
		return i_io_out_suiteP;
	}

	AEGP_PersistentDataSuite1 *PersistentDataSuite1() const
	{
		if (!i_persistent_data_suiteP)
		{
			i_persistent_data_suiteP = (AEGP_PersistentDataSuite1*)
				LoadSuite(kAEGPPersistentDataSuite, kAEGPPersistentDataSuiteVersion1);
		}
		return i_persistent_data_suiteP;
	}

	AEGP_RenderQueueSuite1	*RenderQueueSuite1() const
	{
		if (!i_render_queue_suiteP)
		{
			i_render_queue_suiteP = (AEGP_RenderQueueSuite1*)
				LoadSuite(kAEGPRenderQueueSuite, kAEGPRenderQueueSuiteVersion1);
		}
		return i_render_queue_suiteP;
	}

	AEGP_FIMSuite1	*FIMSuite1() const
	{
		if (!i_fim_suiteP)
		{
			i_fim_suiteP = (AEGP_FIMSuite1*)
				LoadSuite(kAEGPFIMSuite, kAEGPFIMSuiteVersion1);
		}
		return i_fim_suiteP;
	}

	PF_Sampling8Suite1	*Sampling8Suite() const
	{
		if (!i_sampling_8_suiteP)
		{
			i_sampling_8_suiteP = (PF_Sampling8Suite1*)
				LoadSuite(kPFSampling8Suite, kPFSampling8SuiteVersion1);
		}
		return i_sampling_8_suiteP;
	}
	PF_Sampling16Suite1	*Sampling16Suite() const
	{
		if (!i_sampling_16_suiteP)
		{
			i_sampling_16_suiteP = (PF_Sampling16Suite1*)
				LoadSuite(kPFSampling16Suite, kPFSampling16SuiteVersion1);
		}
		return i_sampling_16_suiteP;
	}
	PF_Iterate8Suite1	*Iterate8Suite1() const
	{
		if (!i_iterate_8_suiteP)
		{
			i_iterate_8_suiteP	= (PF_Iterate8Suite1*)
				LoadSuite(kPFIterate8Suite, kPFIterate8SuiteVersion1);
		}
		return i_iterate_8_suiteP;
	}
	PF_iterate16Suite1	*Iterate16Suite1() const
	{
		if (!i_iterate_16_suiteP)
		{
			i_iterate_16_suiteP	= (PF_iterate16Suite1*)
				LoadSuite(kPFIterate16Suite, kPFIterate16SuiteVersion1);
		}
		return i_iterate_16_suiteP;
	}
	AEGP_CollectionSuite1	*CollectionSuite1() const
	{
		if (!i_collection_suiteP)
		{
			i_collection_suiteP	=	(AEGP_CollectionSuite1*)
				LoadSuite(kAEGPCollectionSuite, kAEGPCollectionSuiteVersion1);
		}
		return i_collection_suiteP;
	}
};

#endif