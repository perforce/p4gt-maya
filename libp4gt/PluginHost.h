/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

/*
 * Provides host application information and callbacks.
 * Each type of plugin needs to provide an implementation of this class.
 */

#ifndef INC_PLUGIN_HOST
#define INC_PLUGIN_HOST

#include <windows.h>

#include "PerforceAPI.h"
#include "UniStrBuf.h"

// P4GT version number
extern TCHAR *P4GT_VERSION;

// Max size of host version
#define MAX_HOST_VERSION	256

class PluginHost {

public:

	/*
	 * Constructor
	 *
	 * name - Name of plugin.
	 */
	PluginHost( HINSTANCE instance, HWND win ) : reloading( false ) {this->instance = instance; this->win = win;};


	/*
	 * Return name of plugin
	 */
	virtual TCHAR			*GetName() = 0;

	/*
	 * Return version of plugin
	 */
	virtual TCHAR			*GetVersion() = 0;

	/*
	 * Return full description of plugin for About box.
	 */
	virtual TCHAR			*GetDesc() = 0;

	/*
	 * Return version of host application.
	 */
	virtual TCHAR			*GetHostVersion() = 0;

	/*
	 * Return handle to main window of host application
	 */
	HWND					GetWin() { return win; };

	/*
	 * Return handle to the instance of the plugin.
	 */
	HINSTANCE				GetInstance() { return instance; };

	/*
	 * Return number of documents open.
	 */
	virtual int				GetNumDocs() = 0;

	/*
	 * Get the full path to the current document.
	 * Return true if OK, return false if there is no document path
	 * i.e. The document is new and has not been saved.
	 *
	 * buff - Buffer in which to return path
	 */
	virtual bool			GetCurrDocPath( UniStrBuf &buff ) = 0;


	/*
	 * Returns true if the current document has unsaved changes.
	 */
	virtual bool			IsCurrDocDirty() = 0;

	/*
	 * Save the current document.
	 */
	virtual void			SaveCurrDoc() = 0;

	/*
	 * Reload the current document from disk.
	 */
	virtual void			ReloadCurrDoc() = 0;

	/*
	 * Load a new file from disk
	 */
	virtual void			LoadDoc( StrPtr *path ) = 0;
	virtual void			LoadDoc( LPTSTR *path ) = 0;

	/*
	 * Returns true if host supports external references.
	 */
	virtual bool			HasRefs() = 0;

	/*
	 * Return true if current document is reloading
	 */
	bool					IsReloading() {return reloading;};

	/*
	 * Return true if host application has support for multiple documents
	 */
	virtual bool			HasMultiDocs() = 0;

	/*
	 * Set reloading status
	 */
	void					SetReloading( bool reloading ) {this->reloading = reloading;};


private:

	/*
  	 * Handle to main window
  	 */
  	HWND					win;

  	/*
  	 * Handle to plugin instance
  	 */
  	HINSTANCE				instance;

	/*
	 * true if we are reloading the file
	 */
	bool					reloading;
};

#endif
