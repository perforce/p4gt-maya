/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "OpenFilesDialog.h"

#include <commctrl.h>

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif	// LANG_ja

OpenFilesDialog::OpenFilesDialog( PluginOperations *ops, PerforceResults *results ) : Dialog( ops, IDD_OPEN_FILES_DIALOG, DialogWndProc )
{
	this->results = results;
}

INT_PTR CALLBACK OpenFilesDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HWND fileList = GetDlgItem( hWnd, IDC_OPEN_FILE_LIST );
	HWND okButton = GetDlgItem( hWnd, IDOK );

	switch(uMsg) {

	case WM_INITDIALOG: {
		OpenFilesDialog *dialog = ( OpenFilesDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		SetListViewImageLists( fileList );

		LV_COLUMN col;
		col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		col.fmt = LVCFMT_LEFT;
		col.cx = 200;
		col.pszText = P4_OPENFILES;
		col.iSubItem = 0;
		ListView_InsertColumn( fileList, 0, &col );

		LV_ITEM item;
		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.iSubItem = 0;

		UniStrBuf fileText;

		PerforceResults *results = dialog->Results();
		for ( int i=0; i<results->Count(); i++ ) {
			UniStrDict *dict = ( UniStrDict * )results->Get( i );
			TCHAR *filename = (TCHAR *)( dict->GetVar ( "depotFile" ) )->Text();
			TCHAR *rev = (TCHAR *)( dict->GetVar ( "rev" ) )->Text();
			TCHAR *action = (TCHAR *)( dict->GetVar ( "action" ) )->Text();
			TCHAR *change = (TCHAR *)( dict->GetVar ( "change" ) )->Text();
			TCHAR *type = (TCHAR *)( dict->GetVar ( "type" ) )->Text();

			fileText.Clear();
			fileText.Set( filename );
			fileText.Append( _T("#") );
			fileText.Append( rev );
			fileText.Append( _T(" - ") );
			fileText.Append( action );
			fileText.Append( _T(" ") );
			fileText.Append( change );
			fileText.Append( _T(" change ") );
			fileText.Append( _T("(") );
			fileText.Append( type );
			fileText.Append( _T(")") );
			item.pszText = fileText.Text();
			item.iItem = i;
			item.iImage = FileIcon( dict->GetVar ( "depotFile" ) );
			ListView_InsertItem( fileList, &item );
		}

		ListView_SetColumnWidth( fileList, 0, LVSCW_AUTOSIZE );

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( fileList, RESIZE_WIDTH | RESIZE_HEIGHT );
		SetResizeFlags( okButton, RESIZE_TOP | RESIZE_LEFT_HALF );

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
		OpenFilesDialog *dialog = ( OpenFilesDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
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

