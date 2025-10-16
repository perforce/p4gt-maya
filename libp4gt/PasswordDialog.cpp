/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PasswordDialog.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

PasswordDialog::PasswordDialog( PluginOperations *ops, const TCHAR *user, const TCHAR *port ) : Dialog( ops, IDD_PASSWORD_DIALOG, DialogWndProc ), password( NULL )
{
	this->user = user;
	this->port = port;
}

PasswordDialog::~PasswordDialog()
{
	if ( password != NULL ) {
		delete []password;
	}
}

INT_PTR CALLBACK PasswordDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HWND userPort = GetDlgItem( hWnd, IDC_USER_PORT_TEXT1 );
	HWND passwordEdit = GetDlgItem( hWnd, IDC_PASSWORD_EDIT );

	switch(uMsg) {

	case WM_INITDIALOG:	{
		PasswordDialog *dialog = ( PasswordDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		UniStrBuf buff;
		buff.Set( P4_PASSWORD_PASSWORD );
		buff.Append( dialog->GetUser() );
		buff.Append( P4_PASSWORD_SERVER );
		buff.Append( dialog->GetPort() );
		buff.Append( _T("'.") );
		SetWindowText( userPort, buff.Text() );

		SetFocus( passwordEdit );
		Centre( hWnd );
		return true;
		break;
	}

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case IDOK: {
			PasswordDialog *dialog = ( PasswordDialog * )DialogObject( hWnd );
			// If LRESULT is over MAXINT, this will truncate the value.
			int len = (int)SendMessage( passwordEdit, WM_GETTEXTLENGTH, 0, 0 ) + 1;
			TCHAR *text = new TCHAR[ len ];
			GetWindowText( passwordEdit, text, len );
			dialog->SetPassword( text );
			EndDialog( hWnd, TRUE );
			return true;
		}
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			return true;
		}
	}

	return false;
}


