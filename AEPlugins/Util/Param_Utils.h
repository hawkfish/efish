#ifndef H_PARAM_UTILS
#define H_PARAM_UTILS

// requires the explicit use of 'def' for the struct name


#define PF_ADD_COLOR(NAME, RED, GREEN, BLUE, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_COLOR; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.cd.value.red = (RED); \
		def.u.cd.value.green = (GREEN); \
		def.u.cd.value.blue = (BLUE); \
		def.u.cd.value.alpha = 255; \
		def.u.cd.dephault = def.u.cd.value; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_ARBITRARY(NAME, WIDTH, HEIGHT, FLAGS, DFLT, ID, REFCON)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ARBITRARY_DATA; \
		PF_STRCPY(def.name, (NAME) ); \
		def.ui_width = (WIDTH);\
	    def.ui_height = (HEIGHT);\
		def.ui_flags = (FLAGS); \
		def.u.arb_d.value = NULL;\
		def.u.arb_d.pad = 0;\
		def.u.arb_d.dephault = (DFLT); \
		def.uu.id = def.u.arb_d.id = (ID); \
		def.u.arb_d.refconPV = REFCON; \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_SLIDER; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.sd.value_str[0] = '\0'; \
		def.u.sd.value_desc[0] = '\0'; \
		def.u.sd.valid_min = (VALID_MIN); \
		def.u.sd.slider_min = (SLIDER_MIN); \
		def.u.sd.valid_max = (VALID_MAX); \
		def.u.sd.slider_max = (SLIDER_MAX); \
		def.u.sd.value = def.u.sd.dephault = (DFLT); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_FIXED(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_FIX_SLIDER; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.fd.value_str[0]	= '\0'; \
		def.u.fd.value_desc[0]	= '\0'; \
		def.u.fd.valid_min		= (VALID_MIN); \
		def.u.fd.slider_min		= (SLIDER_MIN); \
		def.u.fd.valid_max		= (VALID_MAX); \
		def.u.fd.slider_max		= (SLIDER_MAX); \
		def.u.fd.value			= def.u.fd.dephault	= (DFLT); \
		def.u.fd.precision		= (short)(PREC); \
		def.u.fd.display_flags  |= (short)(DISP); \
		def.flags				|= (FLAGS); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_FLOAT_SLIDER; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.fs_d.valid_min		= (VALID_MIN); \
		def.u.fs_d.slider_min		= (SLIDER_MIN); \
		def.u.fs_d.valid_max		= (VALID_MAX); \
		def.u.fs_d.slider_max		= (SLIDER_MAX); \
		def.u.fs_d.value			= (DFLT); \
		def.u.fs_d.dephault			= (DFLT); \
		def.u.fs_d.precision		= (PREC); \
		def.u.fs_d.display_flags	= (DISP); \
		def.u.fs_d.fs_flags			|= (WANT_PHASE) ? PF_FSliderFlag_WANT_PHASE : 0; \
		def.u.fs_d.curve_tolerance	= AEFX_AUDIO_DEFAULT_CURVE_TOLERANCE;\
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_CHECKBOX(NAME_A, NAME_B, DFLT, DISP, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_CHECKBOX; \
		PF_STRCPY(def.name, NAME_A); \
		def.u.bd.u.nameptr  = (NAME_B); \
		def.u.bd.value		= (DFLT); \
		def.u.bd.dephault	= (DFLT); \
		def.flags			|= (DISP); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_ANGLE(NAME, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ANGLE; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.ad.value = def.u.ad.dephault = (DFLT) * 65536.0; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)


#define PF_ADD_NULL(NAME, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_NO_DATA; \
		PF_STRCPY(def.name, (NAME) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)


#define PF_ADD_POPUP(NAME, CHOICES, DFLT, STRING, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POPUP; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.pd.num_choices = (CHOICES); \
		def.u.pd.dephault = (DFLT); \
		def.u.pd.value = (DFLT); \
		def.u.pd.u.namesptr = (STRING); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
	
#define PF_ADD_LAYER(NAME, DFLT, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_LAYER; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.pd.dephault = (DFLT); \
		def.uu.id = ID;\
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)			

#define PF_ADD_255_SLIDER(NAME, DFLT, ID)\
	PF_ADD_SLIDER( (NAME), 0, 255, 0, 255, (DFLT), (ID))

#define PF_ADD_PERCENT(NAME, DFLT, ID)\
	PF_ADD_FIXED( (NAME), 0, 100, 0, 100, (DFLT), 1, 1, 0, (ID))

#define PF_ADD_POINT(NAME, X_DFLT, Y_DFLT, RESTRICT_BOUNDS, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POINT; \
		PF_STRCPY(def.name, (NAME) ); \
		def.u.td.restrict_bounds = RESTRICT_BOUNDS;\
		def.u.td.x_value = def.u.td.x_dephault = (X_DFLT); \
		def.u.td.y_value = def.u.td.y_dephault = (Y_DFLT); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_TOPIC(NAME, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_START; \
		PF_STRCPY(def.name, (NAME) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_END_TOPIC(ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_END; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)	


#endif // H_PARAM_UTILS