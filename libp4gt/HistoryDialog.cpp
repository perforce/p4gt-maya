/*
 *
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "HistoryDialog.h"
#include "PerforceConnection.h"
#include "debug.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

#include <commctrl.h>
#include <time.h>

HistoryDialog::HistoryDialog( PluginOperations *ops, PerforceResults *results ) : Dialog( ops, IDD_HISTORY_DIALOG, DialogWndProc )
{
	this->results = results;
	exitCode = 0;

	// Set initial sort orders
	revOrder = -1;
	changeOrder = -1;
	dateOrder = -1;
	userOrder = -1;
	actionOrder = -1;
	descOrder = -1;
}

PerforceResults *HistoryDialog::Results()
{
	return results;
}

StrBuf *HistoryDialog::TempFile()
{
	return &tempFile;
}

int HistoryDialog::GetSelectedRevision( HWND list )
{
	int count = ListView_GetItemCount( list );

	// Go through list looking for selected item
	for (int i=0; i<count; i++) {

		// Found selected item
		if ( ListView_GetItemState( list, i, LVIS_SELECTED ) ) {

			// Get revision number of selected item
			LVITEM item;
			item.mask = LVIF_TEXT;
			item.iItem = i;
			item.iSubItem = 0;
			TCHAR buff[64];
			item.pszText = buff;
			item.cchTextMax = sizeof( buff );
			ListView_GetItem( list, &item );
			return _tstoi( buff );
		}
	}
	return 0;
}

void HistoryDialog::SyncFile( int version )
{
	UniStrDict *dict = ( UniStrDict * )results->Get( 0 );
	TCHAR verBuff[64];
	_stprintf( verBuff, _T("%d"), version );

	// Get full depot path of file including revision number
	StrPtr *depotFile = dict->GetVar( "depotFile" );
	UniStrBuf depotVersion;
	depotVersion.Set( (TCHAR *)depotFile->Text() );
	depotVersion.Append( _T("#") );
	depotVersion.Append( verBuff );

	ops->Sync( depotVersion.Text(), false );
}

void HistoryDialog::CreateTempFile( int version )
{
	UniStrDict *dict = ( UniStrDict * )results->Get( 0 );
	TCHAR verBuff[64];
	_stprintf( verBuff, _T("%d"), version );

	// Get full depot path of file including revision number
	StrPtr *depotFile = dict->GetVar( "depotFile" );
	UniStrBuf depotVersion;
	depotVersion.Set( (TCHAR *)depotFile->Text() );
	depotVersion.Append( _T("#") );
	depotVersion.Append( verBuff );

	// Generate full path of temporary file
	TCHAR buff[MAX_PATH];
	GetTempPath( MAX_PATH, buff );
	tempFile.Set( buff );
	tempFile.Append( P4_HISTORY_VER );
	tempFile.Append( verBuff );
	tempFile.Append( _T("-") );
	tempFile.Append( ExtractFilename( depotFile ) );

	// Copy depot file to temporary file
	CONNECTION( ops );
	PerforceCommand cmd( _T("print"), _T("-q"), _T("-o"), tempFile.Text(), depotVersion.Text() );
	PerforceResults results;
	con.Exec( &cmd, &results );

	// Make the file read-only
	DWORD attr = GetFileAttributes( tempFile.Text() );
	attr |= FILE_ATTRIBUTE_READONLY;
	SetFileAttributes( tempFile.Text(), attr );
}

void HistoryDialog::InsertListData( HWND list, UniStrDict *dict )
{
	LV_ITEM item;
	StrBuf key;
	UniStrBuf userClient;
	StrPtr *revision, *description, *changelist;
	StrPtr *time, *user, *client, *action;

	// Insert all history lines
	for ( int i=0;;i++ ) {

		// Test if we have another entry
		key.Clear();
		revision = dict->GetVar( key << "rev" << i );
		if ( revision == NULL ) {
			break;
		}

		key.Clear();
		changelist = dict->GetVar( key << "change" << i);

		key.Clear();
		time = dict->GetVar( key << "time" << i );

		key.Clear();
		user = dict->GetVar( key << "user" << i );

		key.Clear();
		client = dict->GetVar( key << "client" << i );

		key.Clear();
		action = dict->GetVar( key << "action" << i );

		key.Clear();
		description = dict->GetVar( key << "desc" << i );

		// Revision column
		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.iItem = i;
		item.iSubItem = 0;
		item.pszText = (TCHAR *)revision->Text();
		// Store the item number in the userdata.
		item.lParam = ( LPARAM )i;
		ListView_InsertItem( list, &item );

		// Changelist column
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 1;
		item.pszText = (TCHAR *)changelist->Text();
		ListView_SetItem( list, &item );

		// Date Column
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 2;
		item.pszText = AsciiTime( time );
		ListView_SetItem( list, &item );

		// User Column
		userClient.Clear();
		userClient.Set( user );
		userClient.Append( _T("@") );
		userClient.Append( client );
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 3;
		item.pszText = userClient.Text();
		ListView_SetItem( list, &item );

		// Action Column
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 4;
		item.pszText = (TCHAR *)action->Text();
		ListView_SetItem( list, &item );

		// Description Column
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 5;
		TCHAR *buff = ReplaceNewlines( description, _T(" ") );
		item.pszText = buff;
		ListView_SetItem( list, &item );

		// Free buffer created by ReplaceNewlines
		delete []buff;
	}
}

void HistoryDialog::SetupList( HWND list )
{

	// Set full row highlight bar
	DWORD style = ListView_GetExtendedListViewStyle( list );
	style |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle( list, style );

	LV_COLUMN col;

	// Add Revision column
	col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 100;
	col.iSubItem = 0;
	col.pszText = P4_HISTORY_REV;
	ListView_InsertColumn( list, 0, &col );

	// Add Changelist column
	col.iSubItem = 1;
	col.cx = 100;
	col.pszText = P4_HISTORY_CHG;
	ListView_InsertColumn( list, 1, &col );

	// Add Date column
	col.iSubItem = 2;
	col.cx = 150;
	col.pszText = P4_HISTORY_DATE;
	ListView_InsertColumn( list, 2, &col );

	// Add User column
	col.iSubItem = 3;
	col.cx = 100;
	col.pszText = P4_HISTORY_USER;
	ListView_InsertColumn( list, 3, &col );

	// Add Action column
	col.iSubItem = 4;
	col.cx = 100;
	col.pszText = P4_HISTORY_ACTION;
	ListView_InsertColumn( list, 4, &col );

	// Add description column
	col.iSubItem = 5;
	col.cx = 100;
	col.pszText = P4_HISTORY_DESC;
	ListView_InsertColumn( list, 5, &col );
}

INT_PTR CALLBACK HistoryDialog::DialogWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND historyList = GetDlgItem( hWnd, IDC_HISTORY_LIST );
	HWND historyDescLabel = GetDlgItem( hWnd, IDC_HISTORY_DESC_TEXT );
	HWND historyDesc = GetDlgItem( hWnd, IDC_HISTORY_DESC_EDIT );
	HWND viewButton = GetDlgItem( hWnd, ID_VIEW );
	HWND syncButton = GetDlgItem( hWnd, ID_SYNC );
	HWND cancelButton = GetDlgItem( hWnd, IDCANCEL );

	switch(uMsg) {

	case WM_INITDIALOG:	{
		HistoryDialog *dialog = (HistoryDialog *)lParam;
		SaveDialogObject(hWnd, dialog);

		SetupList( historyList );

		PerforceResults *results = dialog->Results();
		if (results->Count() > 0) {
			UniStrDict *dict = (UniStrDict *)results->Get( 0 );
			StrPtr *depotFile = dict->GetVar ("depotFile");
			if ( depotFile != NULL ) {
				// Set dialog title with filename
				UniStrBuf title;
				title.Set( P4_HISTORY_HIST );
				title.Append( dict->GetVar ("depotFile") );
				SetWindowText(hWnd, title.Text());

				// Populate list box with results
				if ( dict->GetVar( "rev0" ) != NULL ) {
					InsertListData( historyList, dict );
				}
			}
		}

		// Set size of last column to fit control
		ListView_SetColumnWidth(historyList, 5, LVSCW_AUTOSIZE_USEHEADER);

		// Select first item in list
		ListView_SetItemState(historyList, 0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

		SetFocus(historyList);

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( historyList, RESIZE_WIDTH | RESIZE_HEIGHT_HALF );
		SetResizeFlags( historyDesc, RESIZE_TOP_HALF | RESIZE_WIDTH | RESIZE_HEIGHT_HALF );
		SetResizeFlags( historyDescLabel, RESIZE_TOP_HALF );
		SetResizeFlags( viewButton, RESIZE_TOP );
		SetResizeFlags( syncButton, RESIZE_TOP );
		SetResizeFlags( cancelButton, RESIZE_TOP | RESIZE_LEFT );

		return true;
		break;
	}

	case WM_GETMINMAXINFO:
		SetMinimumSize( hWnd, (LPMINMAXINFO) lParam );
		return true;
		break;

	case WM_SIZE:
		Resize( hWnd, lParam );
		return true;
		break;

	case WM_PAINT: {
		HistoryDialog *dialog = ( HistoryDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_NOTIFY: {
		HistoryDialog *dialog = ( HistoryDialog * )DialogObject( hWnd );
		LPNMHDR listEvent = (LPNMHDR) lParam;
		// If history list event
		if ((int)wParam  == IDC_HISTORY_LIST) {

			// If selected item has changed
			if ( listEvent->code == LVN_ITEMCHANGED ) {
				LPNMLISTVIEW stateChange = (LPNMLISTVIEW) lParam;
				if ( stateChange->uNewState & LVIS_SELECTED ) {

					// Get full description stored in item user data
					LVITEM item;
					item.mask = LVIF_PARAM;
					item.iItem = stateChange->iItem;
					item.iSubItem = 0;
					ListView_GetItem( listEvent->hwndFrom, &item );

					UniStrDict *dict = (UniStrDict *)dialog->Results()->Get( 0 );
					StrBuf key;

					// Display the full description.
					StrPtr *desc = dict->GetVar( key << "desc" << (int)item.lParam );
					TCHAR *buff = ReplaceNewlines( desc, _T("\r\n") );
					SetDlgItemText( hWnd, IDC_HISTORY_DESC_EDIT, buff );
					delete []buff;  // Free buffer created by ReplaceNewlines

					// Set the description title
					key.Clear();
					StrPtr *changelist = dict->GetVar( key << "change" << (int)item.lParam );
					UniStrBuf title;
					title.Set( P4_HISTORY_DESC_CHG );
					title.Append( changelist );
					SetDlgItemText( hWnd, IDC_HISTORY_DESC_TEXT, title.Text() );
				}
				return 0;
			}
			// Column header click
			else if ( listEvent->code == LVN_COLUMNCLICK ) {
				LPNMLISTVIEW item = ( LPNMLISTVIEW ) lParam;
				dialog->sortCol = item->iSubItem;
				// Get column for sort and change order
				switch ( dialog->sortCol ) {
				case 0:
					dialog->revOrder *= -1;
					dialog->sortOrder = dialog->revOrder;
					break;
				case 1:
					dialog->changeOrder *= -1;
					dialog->sortOrder = dialog->changeOrder;
					break;
				case 2:
					dialog->dateOrder *= -1;
					dialog->sortOrder = dialog->dateOrder;
					break;
				case 3:
					dialog->userOrder *= -1;
					dialog->sortOrder = dialog->userOrder;
					break;
				case 4:
					dialog->actionOrder *= -1;
					dialog->sortOrder = dialog->actionOrder;
					break;
				case 5:
					dialog->descOrder *= -1;
					dialog->sortOrder = dialog->descOrder;
					break;
				}
				ListView_SortItems( historyList, Compare, (LPARAM) dialog );
			}
		}
		}
		break;

	case WM_CTLCOLORSTATIC:
		// Make the background of edit boxes white instead of the read-only grey.
		if ( ( HWND )lParam == GetDlgItem( hWnd, IDC_HISTORY_DESC_EDIT ) ) {
			return GetStockObject( WHITE_BRUSH ) != NULL;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		// View button clicked
		case ID_VIEW: {
			HistoryDialog *dialog = ( HistoryDialog * )DialogObject( hWnd );
			HWND historyList = GetDlgItem( hWnd, IDC_HISTORY_LIST );
			int revision = dialog->GetSelectedRevision( historyList );
			if ( revision > 0 ) {
				dialog->CreateTempFile( revision );
				dialog->SetExitCode( ID_VIEW );
				FinishDialog( hWnd, TRUE );
			}
			return true;
			}

		// Sync button clicked
		case ID_SYNC: {
			HistoryDialog *dialog = ( HistoryDialog * )DialogObject( hWnd );
			HWND historyList = GetDlgItem( hWnd, IDC_HISTORY_LIST );
			int revision = dialog->GetSelectedRevision( historyList );
			if ( revision > 0 ) {
				dialog->SyncFile( revision );
				dialog->SetExitCode( ID_SYNC );
				FinishDialog( hWnd, TRUE );
			}
			return true;
			}

		case IDOK:
			FinishDialog( hWnd, TRUE );
			return true;
		case IDCANCEL:
			FinishDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}

int CALLBACK HistoryDialog::Compare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	HistoryDialog *dialog = (HistoryDialog *)lParamSort;
	UniStrDict *dict = (UniStrDict *)dialog->Results()->Get( 0 );
	bool strval;
	char *name;
	switch ( dialog->sortCol ) {
	case 0:
		strval = false;
		name = "rev";
		break;
	case 1:
		strval = false;
		name = "change";
		break;
	case 2:
		strval = false;
		name = "time";
		break;
	case 3:
		strval = true;
		name = "user";
		break;
	case 4:
		strval = true;
		name = "action";
		break;
	case 5:
		strval = true;
		name = "desc";
		break;
	}
	StrBuf key;
	StrPtr *val1 = dict->GetVar( key << name << (int)lParam1 );
	key.Clear();
	StrPtr *val2 = dict->GetVar( key << name << (int)lParam2 );
	int result;
	if ( strval ) {
		result = _tcsicmp( (TCHAR *)val1->Text(), (TCHAR *)val2->Text() );
	}
	else {
		long num1 = _ttol( (TCHAR *)val1->Text() );
		long num2 = _ttol( (TCHAR *)val2->Text() );
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

TCHAR *HistoryDialog::AsciiTime( StrPtr *time )
{
	time_t secs = _ttoi( (TCHAR *)time->Text() );
	struct tm *ltime = localtime( &secs );
	static TCHAR buff[128];		// static buffer
	_stprintf( buff, _T("%04d/%02d/%02d %02d:%02d:%02d"),
		          ltime->tm_year + 1900, ltime->tm_mon + 1,
				  ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec );

	return buff;
}

void HistoryDialog::SetExitCode( int code ) {
	exitCode = code;
}

int HistoryDialog::ExitCode() {
	return exitCode;
}

