/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_CHECK_IN_ADD_DIALOG
#define INC_CHECK_IN_ADD_DIALOG

#include "Dialog.h"
#include "PerforceOptions.h"

/*
 * CheckInAddDialog - dialog box to ask user if they want to check in
 * file after adding to Perforce.
 */

class CheckInAddDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
     * options - Interface to preference store
	 */

							CheckInAddDialog( PluginOperations *ops, PerforceOptions *options );

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
	 * Check if this dialog should appear again
	 *
	 * check - Handle of "do not show" check
	 */
	void					CheckHideOption( HWND check );

	/*
	 * Get the options interface
	 */
	PerforceOptions			*Options() { return options; };

	PerforceOptions			*options;
};

#endif