/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "OpenDepotDialog.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

// Store orginal wndproc of list view
static WNDPROC origListViewProc;

static void GetItemInfo( HWND fileList, int idx, UniStrDict **dict = NULL, TCHAR *buff = NULL, BOOL *selected = NULL )
{
	LVITEM item;
	item.iItem = idx;
	item.iSubItem = 0;
	item.mask = 0;
	if ( buff != NULL ) {
		item.mask |= LVIF_TEXT;
		item.pszText = buff;
		item.cchTextMax = MAX_PATH;
	}
	if ( dict != NULL ) {
		item.mask |= LVIF_PARAM;
	}
	if ( selected != NULL ) {
		item.mask |= LVIF_STATE;
		item.stateMask = LVIS_SELECTED;
	}
	ListView_GetItem( fileList, &item );
	if ( dict != NULL ) {
		*dict = (UniStrDict *)item.lParam;
	}
	if ( selected != NULL ) {
		*selected = item.state & LVIS_SELECTED;
	}
}

OpenDepotDialog::OpenDepotDialog( PluginOperations *ops, PerforceOptions *options, StrBufArray *filetypes ) : Dialog( ops, IDD_OPEN_DEPOT_DIALOG, DialogWndProc )
{
	ops->SetFileDir();
	con = new PerforceConnection( ops );
	ops->RestoreOldDir();

	this->options = options;
	this->filetypes = filetypes;

	// Set initial sort orders
	nameOrder = 1;
	haveOrder = -1;
	headOrder = -1;
	typeOrder = -1;
}

OpenDepotDialog::~OpenDepotDialog()
{
	delete con;
	ImageList_Destroy( lookInImages );
}

LRESULT APIENTRY OpenDepotDialog::ListViewEnterHandler( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_GETDLGCODE ) {
		LPMSG lpmsg;
		if ( ( lpmsg = (LPMSG)lParam ) != NULL ) {
			// Intercept return key when item selected is a folder.
			if ( lpmsg->message == WM_KEYDOWN && lpmsg->wParam == VK_RETURN ) {
				HWND dialog = GetParent( hwnd );
				HWND fileList = GetDlgItem( dialog, IDC_DEPOT_LIST );
				if ( ListView_GetSelectedCount( fileList ) > 0 ) {
					int idx = ListView_GetNextItem( fileList, -1, LVNI_SELECTED );
					UniStrDict *dict;
					GetItemInfo( fileList, idx, &dict );
					if ( IsFolder( dict ) ) {
						return DLGC_WANTALLKEYS;
					}
				}
			}
		}
	}
	else if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN ) {
		// Process return key when selected item is a folder
		HWND dialog = GetParent( hwnd );
		HWND fileList = GetDlgItem( dialog, IDC_DEPOT_LIST );
		if ( ListView_GetSelectedCount( fileList ) > 0 ) {
			int idx = ListView_GetNextItem( fileList, -1, LVNI_SELECTED );
			TCHAR path[MAX_PATH];
			UniStrDict *dict;
			GetItemInfo( fileList, idx, &dict, path );

			// If item is a folder then open it
			if ( IsFolder( dict ) ) {
				EnterFolder( dialog, path );
				return 0;
			}
		}
	}
	return CallWindowProc( origListViewProc, hwnd, uMsg, wParam, lParam );
}

void OpenDepotDialog::EnterFolder( HWND hWnd, TCHAR *path )
{
	HWND fileList = GetDlgItem( hWnd, IDC_DEPOT_LIST );
	HWND lookIn = GetDlgItem( hWnd, IDC_LOOK_IN_COMBO );
	HWND fileTypes = GetDlgItem( hWnd, IDC_FILE_TYPE_COMBO );

	OpenDepotDialog *dialog = ( OpenDepotDialog * )DialogObject( hWnd );

	dialog->OpenDir( lookIn, path );

	// Refresh files view
	try {
		dialog->ShowCurrFolder( fileList, fileTypes );
	}
	catch (exception e) {
		dialog->SetError( hWnd, &e );
	}
}

void OpenDepotDialog::InitFileTypes( HWND combo )
{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT;

	if ( filetypes != NULL && filetypes->Count() > 0 ) {
		for ( int i = 0; i<filetypes->Count(); i++ ) {
			item.iItem = i;
			item.pszText = filetypes->Get( i );
			SendMessage( combo, CBEM_INSERTITEM, 0, ( LPARAM )&item );
		}
		int curr = options->GetOptionValue( PERFORCE_OPEN_FILE_TYPE );
		if ( curr < filetypes->Count() ) {
			SendMessage( combo, CB_SETCURSEL, (WPARAM)curr, 0 );
		}
		else {
			SendMessage( combo, CB_SETCURSEL, (WPARAM)0, 0 );
		}
	}
	else {
		item.iItem = 0;
		item.pszText = P4_OPENDEPOT_ALL;
		SendMessage( combo, CBEM_INSERTITEM, 0, ( LPARAM )&item );
		SendMessage( combo, CB_SETCURSEL, (WPARAM)0, 0 );
	}
}

int OpenDepotDialog::WildCmp( TCHAR *wild, TCHAR *string )
{
	TCHAR *cp, *mp;

	while ( ( *string ) && ( *wild != '*' ) ) {
		if ( _tcsnicmp( wild, string, 1 ) != 0 && ( *wild != '?' ) ) {
			return 0;
		}
		wild = _tcsinc( wild );
		string = _tcsinc( string );
	}

	while ( *string ) {
		if ( *wild == '*' ) {
			wild = _tcsinc( wild );
			if ( !*wild ) {
				return 1;
			}
			mp = wild;
			cp = _tcsinc( string );
		}
		else if ( _tcsnicmp( wild, string, 1 ) == 0 || ( * wild == '?' ) ) {
			wild = _tcsinc( wild );
			string = _tcsinc( string );
		}
		else {
			wild = mp;
			string = cp;
			cp = _tcsinc( cp );
		}
	}

	while ( *wild == '*' ) {
		wild = _tcsinc( wild );
	}

	return !*wild;
}

