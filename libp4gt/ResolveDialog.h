/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_RESOLVE_DIALOG
#define INC_RESOLVE_DIALOG

#include "Dialog.h"

/*
 * ResolveDialog - Shown when user attempts to submit old version of file.
 */

class ResolveDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 */

							ResolveDialog( PluginOperations *ops );

	/*
	 * Get the option the user selected.
	 */
	int						GetOption() {return option;}


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
	 * Set the options the user selected.
	 *
	 * option - Option the user selected.
	 */
	void					SetOption( int option ) {this->option = option;};

	int option;
};

#endif