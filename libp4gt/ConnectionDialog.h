/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_CONNECTION_DIALOG
#define INC_CONNECTION_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"

/*
 * ConnectionDialog - show the Perforce Connection Information dialog box.
 */

class ConnectionDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * results - Results of doing a p4 info command.
	 */

							ConnectionDialog( PluginOperations *ops, PerforceResults *results );

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
	 * Get the results
	 */
	PerforceResults			*Results() { return results; };

	/* 
	 * Resuls of p4 info command 
	 */
	PerforceResults			*results;
};

#endif