/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "ExternalRefDialog.h"
#include "PerforceFileStat.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

#include <commctrl.h>

ExternalRefDialog::ExternalRefDialog( PluginOperations *ops, PerforceResults *results ) : Dialog( ops, IDD_EXTERNAL_REF_DIALOG, DialogWndProc ), syncFiles( NULL )
{
	this->results = results;
}

ExternalRefDialog::~ExternalRefDialog()
{
	if ( syncFiles ) {
		delete syncFiles;
	}
}

INT_PTR CALLBACK ExternalRefDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HWND fileList = GetDlgItem( hWnd, IDC_FILES_LIST );
	HWND okButton = GetDlgItem( hWnd, IDOK );
	HWND cancelButton = GetDlgItem( hWnd, IDCANCEL );

	switch(uMsg) {

	case WM_INITDIALOG: {
		ExternalRefDialog *dialog = ( ExternalRefDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		DWORD style = ListView_GetExtendedListViewStyle( fileList );
		style |= LVS_EX_CHECKBOXES;
		ListView_SetExtendedListViewStyle( fileList, style );

		// Filename column
		LV_COLUMN col;
		col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		col.fmt = LVCFMT_LEFT;
		col.cx = 200;
		col.pszText = P4_EXTERNALREF_NAME;
		col.iSubItem = 0;
		ListView_InsertColumn( fileList, 0, &col );

		// Have revision column
		col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		col.fmt = LVCFMT_RIGHT;
		col.cx = 110;
		col.iSubItem = 1;
		col.pszText = P4_EXTERNALREF_HAVE;
		ListView_InsertColumn( fileList, 1, &col );

		// Head revision column
		col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		col.fmt = LVCFMT_RIGHT;
		col.cx = 110;
		col.iSubItem = 2;
		col.pszText = P4_EXTERNALREF_HEAD;
		ListView_InsertColumn( fileList, 2, &col );

		LV_ITEM item;
		item.mask = LVIF_TEXT;
		LV_FINDINFO find;
		find.flags = LVFI_STRING;

		PerforceResults *results = dialog->Results();
		PerforceFileStat refStat;
		for ( int i=0; i<results->Count(); i++ ) {
			UniStrDict *dict = ( UniStrDict * )results->Get( i );
			if ( dict->GetVar( "depotFile" ) != NULL ) {
				refStat.Set( dict );
				if ( refStat.IsLatestVersion() ) {
					continue;
				}

				// Check file does not already exist
				find.psz = (TCHAR *)refStat.ClientPath()->Text();
				if ( ListView_FindItem( fileList, -1, &find ) > -1 ) {
					continue;
				}

				item.pszText = (TCHAR *)refStat.ClientPath()->Text();
				item.iItem = i;
				item.iSubItem = 0;
				item.iItem = ListView_InsertItem( fileList, &item );

				if ( refStat.HaveRevision() ) {
					item.pszText = (TCHAR *)refStat.HaveRevision()->Text();
				}
				else {
					item.pszText = _T("0");
				}
				item.iSubItem = 1;
				ListView_SetItem( fileList, &item );

				item.pszText = (TCHAR *)refStat.HeadRevision()->Text();
				item.iSubItem = 2;
				ListView_SetItem( fileList, &item );

				ListView_SetCheckState( fileList, item.iItem, TRUE );
			}
		}

		ListView_SetColumnWidth( fileList, 0, LVSCW_AUTOSIZE );

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( fileList, RESIZE_WIDTH | RESIZE_HEIGHT );
		SetResizeFlags( okButton, RESIZE_TOP | RESIZE_LEFT_HALF );
		SetResizeFlags( cancelButton, RESIZE_TOP | RESIZE_LEFT_HALF );

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
		ExternalRefDialog *dialog = ( ExternalRefDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case IDOK: {
			ExternalRefDialog *dialog = ( ExternalRefDialog * )DialogObject( hWnd );
			HWND fileList = GetDlgItem( hWnd, IDC_FILES_LIST );

			TCHAR name[MAX_PATH];
			LV_ITEM item;
			item.mask = LVIF_TEXT;
			item.iSubItem = 0;
			item.pszText = name;
			item.cchTextMax = sizeof(name);

			StrBufArray *files = new StrBufArray();
			int count = ListView_GetItemCount( fileList );
			for ( int i=0; i<count; i++ ) {
				if ( ListView_GetCheckState( fileList, i ) == TRUE ) {
					item.iItem = i;
					ListView_GetItem( fileList, &item );
					files->Put( name );
				}
			}
			dialog->SetSyncFiles( files );
			FinishDialog( hWnd, TRUE );
			return true;
		}
		case IDCANCEL:
			FinishDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}

