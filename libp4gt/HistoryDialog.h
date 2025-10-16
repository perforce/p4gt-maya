/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_HISTORY_DIALOG
#define INC_HISTORY_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"
#include "PluginOperations.h"

/*
 * HistoryDialog - dialog box to show revision history of file.
 */

class HistoryDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - Plugin operations object.
	 * results - Results of doing a p4 filelog on the file.
	 */
						HistoryDialog( PluginOperations *ops, PerforceResults *results );

	/*
	 * Returns filelog results
	 */
	PerforceResults		*Results();

	/* 
	 * Create local copy of depot file. 
	 *
	 * version - The version of the depot file to get.
	 */
	void				CreateTempFile( int version );

	/* 
	 * Sync to specified revision of file. 
	 *
	 * version - The version of the depot file to sync to.
	 */
	void				SyncFile( int version );

	/*
	 * Returns the name of the temporay depot file
	 */
	StrBuf				*TempFile();

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

	/* 
	 * Setup list control
	 * 
	 * list - Handle of list control
	 */
	static void				SetupList( HWND list );

	/* 
	 * Insert history data into list
	 * 
	 * list - Handle of list control
	 */
	static void				InsertListData( HWND list,  UniStrDict *dict );

	/*
	 * Convert number of seconds since 1970 string into the following string:
	 * YYYY/MM/DD HH:MM:SS. Return value in a static buffer.
	 * 
	 * time - Time in seconds
	 */
	static TCHAR			*AsciiTime( StrPtr *time );

	/*
	 * Column comparsion function
	 */
	static int CALLBACK		Compare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

	/*
	 * Get the selected revision number
	 *
	 * list - Handle of list control
	 */
	static int				GetSelectedRevision( HWND list );

	PerforceResults			*results;
	UniStrBuf				tempFile;

	int						sortCol;
	int						sortOrder;

	int						revOrder;
	int						changeOrder;
	int						dateOrder;
	int						userOrder;
	int						actionOrder;
	int						descOrder;

	int						exitCode;
};

#endif