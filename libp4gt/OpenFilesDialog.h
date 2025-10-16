/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_OPEN_FILES_DIALOG
#define INC_OPEN_FILES_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"

/*
 * OpenFilesDialog - Dialog showing list of checked out files.
 */
class OpenFilesDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
	 * results - Results of doing "p4 opened -c default" command
	 */
							OpenFilesDialog( PluginOperations *ops, PerforceResults *results );

	/*
	 * Get results of p4 opened command
	 */
	PerforceResults			*Results() { return results; };

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

	PerforceResults			*results;
};

#endif