/*
 * Original ASCII version of WildCmp
 *
int OpenDepotDialog::WildCmp( char *wild, char *string )
{
	char *cp, *mp;

	while ( ( *string ) && ( *wild != '*' ) ) {
		if ( !CharICmp( *wild, *string ) && ( *wild != '?' ) ) {
			return 0;
		}
		wild++;
		string++;
	}

	while ( *string ) {
		if ( *wild == '*' ) {
			if ( !*++wild ) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		}
		else if ( CharICmp( *wild, *string ) || ( *wild == '?' ) ) {
			wild++;
			string++;
		}
		else {
			wild = mp;
			string = cp++;
		}
	}

	while (*wild == '*') {
		wild++;
	}

	return !*wild;
}
*/

bool OpenDepotDialog::MatchFileType( HWND combo, TCHAR *filename )
{
	TCHAR buff[MAX_TYPE_SIZE];

	COMBOBOXEXITEM info;
	info.mask = CBEIF_TEXT;
	info.iItem = SendMessage( combo, CB_GETCURSEL, 0, 0 );
	info.pszText = buff;
	info.cchTextMax = sizeof( buff );
	SendMessage( combo, CBEM_GETITEM, 0, (LPARAM)&info );

	// Extract name extension
	TCHAR *extn = _tcsrchr( filename, '.' );
	if ( extn != NULL ) {
		extn++;
	}

	TCHAR *curr = buff;
	UniStrBuf wildext;
	while ( curr = _tcschr( curr, '.' ) ) {
		curr++;
		TCHAR *end = curr;

		// Extract extension
		while ( *end && *end != ';' && *end != ')' ) {
			end++;
		}
		// Truncation of sizeof ok. 
#pragma warning(push)
#pragma warning(disable: 4267)
		wildext.Set( curr, ( end - curr ) * sizeof( TCHAR ) );
#pragma warning(pop)

		// Check for files that do not have an extension
		if ( extn == NULL ) {
			// files with no extension only match against *.*
			if ( _tcscmp( wildext.Text(), _T("*") ) == 0 ) {
				return true;
			}
		}
		else {
			if ( WildCmp( wildext.Text(), extn ) ) {
				return true;
			}
		}
	}

	return false;
}

void OpenDepotDialog::AddLookInItem( HWND combo, int pos, TCHAR *text, int indent )
{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	item.iItem = pos;
	item.pszText = text;
	item.iIndent = indent;

	item.iImage = 0;
	item.iSelectedImage = 1;

	SendMessage( combo, CBEM_INSERTITEM, 0, ( LPARAM )&item );
}

void OpenDepotDialog::SetupLookInImageLists( HWND combo )
{
	// Image list gets destroyed in destructor
	lookInImages = ImageList_Create( 21, 13, ILC_COLOR24 | ILC_MASK, 3, 1 );
	COLORREF mask = RGB( 0, 255, 255 );

	HBITMAP bitmap = ( HBITMAP )LoadImage( Instance(), MAKEINTRESOURCE( IDB_FOLDER_CLOSED ), IMAGE_BITMAP, 0, 0, 0 );
	ImageList_AddMasked( lookInImages, bitmap, mask );
	DeleteObject( bitmap );	// ImageList_Add copies object

	bitmap = ( HBITMAP )LoadImage( Instance(), MAKEINTRESOURCE( IDB_FOLDER_OPEN ), IMAGE_BITMAP, 0, 0, 0 );
	ImageList_AddMasked( lookInImages, bitmap, mask );
	DeleteObject( bitmap );	// ImageList_Add copies object

	bitmap = ( HBITMAP )LoadImage( Instance(), MAKEINTRESOURCE( IDB_DEPOT ), IMAGE_BITMAP, 0, 0, 0 );
	ImageList_AddMasked( lookInImages, bitmap, mask );
	DeleteObject( bitmap );	// ImageList_Add copies object

	SendMessage( combo, CBEM_SETIMAGELIST, 0, ( LPARAM )lookInImages );
}

void OpenDepotDialog::SetupReportColumns( HWND list )
{
	LV_COLUMN col;

	// Filename column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 200;
	col.iSubItem = 0;
	col.pszText = P4_OPENDEPOT_NAME;
	ListView_InsertColumn( list, 0, &col );

	// Have revision column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_RIGHT;
	col.cx = 110;
	col.iSubItem = 1;
	col.pszText = P4_OPENDEPOT_HAVE;
	ListView_InsertColumn( list, 1, &col );

	// Out-of-sync indicator column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 10;
	col.iSubItem = 2;
	col.pszText = _T(" ");
	ListView_InsertColumn( list, 2, &col );

	// Head revision column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_RIGHT;
	col.cx = 110;
	col.iSubItem = 3;
	col.pszText = P4_OPENDEPOT_HEAD;
	ListView_InsertColumn( list, 3, &col );

	// File type column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 60;
	col.iSubItem = 4;
	col.pszText = P4_OPENDEPOT_TYPE;
	ListView_InsertColumn( list, 4, &col );

	// Locked by column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 170;
	col.iSubItem = 5;
	col.pszText = P4_OPENDEPOT_LOCKED_BY;
	ListView_InsertColumn( list, 5, &col );
}

