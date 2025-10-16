/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_OLD_VERSION_DIALOG
#define INC_OLD_VERSION_DIALOG

#include <windows.h>

#include "Dialog.h"
#include "PerforceApi.h"

/*
 * OldVersionDialog - This dialog is shown when user opens an out of date
 * version of a file.
 */

class OldVersionDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * filename - The name of the file.
	 * have - The version of the file in the client workspace.
	 * head - The latest version in the depot.
	 */
				OldVersionDialog( PluginOperations *ops, TCHAR *filename, StrPtr *have, StrPtr *head );

	/*
	 * Get the name of the file.
	 */
	TCHAR		*Filename();

	/*
	 * Get the version in the client workspace.
	 */
	StrPtr		*HaveRevision();

	/*
	 * Get the latest version in the depot.
	 */
	StrPtr		*HeadRevision();

	/*
	 * Set which button the user presses to exit the dialog.
	 */
	void		SetExitCode( int code) ;

	/*
	 * Get which button the user presses to exit the dialog.
	 */
	int			ExitCode();

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

	TCHAR		*filename;
	StrPtr		*have;
	StrPtr		*head;
	int			exitCode;
};

#endif