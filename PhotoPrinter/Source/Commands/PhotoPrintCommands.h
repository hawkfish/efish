/*
	File:		PhotoPrintCommands.h

	Contains:	Definitions of PowerPlant command IDs.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 Aug 2000		drd		Moved tool_ here, added tool_Rotate
		24 Jul 2000		drd		cmd_LayoutPalette, cmd_ToolsPalette
		14 Jun 2000		drd		First commands
*/

enum {
	// Options menu
	cmd_BackgroundOptions	= 'bOpt',
	cmd_ImageOptions		= 'iOpt',

	cmd_LayoutPalette		= 'shoP',
	cmd_ToolsPalette		= 'shoT',

	// These guys are actually commands, too
	tool_Arrow = 'arro',
	tool_Crop = 'crop',
	tool_Rotate = 'rota',
	tool_Zoom = 'zoom'
};
