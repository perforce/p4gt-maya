/* 
 *
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_TABBED_OPTIONS_DIALOG
#define INC_TABBED_OPTIONS_DIALOG

#include "Dialog.h"
#include "PluginOperations.h"

/*
 * TabbedOptionsDialog - Show Options dialog.
 */
class TabbedOptionsDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - Handle to ops object
	 * options - Options repository interface
	 * refs - Whether or not to show external references option.
	 */
						TabbedOptionsDialog( PluginOperations *ops, PerforceOptions *options,  bool refs );

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
	 * Windows procedure for tab panes
	 *
	 * hWnd - Handle of dialog window.
	 * uMsg - Windows message.
	 * wParam - Message wParam.
	 * lParam - Message lParam.
	 */
	static INT_PTR CALLBACK	TabWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 

	/*
	 * Init combo box options
	 *
	 * dialog - handle to dialog
	 * combo - ID of combo
	 * prompt - Whether or not to include a prompt option
	 */
	static void				InitCombo( HWND dialog, int combo, bool prompt );

	/*
	 * Get which radio button selected from a group of 3.
	 *
	 * radio1 - First radio button
	 * radio2 - Second radio button
	 * radio3 - Third radio button
	 */
	static int				GetRadioSelection( HWND radio1, HWND radio2, HWND radio3 );

	/*
	 * Check one of a group of 3 radio buttons depending on the value
	 *
	 * value - Value which decides the radio button to set
	 * radio1 - First radio button
	 * radio2 - Second radio button
	 * radio3 - Third radio button
	 */
	static void				SetRadioSelection( int value, HWND radio1, HWND radio2, HWND radio3 );

	/*
	 * Check/uncheck checkbox to reflect the value
	 *
	 * value - Value to represent in checkbox
	 * check - Handle to checkbox
	 */
	static void				SetCheckSelection( int value, HWND check );

	/*
	 * Gets whether or not checkbox has been checked.
	 *
	 * check - Handle to checkbox
	 */
	static int				GetCheckSelection( HWND check );

	/*
	 * Save state of options in the registry
	 *
	 * dialog - Handle to options dialog.
	 */
	static void				SaveOptions( HWND dialog );

	/*
	 * Return true if we should display external references option
	 */
	bool					HasRefs() {return refs;};

	PerforceOptions		*options;
	bool				refs;
	HWND				page1;
	HWND				page2;
};

#endif