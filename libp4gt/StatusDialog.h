/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_STATUS_DIALOG
#define INC_STATUS_DIALOG

#include "Dialog.h"
#include "PerforceFileStat.h"
#include "PerforceAPI.h"

/*
 * StatusDialog - show the Document Status dialog box.
 */

class StatusDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * stat - The file status object.
	 */

							StatusDialog( PluginOperations *ops, PerforceFileStat *stat );

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
	 * Get the file status
	 */
	PerforceFileStat		*FileStatus() { return stat; };

	/* 
	 * The file status 
	 */
	PerforceFileStat		*stat;
};

#endif