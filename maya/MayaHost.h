/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PluginHost.h"

/*
 * Allows plugin library to request host information
 */
class MayaHost : public PluginHost {

public:

					MayaHost ( HINSTANCE instance, HWND win ) : PluginHost( instance, win ) {}

	/*
	 * Return name of plugin
	 */
	TCHAR			*GetName();

	/*
	 * Return full description of plugin for About box.
	 */
	TCHAR			*GetDesc();

	/*
	 * Return version of plugin
	 */
	TCHAR			*GetVersion();

	/*
	 * Return version of host application.
	 */
	TCHAR			*GetHostVersion();

	/*
	 * Returns true if host supports external references.
	 */
	bool			HasRefs() {return true;};

	/*
	 * Return number of documents open.
	 */
	int				GetNumDocs();

	/*
	 * Get the full path to the current document.
	 * Return true if OK, return false if there is no document path
	 * i.e. The document is new and has not been saved.
	 *
	 * buff - Buffer in which to return path
	 */
	bool			GetCurrDocPath( UniStrBuf &buff );

	/*
	 * Returns true if the current document has unsaved changes.
	 */
	bool			IsCurrDocDirty();

	/*
	 * Save the current document.
	 */
	void			SaveCurrDoc();

	/*
	 * Load a new file from disk
	 */
	void			LoadDoc( StrPtr *path );
	void			LoadDoc( LPTSTR *path ) {}

	/*
	 * Reload the current document from disk.
	 */
	void			ReloadCurrDoc();

	/*
	 * Return true if host application has support for multiple documents
	 */
	bool			HasMultiDocs();

private:

	UniStrBuf currDocPath;
};