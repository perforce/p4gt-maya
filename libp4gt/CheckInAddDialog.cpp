/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "CheckInAddDialog.h"

CheckInAddDialog::CheckInAddDialog( PluginOperations *ops, PerforceOptions *options ) : Dialog( ops, IDD_CHECK_IN_ADD_DIALOG, DialogWndProc ) 
{
	this->options = options;
}

INT_PTR CALLBACK CheckInAddDialog::DialogWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HWND check = GetDlgItem( hWnd, IDC_HIDE_CHECK );

	switch(uMsg) {

	case WM_INITDIALOG: {
		CheckInAddDialog *dialog = ( CheckInAddDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		Centre( hWnd );
		return true;
		break;
	}

	case WM_COMMAND:
		CheckInAddDialog *dialog = ( CheckInAddDialog * )DialogObject( hWnd );
		switch ( LOWORD( wParam ) ) {
		case IDOK: 
			dialog->CheckHideOption( check );
			EndDialog( hWnd, TRUE );
			return true;
		case IDCANCEL:
			dialog->CheckHideOption( check );
			EndDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}

void CheckInAddDialog::CheckHideOption( HWND check )
{
	int hide =  (int)SendMessage( check, BM_GETCHECK, 0, 0 );
	if ( hide ) {
		options->SetOptionValue( CHECK_IN_ON_ADD_OPTION, FALSE );
	}
}

