/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "ResolveDialog.h"

#define BUFSIZE		80

ResolveDialog::ResolveDialog( PluginOperations *ops ) : Dialog( ops, IDD_RESOLVE_DIALOG, DialogWndProc ) 
{
}

INT_PTR CALLBACK ResolveDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{

	switch(uMsg) {

	case WM_INITDIALOG:	{	
		ResolveDialog *dialog = ( ResolveDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		CheckRadioButton( hWnd, IDC_CANCEL_RADIO, IDC_UNDO_RADIO, IDC_CANCEL_RADIO );
		Centre( hWnd );
		}
		break;

	case WM_COMMAND:
		ResolveDialog *dialog = ( ResolveDialog * )DialogObject( hWnd );
		switch ( LOWORD( wParam ) ) {
		case IDOK:
			if ( IsDlgButtonChecked( hWnd, IDC_CANCEL_RADIO ) == BST_CHECKED ) {
				dialog->SetOption( IDC_CANCEL_RADIO );
			}
			else if ( IsDlgButtonChecked( hWnd, IDC_CHECK_IN_RADIO ) == BST_CHECKED ) {
				dialog->SetOption( IDC_CHECK_IN_RADIO );
			}
			else if ( IsDlgButtonChecked( hWnd, IDC_UNDO_RADIO ) == BST_CHECKED ) {
				dialog->SetOption( IDC_UNDO_RADIO );
			}
			EndDialog( hWnd, TRUE );
			return true;
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}


