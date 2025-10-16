/*
 * Copyright 2004-2022 Perforce Software.  All rights reserved.
 *
 * Original by Data Shades Ltd.
 */
#ifndef INC_ABOUT_DIALOG
#define INC_ABOUT_DIALOG

#include "Dialog.h"
#include "UniStrBuf.h"
#include "strsafe.h"

/*
 * AboutDialog - show the About dialog box.
 */

class AboutDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * name - Plugin name.
	 * pluginVer - Version of plugin.
	 * appVer - Version of host application.
	 */

							AboutDialog( PluginOperations *ops, WCHAR *name, WCHAR *pluginVer, WCHAR *appVer );

	/*
	 * Get host application version
	 */
	WCHAR					*AppVersion() { return appVersion; };

	/*
	 * Get name of plugin
	 */
	WCHAR					*PluginName() { return pluginName; }; 

	/*
	 * Get plugin version
	 */
	WCHAR					*PluginVersion() { return pluginVersion; };


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
	 * Get the windows version string. Returns TRUE if successful
	 *
	 * buff - buffer to return version string
	 */
	bool					WindowsVersion( UniStrBuf &buff ); // TODO Use wstring.

	/* 
	 * Name of plugin 
	 */
	WCHAR					*pluginName;

	/* 
	 * Version of plugin 
	 */
	WCHAR					*pluginVersion;

	/* 
	 * Version of host application 
	 */
	WCHAR					*appVersion;
};

#endif