/** AEGP_SuiteHandler.cpp

	Implementation of AEGP_SuiteHandler non-inlines. See AEGP_SuiteHandler.h for usage.

	Copyright (c) 2000 Adobe Systems Inc.

	created 9/11/2000 jms
**/

#include <AEGP_SuiteHandler.h>

AEGP_SuiteHandler::AEGP_SuiteHandler(const SPBasicSuite *pica_basicP) :
	i_pica_basicP(pica_basicP),
	i_comp_suiteP(0),
	i_proj_suiteP(0),
	i_layer_suiteP(0),
	i_stream_suiteP(0),
	i_keyframe_suiteP(0),
	i_canvas_suiteP(0),
	i_camera_suiteP(0),
	i_register_suiteP(0),
	i_memory_suiteP(0),
	i_item_suiteP(0),
	i_light_suiteP(0),
	i_effect_suiteP( 0 ),
	i_mask_suiteP( 0 ),
	i_mask_outline_suiteP( 0 ),
	i_footage_suiteP( 0 ),
	i_pf_interface_suiteP(0),
	i_path_query_suiteP(0),
	i_path_data_suiteP(0),
	i_param_utils_suiteP(0),
	i_app_suiteP(0),
	i_command_suiteP(0),
	i_utility_suiteP(0),
	i_ansi_suiteP(0),
	i_handle_suiteP(0),
	i_fill_matte_suiteP(0),
	i_world_transform_suiteP(0),
	i_composite_suiteP(0),
	i_world_suiteP(0),
	i_io_in_suiteP(0),
	i_io_out_suiteP(0),
	i_persistent_data_suiteP(0),
	i_render_queue_suiteP(0),
	i_fim_suiteP(0),
	i_sampling_8_suiteP(0),
	i_sampling_16_suiteP(0),
	i_iterate_8_suiteP(0),
	i_iterate_16_suiteP(0),
	i_collection_suiteP(0)
{
	if (!i_pica_basicP) {						//can't construct w/out basic suite, everything else is demand loaded
		MissingSuiteError();
	}
}

AEGP_SuiteHandler::~AEGP_SuiteHandler()
{
	ReleaseAllSuites();
}

void *AEGP_SuiteHandler::LoadSuite(char *nameZ, long versionL) const
{
	const void *suiteP;
	long err = i_pica_basicP->AcquireSuite(nameZ, versionL, &suiteP);

	if (err || !suiteP) {
		MissingSuiteError();
	}

	return (void*)suiteP;
}

// Free a particular suite. Ignore errors, because, what is there to be done if release fails?
void AEGP_SuiteHandler::ReleaseSuite(char *nameZ, long versionL)
{
	i_pica_basicP->ReleaseSuite(nameZ, versionL);
}