void OpenDepotDialog::SelectLookInFolder( HWND combo, int selected )
{
	// If LRESULT is over MAXINT, this will truncate the value.
	int count = (int)SendMessage( combo, CB_GETCOUNT, 0, 0 );
	int last = selected;
	TCHAR path[MAX_PATH];

	// Get text and indent of selected item
	COMBOBOXEXITEM info;
	info.mask = CBEIF_INDENT | CBEIF_TEXT;
	info.iItem = selected;
	info.pszText = path;
	info.cchTextMax = MAX_PATH;
	SendMessage( combo, CBEM_GETITEM, 0, ( LPARAM )&info );

	// Selected item is a Depot
	if ( info.iIndent == 0 ) {
		info.mask = CBEIF_INDENT;
		// Remove all non depot entries
		for ( int i=count-1; i >= 0; i-- ) {
			info.iItem = i;
			SendMessage( combo, CBEM_GETITEM, 0, ( LPARAM )&info );
			if ( info.iIndent > 0 ) {
				SendMessage( combo, CBEM_DELETEITEM, i, 0 );
			}
		}
		SetCurrDepot( path );
	}
	// Selected item is a folder
	else {
		info.mask = CBEIF_INDENT;
		// Get last item we need to delete.
		// This will be the last folder before the next depot or end of list
		for ( int i=selected + 1; i<count; i++ ) {
			info.iItem = i;
			SendMessage( combo, CBEM_GETITEM, 0, ( LPARAM )&info );
			// This item is a depot so break
			if ( info.iIndent == 0 ) {
				break;
			}
			last = i;
		}
		// Remove all items between last and curr selected.
		for ( int i=last; i != selected; i-- ) {
			SendMessage( combo, CBEM_DELETEITEM, i, 0 );
			PopFolder();
		}
	}

	SendMessage( combo, CB_SETCURSEL, ( WPARAM )selected, 0);
}

void OpenDepotDialog::Tooltips( LPNMTBGETINFOTIP tooltipEvent )
{
	switch ( tooltipEvent->iItem ) {
	case IDM_LARGEICON:
		_tcscpy( tooltipEvent->pszText, P4_OPENDEPOT_LARGE_ICON );
		break;
	case IDM_SMALLICON:
		_tcscpy( tooltipEvent->pszText, P4_OPENDEPOT_SMALL_ICON );
		break;
	case IDM_LISTVIEW:
		_tcscpy( tooltipEvent->pszText, P4_OPENDEPOT_LIST );
		break;
	case IDM_REPORTVIEW:
		_tcscpy( tooltipEvent->pszText, P4_OPENDEPOT_DETAILS );
		break;
	case IDM_PARENTFOLDER:
		_tcscpy( tooltipEvent->pszText, P4_OPENDEPOT_UP_ONE );
		break;
	}
}

