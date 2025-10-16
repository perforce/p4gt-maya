/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PLUGIN_OPERATIONS
#define INC_PLUGIN_OPERATIONS

#include <windows.h>
#include <exception>
using namespace std;

#include "PerforceResults.h"
#include "PerforceConnection.h"
#include "PerforceFileStat.h"
#include "PerforceOptions.h"
#include "StrBufArray.h"
#include "MenuState.h"
#include "PluginHost.h"
#include "UniStrBuf.h"

class PerforceConnection;

/*
 * Provides services and UI for clients to interact with Perforce.
 * Also does error handling.
 */
class PluginOperations {

public:

	/*
	 * Constructor.
	 *
	 * instance - Instance of plugin.
	 * parent - Handle of host application.
	 */
						PluginOperations( PluginHost *host );

	/*
	 * Destructor.
	 */
						~PluginOperations();


	/*
	 * Toggle server online state
	 */
	void				ToggleOnline();

	/*
	 * Return object which describes the state of the menu.
	 */
	MenuState			*GetMenuState();

	/*
	 * Call this when a new file is opened in the host application
	 */
	void				FileOpenEvent();

	/*
	 * Call this when a file is closed in the host application
	 */
	void				FileCloseEvent();

	/*
	 * Do a p4 fstat on the request file. Returns true if file is under
	 * Perforce otherwise false.
	 *
	 * filename - The name of the file to stat.
	 * stat - The file status information is returned here.
	 * results - The buffer to return the results of the stat or NULL.
	 */
	bool				Stat( TCHAR *filename, PerforceFileStat *stat, PerforceResults *results = NULL );

	/*
	 * Show About dialog box.
	 */
	void				ShowAbout();

	/*
	 * Show "Open from Perforce" dialog.
	 *
	 * filetypes - An array of file type strings.
	 */
	void				ShowOpenDepot( StrBufArray *filetypes );

	/*
	 * Show Perforce Information dialog box.
	 */
	void				ShowInfo();

	/*
	 * Show Options dialog box.
	 */
	void				ShowOptions();

	/*
	 * Show the online help.
	 */
	void				ShowHelp();

	/*
	 * Display dialog showing open files.
	 */
	void				ShowOpenFiles();

	/*
	 * Show file status information
	 */
	void				ShowStatus();

	/*
	 * Show dialog that says user needs to save file before
	 * adding to Perforce.
	 */
	void				NeedSaveDialog();

	/*
	 * Display dialog showing file history.
	 *
	 */
	void				ShowHistory();

	/*
	 * Sync external referenced files
	 *
	 * files - List of referenced files.
	 */
	void				SyncReferences( StrBufArray *files );

	/*
	 * p4 sync the named file.
	 *
	 * filename - The name of the file to sync.
	 * show - Whether or not to display a dialog box.
	 */
	void				Sync( TCHAR *filename, bool show=true );

	/*
	 * p4 add the current file.
	 */
	void				Add();

	/*
	 * p4 edit the current file.
	 *
	 */
	void				Edit();

	/*
	 * p4 revert the current file.
	 *
	 */
	void				Revert();


	/*
	 * Lock file if unlocked and visa versa
	 */
	void				ToggleLock();

	/*
	 * p4 submit the current file.
	 *
	 */
	void				Submit();


	/*
	 * Show results of last command.
	 */
	void				ShowLastResults();

	/*
	 * Get the host plugin
	 */
	PluginHost			*GetHost() { return host; };

	/*
	 * Get stored password
	 */
	StrPtr				&GetPassword() { return password; };

	/*
	 * Set stored password
	 *
	 * password - the password to set
	 */
	void				SetPassword( TCHAR *password );

	/*
	 * Call this if server supports login/logout
	 */
	void				SetUseLogin() { login = true; };

	/*
	 * Returns whether or not server supports login, logout
	 */
	bool				GetUseLogin() { return login; };

	/*
	 * Call this if we have got connection info
	 */
	void				SetGotConnectionInfo() { gotInfo = true; };

	/*
	 * Returns whether or not we have already retrieved connection info
	 */
	bool				GotConnectionInfo() { return login; };

	/*
	 * Return name of current file
	 */
	StrPtr				*GetCurrFile() { return &currFile; };

	/*
	 * Set the current directory to the one containing the working file
	 */
	void				SetFileDir();

	/*
	 * Restore current directory to what it was before plugin was called
	 */
	void				RestoreOldDir();

	/*
	 * Check whether or not server is online
	 */
	BOOL				IsOnline() {return online;}

	/*
	 * Call this if server supports extended submit options
	 */
	void				SetExtSubmit() { extendedSubmit = true; };

	/*
	 * Returns whether or not server supports extended submit options
	 */
	bool				GetExtSubmit() { return extendedSubmit; };

private:

	/*
	 * Get the depot path for a file. Returns true if the depot path
	 * was retrieved.
	 *
	 * con - The perforce connection to use.
	 * filename - The name of the file to get the depot file for.
	 * buff - The depot path is returned in this buffer.
	 */
	bool				GetDepotPath( PerforceConnection& con, TCHAR* filename, StrBuf& buff );

	/*
	 * Do a specific operation such as add, edit, lock on a file.
	 * Return true if op was successful otherwise false.
	 *
	 *
	 * op - The operation i.e "add", "edit" ...
	 * filename - The name of the file.
	 * show - True if need to show the result of the op in a dialog box.
	 * res - Store the results of the operation here
	 */
	bool				DoFileOp( TCHAR *op, TCHAR *filename, bool show = true, PerforceResults *res = NULL);

	/*
	 * Show dialog box displaying error message.
	 *
	 * err - The error.
	 */
	void				DisplayError( exception &err );

	/*
	 * Check for attempt to add file outside client root.
	 * Return client root directory if true otherwise NULL
	 *
	 *
	 * op - The operation i.e add, edit etc
	 * results - The results of the operation
	 */
	TCHAR				*AddOutsideRoot( TCHAR *op, PerforceResults &results );

	/*
	 * Check for attempt to add file outside client view.
	 * Return true if attempted to add outside view
	 *
	 * op - The operation i.e add, edit etc
	 * results - The results of the operation
	 */
	bool				AddOutsideView( TCHAR *op, PerforceResults &results );

	/*
	 * Extract filename from full path.
	 * Return just filename element of path
	 *
	 * path - the full file path
	 */
	TCHAR				*GetFilename( TCHAR *path );

	/*
	 * Show file contents for the user to view.
	 *
	 * HWND - Handle of host
	 * path - the full file path
	 */
	bool                ShowFile( HWND hwndCaller, TCHAR *path );

	PerforceResults		results;
	PluginHost			*host;
	PerforceFileStat	stat;
	PerforceOptions		*options;
	UniStrBuf			password;
	bool				login;
	bool				gotInfo;
	UniStrBuf			currFile;
	TCHAR				oldDir[MAX_PATH];
	BOOL				online;
	bool				extendedSubmit;
};

#endif
