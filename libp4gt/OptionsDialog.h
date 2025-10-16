/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_OPTIONS_DIALOG
#define INC_OPTIONS_DIALOG

#include "Dialog.h"
#include "PerforceOptions.h"

/*
 * OptionsDialog - Show Options dialog.
 */
class OptionsDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
	 * options - Options repository interface
	 * refs - Whether or not to show external references option.
	 */
						OptionsDialog( PluginOperations *ops, PerforceOptions *options,  bool refs );

	/*
	 * Get options repository interface
	 */
	PerforceOptions		*Options();

private:

	/* 
	 * Dialog windows procedure
	 *
	 * hWnd - Handle of dialog window.
	 * uMsg - Windows message.
	 * wParam - Message wParam.
	 * lParam - Message lParam.
	 */
	static INT_PTR CALLBACK	DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	/*
	 * Init combo box options
	 *
	 * dialog - handle to dialog
	 * combo - ID of combo
	 * prompt - Whether or not to include a prompt option
	 */
	static void				InitCombo( HWND dialog, int combo, bool prompt );

	/*
	 * Set combo box selection
	 *
	 * dialog - handle to dialog
	 * combo - ID of combo
	 * i - Selection index
	 */
	static void				SetSelection( HWND dialog, int combo, UINT i );

	/*
	 * Get combo box selection
	 *
	 * dialog - handle to dialog
	 * combo - ID of combo
	 */
	static UINT				GetSelection( HWND dialog, int combo );

	/*
	 * Return true if we should display external references option
	 */
	bool					HasRefs() {return refs;};

	PerforceOptions		*options;
	bool				refs;
};

#endif