void OpenDepotDialog::CreateToolbar( HWND dialog, HWND combo )
{
	static TBBUTTON buttons1[] = {
		{VIEW_PARENTFOLDER, IDM_PARENTFOLDER, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, 0},
	};

	static TBBUTTON buttons2[] = {
		{VIEW_LARGEICONS, IDM_LARGEICON, TBSTATE_ENABLED | TBSTATE_CHECKED, BTNS_BUTTON | BTNS_CHECKGROUP, {0}, 0, 0},
		{VIEW_SMALLICONS, IDM_SMALLICON, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_CHECKGROUP, {0}, 0, 0},
		{VIEW_LIST, IDM_LISTVIEW, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_CHECKGROUP, {0}, 0, 0},
		{VIEW_DETAILS, IDM_REPORTVIEW, TBSTATE_ENABLED, BTNS_BUTTON | BTNS_CHECKGROUP, {0}, 0, 0},
	};

	static TBADDBITMAP bitmaps = {HINST_COMMCTRL, IDB_VIEW_SMALL_COLOR};

	toolbar1 = CreateWindowEx( 0, TOOLBARCLASSNAME, (TCHAR *) NULL,
							   WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT |
							   CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | WS_TABSTOP,
							   0, 0, 0, 0, dialog, ( HMENU )IDC_FOLDER_TOOLBAR, Instance(), NULL );

	toolbar2 = CreateWindowEx( 0, TOOLBARCLASSNAME, (TCHAR *) NULL,
							   WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT |
							   CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | WS_TABSTOP,
							   0, 0, 0, 0, dialog, ( HMENU )IDC_LIST_TOOLBAR, Instance(), NULL );

	// Set tab order
	HWND lookIn = GetDlgItem( dialog, IDC_LOOK_IN_COMBO );
	SetWindowPos( toolbar1, lookIn, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	SetWindowPos( toolbar2, toolbar1, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

	// Set version of TBBUTTON structure
	SendMessage( toolbar1, TB_BUTTONSTRUCTSIZE, ( WPARAM )sizeof( TBBUTTON ), 0 );
	SendMessage( toolbar2, TB_BUTTONSTRUCTSIZE, ( WPARAM )sizeof( TBBUTTON ), 0 );

	// Set toolbar images
	SendMessage( toolbar1, TB_ADDBITMAP, 0, (LPARAM)&bitmaps );
	SendMessage( toolbar2, TB_ADDBITMAP, 0, (LPARAM)&bitmaps );

	int nbuttons1 = sizeof( buttons1 )/sizeof( TBBUTTON );
	int nbuttons2 = sizeof( buttons2 )/sizeof( TBBUTTON );

	// Set toolbar buttons
	SendMessage( toolbar1, TB_ADDBUTTONS, (WPARAM) nbuttons1,  (LPARAM) &buttons1 );
	SendMessage( toolbar2, TB_ADDBUTTONS, (WPARAM) nbuttons2,  (LPARAM) &buttons2 );

	// Set toolbar size
	SendMessage( toolbar1, TB_AUTOSIZE, (WPARAM) 0, (LPARAM) 0 );
	SendMessage( toolbar2, TB_AUTOSIZE, (WPARAM) 0, (LPARAM) 0 );

	// Move toolbars to right of combo
	RECT comboRect;
	GetControlPos( dialog, combo, &comboRect );
	// The TB_AUTOSIZE does not seem to set toolbar width and height
	// so need to set explicitly
	MoveWindow( toolbar1, comboRect.right + 10, comboRect.top, TB_BUTTON_WIDTH * nbuttons1, TB_BUTTON_HEIGHT, FALSE );
	MoveWindow( toolbar2, comboRect.right + 40, comboRect.top, TB_BUTTON_WIDTH * nbuttons2, TB_BUTTON_HEIGHT, FALSE );

	ShowWindow( toolbar1, SW_SHOW );
	ShowWindow( toolbar2, SW_SHOW );
}

INT_PTR CALLBACK OpenDepotDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DWORD dwStyle;

	HWND fileList = GetDlgItem( hWnd, IDC_DEPOT_LIST );
	HWND lookIn = GetDlgItem( hWnd, IDC_LOOK_IN_COMBO );
	HWND openFile = GetDlgItem( hWnd, IDC_OPEN_FILE_EDIT );
	HWND fileTypes = GetDlgItem( hWnd, IDC_FILE_TYPE_COMBO );
	HWND fileNameLabel = GetDlgItem( hWnd, IDC_FILE_NAME_LABEL );
	HWND fileTypeLabel = GetDlgItem( hWnd, IDC_FILE_TYPE_LABEL );
	HWND okButton = GetDlgItem( hWnd, IDOK );
	HWND cancelButton = GetDlgItem( hWnd, IDCANCEL );

	switch(uMsg) {

	case WM_INITDIALOG:	{
		OpenDepotDialog *dialog = ( OpenDepotDialog * )lParam;
		dialog->SaveDialogObject( hWnd, dialog );

		SetupReportColumns( fileList );
		SetListViewImageLists( fileList );
		dialog->SetupLookInImageLists( lookIn );
		dialog->CreateToolbar( hWnd, lookIn );
		dialog->InitFileTypes( fileTypes );
        dialog->InitCaption( hWnd );

		origListViewProc = (WNDPROC)SetWindowLongPtr( fileList, (int) GWLP_WNDPROC , (LONG_PTR)ListViewEnterHandler );

		try {
			dialog->InitCurrDir( lookIn );
			dialog->ShowCurrFolder( fileList, fileTypes );
		}
		catch ( exception e ) {
			dialog->SetError( hWnd, &e );
		}

		SaveInitialSizes( hWnd );
		SetResizeFlags( fileList, RESIZE_HEIGHT | RESIZE_WIDTH );
		SetResizeFlags( openFile, RESIZE_TOP | RESIZE_WIDTH );
		SetResizeFlags( fileTypes, RESIZE_TOP | RESIZE_WIDTH );
		SetResizeFlags( fileNameLabel, RESIZE_TOP );
		SetResizeFlags( fileTypeLabel, RESIZE_TOP );
		SetResizeFlags( lookIn, RESIZE_WIDTH );
		SetResizeFlags( dialog->toolbar1, RESIZE_LEFT );
		SetResizeFlags( dialog->toolbar2, RESIZE_LEFT );
		SetResizeFlags( okButton, RESIZE_TOP | RESIZE_LEFT );
		SetResizeFlags( cancelButton, RESIZE_TOP | RESIZE_LEFT );

		// Restore size of dialog
		int width = dialog->options->GetOptionValue( PERFORCE_OPEN_WIDTH );
		int height = dialog->options->GetOptionValue( PERFORCE_OPEN_HEIGHT );
		if ( width > 0 && height > 0 ) {
			MoveWindow( hWnd, 0, 0, width, height, TRUE );
			RECT rect;
			GetClientRect( hWnd, &rect );
			Resize( hWnd, MAKELONG( rect.right, rect.bottom ) );
		}
		Centre( hWnd );

		long button;
		long style = dialog->options->GetOptionValue( PERFORCE_OPEN_VIEW_TYPE );
		switch ( style ) {
		case LVS_ICON:
			button = IDM_LARGEICON;
			break;
		case LVS_SMALLICON:
			button = IDM_SMALLICON;
			break;
		case LVS_LIST:
			button = IDM_LISTVIEW;
			break;
		case LVS_REPORT:
			button = IDM_REPORTVIEW;
			break;
		}

		SendMessage( dialog->toolbar2, (UINT) TB_CHECKBUTTON, (WPARAM)button, (LPARAM)TRUE );
		DWORD dwStyle = GetWindowLong( fileList, GWL_STYLE );
		SetWindowLong( fileList, GWL_STYLE,  ( dwStyle & ~LVS_TYPEMASK ) | style );

		return true;
		break;
	}

	case WM_GETMINMAXINFO:
		SetMinimumSize( hWnd, (LPMINMAXINFO) lParam );
		return true;
		break;

	case WM_SIZE: {
		Resize( hWnd, lParam );
		return true;
		break;
	}

	case WM_PAINT: {
		OpenDepotDialog *dialog = ( OpenDepotDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_NOTIFY: {
		LPNMHDR listEvent = ( LPNMHDR ) lParam;
		OpenDepotDialog *dialog = ( OpenDepotDialog * )DialogObject( hWnd );

		if ( (int)wParam  == IDC_DEPOT_LIST ) {
			if ( listEvent->code == NM_DBLCLK ) {
				LPNMITEMACTIVATE clickEvent = ( LPNMITEMACTIVATE ) lParam;

				// Make sure click is on an item
				if ( clickEvent->iItem >= 0 ) {
					TCHAR path[MAX_PATH];
					UniStrDict *dict;
					GetItemInfo( listEvent->hwndFrom, clickEvent->iItem, &dict, path );

					// If item is a folder and it was double clicked
					if ( IsFolder( dict ) ) {
						EnterFolder( hWnd, path );
					}
					// double click item is file
					else {
						EndDialog( hWnd, TRUE );
					}
				}
				return true;
			}
			else if ( listEvent->code == LVN_ITEMCHANGED ) {
				LPNMLISTVIEW changedEvent = ( LPNMLISTVIEW ) lParam;
				if ( changedEvent->iItem >= 0 ) {
					TCHAR path[MAX_PATH];
					UniStrDict *dict;
					BOOL selected;
					GetItemInfo( listEvent->hwndFrom, changedEvent->iItem, &dict, path, &selected );
					if ( selected && !IsFolder( dict ) ) {
						dialog->SetSelectedFile( path );
						SetDlgItemText( hWnd, IDC_OPEN_FILE_EDIT, path );
					}
				}
			}
			// Column header click
			else if ( listEvent->code == LVN_COLUMNCLICK ) {
				LPNMLISTVIEW item = ( LPNMLISTVIEW ) lParam;
				dialog->sortCol = item->iSubItem;
				// Get column for sort and change order
				switch ( dialog->sortCol ) {
				case 0:
					dialog->nameOrder *= -1;
					dialog->sortOrder = dialog->nameOrder;
					break;
				case 1:
					dialog->haveOrder *= -1;
					dialog->sortOrder = dialog->haveOrder;
					break;
				case 2:
					dialog->headOrder *= -1;
					dialog->sortOrder = dialog->headOrder;
					break;
				case 3:
					dialog->typeOrder *= -1;
					dialog->sortOrder = dialog->typeOrder;
					break;
				}
				ListView_SortItems( fileList, Compare, (LPARAM) dialog );
			}
		}
		// Toolbar tooltips
		else if ( ( (int)wParam  == IDC_LIST_TOOLBAR || (int)wParam == IDC_FOLDER_TOOLBAR )
			      && listEvent->code == TBN_GETINFOTIP ) {
			dialog->Tooltips( (LPNMTBGETINFOTIP) lParam );
			return true;
		}
		}
		break;

	case WM_COMMAND:
		OpenDepotDialog *dialog = ( OpenDepotDialog * )DialogObject( hWnd );

		switch ( LOWORD( wParam ) ) {
		case IDC_OPEN_FILE_EDIT:
			// Handle manual edit of filename field
			if ( HIWORD( wParam ) == EN_KILLFOCUS ) {
				TCHAR path[MAX_PATH];
				GetDlgItemText( hWnd, IDC_OPEN_FILE_EDIT, path, MAX_PATH );
				dialog->SetSelectedFile( path );
				return true;
			}
			break;
		case IDC_LOOK_IN_COMBO:
			// Selected a folder from the Look In combo
			if ( HIWORD( wParam ) == CBN_SELCHANGE ) {
				// If LRESULT is over MAXINT, this will truncate the value.
				dialog->SelectLookInFolder( lookIn, (int)SendMessage( lookIn, CB_GETCURSEL, 0, 0 ) );
				try {
					dialog->ShowCurrFolder( fileList, fileTypes );
				}
				catch ( exception e ) {
					dialog->SetError( hWnd, &e );
				}
				return true;
			}
			break;
		case IDC_FILE_TYPE_COMBO:
			// Changed the file type
			if ( HIWORD( wParam ) == CBN_SELCHANGE ) {
				try {
					dialog->ShowCurrFolder( fileList, fileTypes, false );
				}
				catch ( exception e ) {
					dialog->SetError( hWnd, &e );
				}
				return true;
			}
			break;
		case IDM_PARENTFOLDER:
			// Go up a level
			// If LRESULT is over MAXINT, this will truncate the value.
			dialog->SelectLookInFolder( lookIn, (int)SendMessage( lookIn, CB_GETCURSEL, 0, 0 ) - 1);
			try {
				dialog->ShowCurrFolder( fileList, fileTypes );
			}
			catch ( exception e ) {
				dialog->SetError( hWnd, &e );
			}
			return true;
		case IDM_LARGEICON:
			dwStyle = GetWindowLong( fileList, GWL_STYLE );
			SetWindowLong( fileList, GWL_STYLE,  ( dwStyle & ~LVS_TYPEMASK ) | LVS_ICON );
			return true;
		case IDM_SMALLICON:
			dwStyle = GetWindowLong( fileList, GWL_STYLE );
			SetWindowLong( fileList, GWL_STYLE,  ( dwStyle & ~LVS_TYPEMASK ) | LVS_SMALLICON );
			return true;
		case IDM_LISTVIEW:
			dwStyle = GetWindowLong( fileList, GWL_STYLE );
			SetWindowLong( fileList, GWL_STYLE, ( dwStyle & ~LVS_TYPEMASK ) | LVS_LIST );
			return true;
		case IDM_REPORTVIEW:
			dwStyle = GetWindowLong( fileList, GWL_STYLE );
			SetWindowLong( fileList, GWL_STYLE,  ( dwStyle & ~LVS_TYPEMASK ) | LVS_REPORT );
			return true;
		case IDOK:
			TCHAR path[MAX_PATH];
			// Only allow OK if have selected a file
			GetDlgItemText( hWnd, IDC_OPEN_FILE_EDIT, path, MAX_PATH );
			if ( _tcslen( path ) > 0 ) {
				dialog->SaveSettings( hWnd );
				FinishDialog( hWnd, TRUE );
			}
			return true;
		case IDCANCEL:
			dialog->SaveSettings( hWnd );
			FinishDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}

void OpenDepotDialog::SaveSettings( HWND hWnd )
{
	HWND fileList = GetDlgItem( hWnd, IDC_DEPOT_LIST );
	HWND fileTypes = GetDlgItem( hWnd, IDC_FILE_TYPE_COMBO );

	// Save view type
	DWORD dwStyle = GetWindowLong( fileList, GWL_STYLE );
	dwStyle &= LVS_TYPEMASK;
	options->SetOptionValue( PERFORCE_OPEN_VIEW_TYPE, dwStyle );

	// Save current directory
	options->SetOptionString( PERFORCE_OPEN_DIR, currDir.Text() );

	// Save file type
	// If LRESULT is over MAXINT, this will truncate the value.
	options->SetOptionValue( PERFORCE_OPEN_FILE_TYPE, (int)SendMessage( fileTypes, CB_GETCURSEL, 0, 0 ) );

	// Save size of dialog
	RECT rect;
	GetWindowRect( hWnd, &rect );
	options->SetOptionValue( PERFORCE_OPEN_WIDTH, rect.right - rect.left );
    options->SetOptionValue( PERFORCE_OPEN_HEIGHT, rect.bottom - rect.top );
}

void OpenDepotDialog::SelectDepot( HWND combo, long idx )
{
	SendMessage( combo, CB_SETCURSEL, (WPARAM)idx, 0 );
	TCHAR buff[MAX_PATH];
	SendMessage( combo, CB_GETLBTEXT, (WPARAM)idx, (LPARAM)buff );
	SetCurrDepot( buff );
}

void OpenDepotDialog::SetCurrDepot( TCHAR *name )
{
	currDir.Set( _T("//") );
	currDir.Append( name );
}

void OpenDepotDialog::InitCurrDir( HWND combo )
{
	// Go and get all the depots
	PerforceCommand cmd( _T("depots") );
	PerforceResults results;
	con->Exec( &cmd, &results );

	// p4 depots returns nothing unless you have actually gone and
	// created a depot
	if ( results.Count() == NULL ) {
		AddLookInItem( combo, -1, _T("depot"), 0 );
	}
	else if ( results.Count() > 0 ) {
		UniStrBuf depot;
		for ( int i=0; i<results.Count(); i++ ) {
			UniStrDict *dict = ( UniStrDict * )results.Get( i );
			StrPtr *depotInfo = ( StrPtr * )dict->GetVar( "msg" );
			if ( depotInfo != NULL ) {
				TCHAR *start = _tcschr( (TCHAR *)depotInfo->Text(), ' ' );
				start = _tcsinc( start );
				TCHAR *end = _tcschr( start, ' ' );
				// Truncation of sizeof ok. 
#pragma warning(push)
#pragma warning(disable: 4267)
				depot.Set( start, (end - start) * sizeof(TCHAR) );
#pragma warning(pop)
				AddLookInItem( combo, -1, depot.Text(), 0 );
			}
		}
	}

	UniStrBuf currentDir;
	bool validDir = false;
	if ( options->GetOptionString( PERFORCE_OPEN_DIR, &currentDir ) ) {

		// Check the saved directory still exists
		PerforceCommand dirsCmd( _T("dirs"), _T("-C"), currentDir.Text() );
		// Errors will get thrown
		con->Exec( &dirsCmd, &results );
		if ( results.Count() > 0 ) {
			TCHAR *start = currentDir.Text();
			start = _tcsinc( start );
			start = _tcsinc( start );
			TCHAR *end;
			UniStrBuf buff;
			// Extract name of depot
			end = _tcschr( start, '/' );
			if ( end ) {
				// Truncation of sizeof ok. 
#pragma warning(push)
#pragma warning(disable: 4267)
				buff.Set( start, (end - start) * sizeof( TCHAR ) );
#pragma warning(pop)
			}
			else {
				buff.Set( start );
			}
			// If LRESULT is over MAXLONG, this will truncate the value.
			long idx = (long)SendMessage( combo, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)buff.Text() );
			if ( idx != CB_ERR ) {
				// Select the depot
				SelectDepot( combo, idx );

				// Go and select each directory in the path
				if ( end = _tcschr( start, '/'), end != NULL ) {
					start = _tcsinc( end );
					while ( end = _tcschr( start, '/' ), end != NULL ) {
					// Truncation of sizeof ok. 
#pragma warning(push)
#pragma warning(disable: 4267)
						buff.Set( start, (end - start) * sizeof( TCHAR ) );
#pragma warning(pop)
						OpenDir( combo, buff.Text() );
						start = _tcsinc( end );
					}
					OpenDir( combo, start );
				}
				validDir = true;
			}
		}
	}

	if ( !validDir ) {
		// Select first depot
		SelectDepot( combo, 0 );
	}
}

void OpenDepotDialog::OpenDir( HWND combo, TCHAR *dir )
{
	// Get Indentation of currently selected folder
	// If LRESULT is over MAXINT, this will truncate the value.
	int selected = (int)SendMessage( combo, CB_GETCURSEL, 0, 0 );
	COMBOBOXEXITEM info;
	info.mask = CBEIF_INDENT;
	info.iItem = selected;
	SendMessage(combo, CBEM_GETITEM, 0, (LPARAM)&info);

	// Add selected folder to Look In Combo
	AddLookInItem( combo, selected + 1, dir, info.iIndent + 1 );

	// Make new folder the current selection
	SendMessage(combo, CB_SETCURSEL, (WPARAM)selected + 1, 0);

	PushFolder( dir );
}

void OpenDepotDialog::PushFolder( TCHAR *folder )
{
	currDir.Append( _T("/") );
	currDir.Append( folder );
}

void OpenDepotDialog::PopFolder()
{
	size_t len = _tcsrchr( currDir.Text(), '/' ) - currDir.Text();
	// Truncation of size_t may cuase loss of data.
#pragma warning(push)
#pragma warning(disable: 4267)
	currDir.Set( currDir.Text(), len * sizeof(TCHAR) );
#pragma warning(pop)
}

void OpenDepotDialog::InsertFolder( HWND list, int pos, UniStrDict *dict )
{
	StrPtr *name = ( StrPtr * )dict->GetVar( "dir" );

	LV_ITEM item;
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	item.iSubItem = 0;
	item.iItem = pos;
	item.iImage = FolderIcon();
	item.pszText = ExtractFilename( name );
	item.lParam = (LPARAM)dict;
	ListView_InsertItem( list, &item );
}

void OpenDepotDialog::InsertFile( HWND list, int pos, UniStrDict *dict )
{
	StrPtr *name 	= ( StrPtr * )dict->GetVar( "depotFile" );
	StrPtr *have 	= ( StrPtr *)dict->GetVar( "haveRev" );
	StrPtr *head 	= ( StrPtr *)dict->GetVar( "headRev" );
	StrPtr *type 	= ( StrPtr *)dict->GetVar( "headType" );
	StrPtr *locked 	= ( StrPtr *)dict->GetVar( "ourLock" );
	StrPtr *other 	= ( StrPtr *)dict->GetVar( "otherLock0" );
	StrPtr *opened 	= ( StrPtr *)dict->GetVar( "otherOpen" );
	StrPtr *owner	= ( StrPtr *)dict->GetVar( "actionOwner" );

	LV_ITEM item;
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	item.iSubItem = 0;
	item.iItem = pos;
	item.iImage = FileIcon( name );
	item.pszText = ExtractFilename( name );
	item.lParam = (LPARAM)dict;
	ListView_InsertItem(list, &item );

	item.mask = LVIF_TEXT;

	item.iSubItem = 1;
	if (have) {
		item.pszText = (TCHAR *)have->Text();
	}
	else {
		item.pszText = _T("0");
	}
	ListView_SetItem( list, &item );

	item.iSubItem = 2;
	// Indicate if file is out-of-sync.
	item.pszText = _T("*");
	if (have) {
		if ( _tcscmp( (TCHAR *)head->Text(), (TCHAR *)have->Text() ) == 0 ) {
			item.pszText = _T(" ");
		}
	}
	ListView_SetItem( list, &item );

	item.iSubItem = 3;
	item.pszText = (TCHAR *)head->Text();
	ListView_SetItem( list, &item );

	item.iSubItem = 4;
	item.pszText = (TCHAR *)type->Text();
	ListView_SetItem( list, &item );

	item.iSubItem = 5;
	// Check if file already locked and by whom.
	if (locked) {
		item.pszText = _T("you");
	}
	else if (other) {
		item.pszText = (TCHAR *)other->Text();
	}
	// Check if exclusively opened and by whom.
	else if ( _tcsstr( ( TCHAR *)type->Text(), _T("+") ) != NULL ) {
			if ( _tcsstr( ( TCHAR *)type->Text(), _T("l") ) != NULL ) {
				if (opened) {
					item.pszText = (TCHAR *)opened->Text();
				}
				else if (owner) {
					item.pszText = P4_OPENDEPOT_YOU;
				}
			else {
				item.pszText = _T(" ");
			}
		}
	}
	else {
		item.pszText = _T(" ");
	}
	ListView_SetItem( list, &item );
}

void OpenDepotDialog::ShowCurrFolder( HWND list, HWND fileTypes, bool refresh )
{
	ListView_DeleteAllItems(list);

	UniStrBuf buff;
	buff.Set( currDir.Text() );
	buff.Append( _T("/*") );
	int pos = 0;

	// Get all the folders
	if ( refresh ) {
		PerforceCommand foldersCmd( _T("dirs"), _T("-C"), buff.Text() );
		con->Exec( &foldersCmd, &dirResults );
	}

	// If there are no folders in the directory then we just
	// get a message saying:
	// //depot/foo/bar/* - no such file(s).
	for ( int i=0; i<dirResults.Count(); i++ ) {
		UniStrDict *dict = ( UniStrDict * )dirResults.Get( i );
		if ( dict->GetVar( "dir" ) != NULL ) {
			InsertFolder( list, pos++, dict );
		}
	}

	try {
		// Get all files
		if ( refresh ) {
			PerforceCommand filesCmd( _T("fstat"), buff.Text() );
			con->Exec( &filesCmd, &fileResults );
		}

		for ( int i=0; i<fileResults.Count(); i++ ) {
			UniStrDict *dict = ( UniStrDict * )fileResults.Get( i );
			StrPtr *name = ( StrPtr * )dict->GetVar( "depotFile" );
			if ( name != NULL ) {
				StrPtr *headAction = ( StrPtr * )dict->GetVar( "headAction" );

				// Don't include deleted files (or newly added files when headAction == NULL)
				// or files not mapped to client
				if ( headAction && _tcscmp( (TCHAR *)headAction->Text(), _T("delete") ) != 0 && dict->GetVar("clientFile") != NULL ) {
					// Check filename matches current type filter
					if ( MatchFileType( fileTypes, (TCHAR *)( ( StrPtr * )dict->GetVar( "depotFile" ) )->Text() ) ) {
						InsertFile( list, pos++, dict );
					}
				}
			}
		}
	}
	catch ( exception e ) {
		// Ignore errors of the form:
		// //depot/* - protected namespace - access denied.
		// This can happen when doing an fstat on a directory for which the user
		// does not have permissions
		if ( strstr( e.what(), "protected namespace" ) == NULL ) {
			throw e;
		}
	}

	// Update state of Up Level button
	SendMessage( toolbar1,  ( UINT ) TB_ENABLEBUTTON, ( WPARAM ) IDM_PARENTFOLDER,
		         ( LPARAM ) MAKELONG( !InRootFolder(), 0 ) );

	if ( ListView_GetItemCount( list ) > 0 ) {
		ListView_SetItemState( list, 0, LVIS_FOCUSED, LVIS_FOCUSED );
	}
}

bool OpenDepotDialog::InRootFolder()
{
	TCHAR *start = currDir.Text();
	start = _tcsinc( start );
	start = _tcsinc( start );
	if ( _tcschr( start, '/' ) != NULL ) {
		return false;
	}
	else {
		return true;
	}
}

void OpenDepotDialog::SetSelectedFile( TCHAR *name )
{
	selectedFile = currDir;
	selectedFile.Append( _T("/") );
	selectedFile.Append( name );
}

StrPtr *OpenDepotDialog::SelectedFile() {

	// Sync file
	PerforceCommand syncCmd( _T("sync"), selectedFile.Text() );
	PerforceResults results;
	con->Exec( &syncCmd, &results );

	// Get and return the client path
	PerforceCommand statCmd( _T("fstat"), selectedFile.Text() );
	con->Exec( &statCmd, &results );
	if ( results.Count() > 0 ) {
		UniStrDict *dict = ( UniStrDict * )results.Get( 0 );
		StrPtr *clientFile = ( StrPtr * )dict->GetVar( "clientFile" );
		if ( clientFile != NULL ) {
			selectedFile.Set( (TCHAR *)clientFile->Text() );
			return &selectedFile;
		}
	}

	return NULL;
}

bool OpenDepotDialog::IsFolder( UniStrDict *dict )
{
	if ( dict->GetVar( "dir" ) ) {
		return true;
	}
	else {
		return false;
	}
}

int CALLBACK OpenDepotDialog::Compare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	OpenDepotDialog *dialog = (OpenDepotDialog *)lParamSort;
	UniStrDict *dict1 = (UniStrDict *)lParam1;
	UniStrDict *dict2 = (UniStrDict *)lParam2;
	bool strval;
	char *name;
	switch ( dialog->sortCol ) {
	case 0:
		strval = true;
		name = "depotFile";
		break;
	case 1:
		strval = false;
		name = "haveRev";
		break;
	case 2:
		strval = true;
		name = "depotFile";
		break;
	case 3:
		strval = false;
		name = "headRev";
		break;
	case 4:
		strval = true;
		name = "headType";
		break;
	case 5:
		strval = true;
		name = "depotFile";
		break;
	}
	TCHAR *val1, *val2;
	UniStrBuf buf1, buf2;
	if ( dialog->sortCol == 0 ) {
		if ( IsFolder( dict1 ) ) {
			// Group folders last
			buf1.Set( _T("ZZZZ") );
			buf1.Append( ExtractFilename( ( StrPtr * )dict1->GetVar( "dir" ) ) );
			val1 = buf1.Text();
		}
		else {
			val1 = ExtractFilename( ( StrPtr * )dict1->GetVar( "depotFile" ) );
		}
		if ( IsFolder( dict2 ) ) {
			// Group folders last
			buf2.Set( _T("ZZZZ") );
			buf2.Append( ExtractFilename( ( StrPtr * )dict2->GetVar( "dir" ) ) );
			val2 = buf2.Text();
		}
		else {
			val2 = ExtractFilename( ( StrPtr * )dict2->GetVar( "depotFile" ) );
		}
	}
	else {
		// Use default sort variables when selected variable doesn't exist.
		if ( (TCHAR *)dict1->GetVar( name ) == NULL ) {
			strval = true;
			name = "depotFile";
		}
		val1 =  IsFolder( dict1 ) ? _T("") : (TCHAR *)dict1->GetVar( name )->Text();
		if( dict2->GetVar( name ) != NULL )
		  val2 = IsFolder( dict2 ) ? _T("") : (TCHAR *)dict2->GetVar( name )->Text();
		else
		  val2 = _T("0");
	}
	int result;
	if ( strval ) {
		result = _tcsicmp( val1, val2 );
	}
	else {
		long num1 = _tcstol( val1, NULL, 10 );
		long num2 = _tcstol( val2, NULL, 10 );
		if ( num1 < num2 ) {
			result = -1;
		}
		else if ( num2 < num1 ) {
			result = 1;
		}
		else {
			result = 0;
		}
	}
	return result * dialog->sortOrder;
}