// Free any outstanding suites.
void AEGP_SuiteHandler::ReleaseAllSuites()
{
	if (i_layer_suiteP) {
		ReleaseSuite(kAEGPLayerSuite, kAEGPLayerSuiteVersion1);
	}
	if (i_stream_suiteP) {
		ReleaseSuite(kAEGPStreamSuite, kAEGPStreamSuiteVersion1);
	}
	if (i_keyframe_suiteP) {
		ReleaseSuite(kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion1);
	}
	if (i_comp_suiteP) {
		ReleaseSuite(kAEGPCompSuite, kAEGPCompSuiteVersion1);
	}
	if (i_proj_suiteP) {
		ReleaseSuite(kAEGPProjSuite, kAEGPProjSuiteVersion1);
	}
	if (i_canvas_suiteP) {
		ReleaseSuite(kAEGPCanvasSuite, kAEGPCanvasSuiteVersion1);
	}
	if (i_camera_suiteP) {
		ReleaseSuite(kAEGPCameraSuite, kAEGPCameraSuiteVersion1);
	}
	if (i_register_suiteP) {
		ReleaseSuite(kAEGPRegisterSuite, kAEGPRegisterSuiteVersion1);
	}
	if (i_memory_suiteP) {
		ReleaseSuite(kAEGPMemorySuite, kAEGPMemorySuiteVersion1);
	}
	if (i_item_suiteP) {
		ReleaseSuite(kAEGPItemSuite, kAEGPItemSuiteVersion1);
	}
	if (i_pf_interface_suiteP) {
		ReleaseSuite(kAEGPPFInterfaceSuite, kAEGPPFInterfaceSuiteVersion1);
	}
	if (i_path_query_suiteP) {
		ReleaseSuite(kPFPathQuerySuite, kPFPathQuerySuiteVersion1);
	}
	if (i_path_data_suiteP) {
		ReleaseSuite(kPFPathDataSuite, kPFPathDataSuiteVersion1);
	}
	if (i_param_utils_suiteP) {
		ReleaseSuite(kPFPathDataSuite, kPFPathDataSuiteVersion1);
	}
	if (i_app_suiteP) {
		ReleaseSuite(kPFAppSuite, kPFAppSuiteVersion1);
	}
	if (i_light_suiteP) {
		ReleaseSuite(kAEGPLightSuite, kAEGPLightSuiteVersion1);
	}
	if (i_effect_suiteP) {
		ReleaseSuite(kAEGPEffectSuite, kAEGPEffectSuiteVersion1);
	}
	if (i_mask_suiteP) {
		ReleaseSuite(kAEGPMaskSuite, kAEGPMaskSuiteVersion1);
	}
	if (i_mask_outline_suiteP) {
		ReleaseSuite(kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion1);
	}
	if (i_footage_suiteP) {
		ReleaseSuite(kAEGPFootageSuite, kAEGPFootageSuiteVersion1);
	}
	if (i_command_suiteP) {
		ReleaseSuite(kAEGPCommandSuite, kAEGPCommandSuiteVersion1);
	}
	if (i_utility_suiteP) {
		ReleaseSuite(kAEGPUtilitySuite, kAEGPUtilitySuiteVersion1);
	}
	if (i_ansi_suiteP) {
		ReleaseSuite(kPFANSISuite, kPFANSISuiteVersion1);
	}
	if (i_handle_suiteP) {
		ReleaseSuite(kPFHandleSuite, kPFHandleSuiteVersion1);
	}
	if (i_fill_matte_suiteP) {
		ReleaseSuite(kPFFillMatteSuite, kPFFillMatteSuiteVersion1);
	}
	if (i_world_transform_suiteP) {
		ReleaseSuite(kPFWorldTransformSuite, kPFWorldTransformSuiteVersion1);
	}
	if (i_composite_suiteP) {
		ReleaseSuite(kAEGPCompositeSuite, kAEGPCompositeSuiteVersion1);
	}
	if (i_world_suiteP) {
		ReleaseSuite(kPFWorldSuite, kPFWorldSuiteVersion1);
	}
	if (i_io_in_suiteP) {
		ReleaseSuite(kAEGPIOInSuite, kAEGPIOInSuiteVersion1);
	}
	if (i_io_out_suiteP) {
		ReleaseSuite(kAEGPIOOutSuite, kAEGPIOOutSuiteVersion1);
	}
	if (i_render_queue_suiteP) {
		ReleaseSuite(kAEGPRenderQueueSuite, kAEGPRenderQueueSuiteVersion1);
	}
	if (i_fim_suiteP) {
		ReleaseSuite(kAEGPFIMSuite, kAEGPFIMSuiteVersion1);
	}
	if (i_sampling_8_suiteP) {
		ReleaseSuite(kPFSampling8Suite, kPFSampling8SuiteVersion1);
	}
	if (i_sampling_16_suiteP) {
		ReleaseSuite(kPFSampling16Suite, kPFSampling16SuiteVersion1);
	}
	if (i_iterate_8_suiteP)	{
		ReleaseSuite(kPFIterate8Suite, kPFIterate8SuiteVersion1);
	}
	if (i_iterate_16_suiteP)	{
		ReleaseSuite(kPFIterate16Suite, kPFIterate16SuiteVersion1);
	}
	if (i_collection_suiteP)	{
		ReleaseSuite(kAEGPCollectionSuite, kAEGPCollectionSuiteVersion1);
	}
}

