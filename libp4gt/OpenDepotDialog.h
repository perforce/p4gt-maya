/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_OPEN_DEPOT_DIALOG
#define INC_OPEN_DEPOT_DIALOG

#include <windows.h>
#include <commctrl.h>

#include "Dialog.h"
#include "PerforceConnection.h"

/*
 * Button identifiers
 */ 
#define IDM_LARGEICON		48000
#define IDM_SMALLICON		48001
#define IDM_LISTVIEW		48002
#define IDM_REPORTVIEW		48003
#define IDM_PARENTFOLDER	48004

/*
 * Toolbar identifiers
 */
#define IDC_FOLDER_TOOLBAR	48010
#define IDC_LIST_TOOLBAR	48011

/* 
 * Toolbar button size
 */
#define TB_BUTTON_WIDTH		24
#define TB_BUTTON_HEIGHT	22

/*
 * Max length of file type string
 */
#define MAX_TYPE_SIZE		256

/*
 * OpenDepotDialog - Show the "Open From Perforce" depot browser dialog.
 */
class OpenDepotDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
	 * options - Interface to options store.
	 * filetypes - An array of file types.
	 */
							OpenDepotDialog( PluginOperations *ops, PerforceOptions *options, StrBufArray *filetypes );

	/*
	 * Destructor.
	 */
							~OpenDepotDialog();

	/*
	 * Append a folder to the current path
	 *
	 * folder - The name of the folder.
	 */
	void					PushFolder( TCHAR *folder );

	/*
	 * Remove the last folder from the current path
	 */
	void					PopFolder();

	/*
	 * Display all files/folders in current folder
	 *
	 * list - Handle to files list.
	 * fileTypes - Handle to file types combo.
	 * refresh - If true get new file/dir info from server.
	 */
	void					ShowCurrFolder( HWND list,  HWND fileTypes, bool refresh = true );

	/*
	 * Initialise the first directory to display
	 *
	 * combo - Handle of Look In combo.
	 */
	void					InitCurrDir( HWND combo );

	/*
	 * Select a depot
	 *
	 * combo - Handle of Look In combo.
	 * idx - Index of depot in combo.
	 */
	void					SelectDepot( HWND combo, long idx );

	/*
	 * Set the current depot
	 *
	 * name - The name of the depot
	 */
	void					SetCurrDepot( TCHAR *name );

	/*
	 * Set the name of the selected file.
	 *
	 * name - name of selected file.
	 */
	void					SetSelectedFile( TCHAR *name );

	/* 
	 * Get the depot path of the selected file.
	 */
	StrPtr*					SelectedFile();

	/*
	 * Setup image list for Look In combo.
	 *
     * list - Handle of list control.
	 */
	void					SetupLookInImageLists( HWND combo );

	/*
	 * Create toolbar showing file list styles
	 *
	 * dialog - Handle to dialog
	 * combo - Handle to combo to place toolbar next to
	 */
	void					CreateToolbar( HWND dialog, HWND combo );

	/*
	 * Show toolbar tooltips
	 */
	void					Tooltips( LPNMTBGETINFOTIP tooltipEvent );

	/*
	 * Select a folder or depot from the Look In combo.
	 * Removes all folders beneath the selected folder.
	 * Returns name of selected folder or depot.
	 *
	 * combo - Handle of combo.
	 * selected - Index of the selected depot/folder.
	 */
	void					SelectLookInFolder( HWND combo, int selected ); 

	

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
	 * Setup columns in Report view.
	 * 
	 * list - Handle of list control
	 */
	static void				SetupReportColumns( HWND list );

	/*
	 * Add depot or folder to Look In combo
	 *
	 * combo - Handle of combo.
	 * pos - Position of new item.
	 * text - Name of new item.
	 * indent - Indent of new item.
	 */
	static void				AddLookInItem( HWND combo, int pos, TCHAR *text, int indent );

	/*
	 * Load file types combo,
	 *
	 * combo - Handle of combo.
	 */
	void					InitFileTypes( HWND combo );

	/*
	 * Check if filename matches current pattern
	 *
	 * combo - Handle of combo containing file types.
	 * filename - Filename to match against.
	 */
	static bool				MatchFileType( HWND combo, TCHAR *filename );

	/*
	 * Check if string matches expression containing wildcards '*' and '?'
	 *
	 * wild - Expression containing wildcards.
	 * string - String to match against wildcard expression. 
	 */
	static int				WildCmp( TCHAR *wild, TCHAR *string );

	/*
	 * Insert a new folder into the list
	 *
	 * list - Handle of list.
	 * pos - Position of new folder.
	 * dict - Dictionary containing folder info.
	 */
	void					InsertFolder( HWND list, int pos, UniStrDict *dict );

	/*
	 * Return true if we are in the root folder i.e the level in the
	 * depot.
	 */
	bool					InRootFolder();


	/*
	 * Insert a new file into the list
	 *
	 * list - Handle of list.
	 * pos - Position of new file.
	 * dict - Dictionary containing folder info.
	 */
	void					InsertFile( HWND list, int pos, UniStrDict *dict );

	/*
	 * Return whether a dictionary contains a folder or file
	 *
	 * dict - Dictionary for folder or file
	 */
	static bool				IsFolder( UniStrDict *dict ); 

	/*
	 * Column comparsion function
	 */
	static int CALLBACK		Compare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

	/*
	 * Save current dialog settings
	 *
	 * hWnd - dialog handle
	 */
	void					SaveSettings( HWND hWnd );

	/*
	 * Open a new directory
	 *
	 * combo - Handle to Look In combo
	 * dir - Name of directory to open
	 */
	void						OpenDir( HWND combo, TCHAR *dir );

	/*
	 * wndproc for list view. Intercepts ENTER key on folders
	 */
	static LRESULT APIENTRY		ListViewEnterHandler( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	/*
	 * Enter a new folder
	 *
	 * hWnd - Handle of list view
	 * path - Name of folder to open
	 */
	static void					EnterFolder( HWND hWnd, TCHAR *path ); 

	/*
	 * Perforce connection to use for commands
	 */
	PerforceConnection		*con;

	/*
	 * Current directory.
	 */
	UniStrBuf				currDir;

	/* 
	 * Name of currently selected file
	 */
	UniStrBuf				selectedFile;

	/*
	 * The image list for the Look In combo.
	 */
	HIMAGELIST				lookInImages;

	/*
	 * Up folder toolbar handle.
	 */
	HWND					toolbar1;

	/*
	 * View type toolbar handle.
	 */
	HWND					toolbar2;

	/*
	 * Holds results from p4 dirs
	 */
	PerforceResults			dirResults;

	/*
	 * Holds results from p4 fstat
	 */
	PerforceResults			fileResults;

	PerforceOptions			*options;

	int						sortCol;
	int						sortOrder;

	int						nameOrder;
	int						haveOrder;
	int						headOrder;
	int						typeOrder;

	StrBufArray				*filetypes;
};

#endif
