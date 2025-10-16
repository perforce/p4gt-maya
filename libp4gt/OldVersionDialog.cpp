/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "OldVersionDialog.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

OldVersionDialog::OldVersionDialog( PluginOperations *ops, TCHAR *filename, StrPtr *have, StrPtr *head ) : Dialog( ops, IDD_OLD_VERSION_DIALOG, DialogWndProc ) {
	this->filename = filename;
	this->have = have;
	this->head = head;
}

INT_PTR CALLBACK OldVersionDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg) {

	case WM_INITDIALOG:	{
		OldVersionDialog *dialog = ( OldVersionDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		UniStrBuf buff;

		buff.Set( P4_OLDVERSION_DOC );
		buff.Append( dialog->Filename() );
		buff.Append( P4_OLDVERSION_CHECKED_IN );
		SetDlgItemText( hWnd, IDC_DOCUMENT_TEXT, buff.Text() );
		buff.Clear();

		buff.Set( P4_OLDVERSION_OLD_REV );
		buff.Append( dialog->HaveRevision() );
		buff.Append( P4_OLDVERSION_HEAD_REV );
		buff.Append( dialog->HeadRevision() );
		buff.Append( _T(".") );
		SetDlgItemText( hWnd, IDC_VERSION_TEXT, buff.Text() );
		Centre( hWnd );
		}
		break;

	case WM_COMMAND:
		OldVersionDialog *dialog = ( OldVersionDialog * )DialogObject( hWnd );
		switch ( LOWORD( wParam ) ) {
		case IDSYNC:
			dialog->SetExitCode( IDSYNC );
			EndDialog( hWnd, TRUE) ;
			return true;
		case IDCHECKOUT:
			dialog->SetExitCode( IDCHECKOUT );
			EndDialog( hWnd, TRUE );
			return true;
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			return true;
		}
		break;
	}

	return false;
}



void OldVersionDialog::SetExitCode( int code ) {
	exitCode = code;
}

int OldVersionDialog::ExitCode() {
	return exitCode;
}

TCHAR *OldVersionDialog::Filename() {
	return filename;
}

StrPtr *OldVersionDialog::HaveRevision() {
	return have;
}

StrPtr *OldVersionDialog::HeadRevision() {
	return head;
}
