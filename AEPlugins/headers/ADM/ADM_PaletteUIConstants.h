#ifndef	_H_ADM_PALETTE_UI_CONSTANTS
#define	_H_ADM_PALETTE_UI_CONSTANTS

#define	ADM_PALETTE_LEFT_OFFSET		5
#define	ADM_PALETTE_TOP_OFFSET		5
#define	ADM_PALETTE_RBN_HEIGHT		12
#define	ADM_PALETTE_RBN_OFFSET		20
#define	ADM_PALETTE_POPUP_OFFSET	40
#define	ADM_PALETTE_BTN_HEIGHT		16
#define	ADM_PALETTE_POPUP_HEIGHT	20
#define	ADM_PALETTE_TXT_HEIGHT		12
#define	ADM_PALETTE_EDIT_HEIGHT		16
#define	ADM_PALETTE_CBX_OFFSET		20
#define	ADM_PALETTE_CBX_HEIGHT		12
#define	ADM_PALETTE_TXT_OFFSET		3


#define	ADM_PALETTE_DIVIDER_HEIGHT	2

typedef	struct	{
	long	left_offsetL;
	long	top_offsetL;
	long	rbn_heightL;
	long	rbn_offsetL;
	long	popup_offsetL;
	long	popup_heightL;
	long	btn_heightL;
	long	txt_heightL;
	long	txt_offsetL;
	long	edit_heightL;
	long	cbx_offsetL;
	long	cbx_heightL;
	long	palette_divider_heightL;
}	Dlg_layout_params;

#endif