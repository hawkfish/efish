/*
	File:		PhotoPrintCommands.h

	Contains:	Definitions of PowerPlant command IDs.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 jul 2001		dml		add msg_Units
		28 jun 2001		dml		add msg_3Hole
		27 Jun 2001		drd		103 msg_TextChanged
		25 Apr 2001		drd		cmd_Redraw
		23 Apr 2001		drd		msg_MaximumSize, msg_MinimumSize
		21 mar 2001		dml		add margin msgs
		13 mar 2001		dml		add cmd_ChangeFont
		02 feb 2001		dml		add cmd_DrawMaxBounds
		23 feb 2001		dml		add tool_name
		06 aug 2000		dml		cmd_FitInWindow, cmd_ViewFullSize, cmd_ZoomOnSelection
		24 aug 2000		dml		cmd_ZoomIn, cmd_ZoomOut
		21 Aug 2000		drd		cmd_Import
		18 Aug 2000		drd		cmd_MakeIcon
		14 Aug 2000		drd		cmd_ReLayout, cmd_Reveal
		11 Aug 2000		drd		Moved tool_ here, added tool_Rotate; cmd_RemoveCrop, cmd_RemoveRotation
		24 Jul 2000		drd		cmd_LayoutPalette, cmd_ToolsPalette
		14 Jun 2000		drd		First commands
*/

enum {
	// File menu
	cmd_Import				= 'impo',

	// Image menu
	cmd_ImageOptions		= 'iOpt',
	cmd_MakeIcon			= 'icon',
	cmd_RemoveCrop			= '-cro',
	cmd_RemoveRotation		= '-rot',
	cmd_Reveal				= 'reve',

	cmd_LayoutPalette		= 'shoP',
	cmd_ToolsPalette		= 'shoT',
	
	// View Menu
	cmd_BackgroundOptions	= 'bOpt',
	cmd_FitInWindow			= 'fitw',
	cmd_ViewFullSize		= 'full',
	cmd_ZoomOnSelection		= 'zmit',
	cmd_ZoomIn				= 'bigr',
	cmd_ZoomOut				= 'smlr',

	// Debug menu
	cmd_Redraw				= 'redr',
	cmd_ReLayout			= 'layo',

	// These guys are actually commands, too
	tool_Arrow = 'arro',
	tool_Crop = 'crop',
	tool_Rotate = 'rota',
	tool_Zoom = 'zoom',
	tool_Name = 'name',
	
	//
	cmd_UseProxies			= 'prxy',
	cmd_DrawMaxBounds		= 'maxB',
	
	//
	cmd_ChangeFont			= 'font',

	//
	msg_MinimalMargins 		= 'minm',
	msg_SymmetricMargins 	= 'symm',
	msg_CustomMargins		= 'cust',
	msg_3Hole				= '3hol',
	msg_Units				= 'unit',
	msg_TextChanged			= 'Ætxt',

	// Popups
	msg_MaximumSize			= 'maxS',
	msg_MinimumSize			= 'minS',

	cmd_FnordNord			='fnrd'
	
};
