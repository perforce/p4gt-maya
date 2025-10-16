/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "OptionsDialog.h"

OptionsDialog::OptionsDialog( PluginOperations *ops, PerforceOptions *options, bool refs ) : Dialog( ops, IDD_OPTIONS_DIALOG, DialogWndProc )
{
	this->options = options;
	this->refs = refs;
}

void OptionsDialog::InitCombo( HWND dialog, int combo, bool prompt )
{
	SendDlgItemMessage( dialog, combo, CB_ADDSTRING, 0, (LPARAM)"NEVER" );
	SendDlgItemMessage( dialog, combo, CB_ADDSTRING, 0, (LPARAM)"ALWAYS" );
	if ( prompt ) {
		SendDlgItemMessage( dialog, combo, CB_ADDSTRING, 0, (LPARAM)"PROMPT" );
	}
}

void OptionsDialog::SetSelection( HWND dialog, int combo, UINT i )
{
	SendDlgItemMessage( dialog, combo, CB_SETCURSEL, (WPARAM)i, 0 );
}

UINT OptionsDialog::GetSelection( HWND dialog, int combo )
{
	// Truncation of data probably ok. 
#pragma warning(push)
#pragma warning(disable: 4244)
	return SendDlgItemMessage( dialog, combo, CB_GETCURSEL, 0, 0 );
#pragma warning(pop)
}

INT_PTR CALLBACK OptionsDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg) {

	case WM_INITDIALOG: {
		OptionsDialog *dialog = ( OptionsDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		PerforceOptions *options = dialog->Options();

		InitCombo( hWnd, IDC_SHOW_DIALOGS_COMBO, false );
		SetSelection( hWnd, IDC_SHOW_DIALOGS_COMBO, options->GetOptionValue( SHOW_CONFIRMATION_OPTION ) );

		InitCombo( hWnd, IDC_OTHER_DOCS_COMBO, false );
		SetSelection( hWnd, IDC_OTHER_DOCS_COMBO, options->GetOptionValue( OTHER_DOCUMENTS_OPTION ) );

		InitCombo( hWnd, IDC_AUTO_LOCK_COMBO, false );
		SetSelection( hWnd, IDC_AUTO_LOCK_COMBO, options->GetOptionValue( LOCK_ON_CHECK_OUT_OPTION ) );

		InitCombo( hWnd, IDC_PROMPT_CLOSE_COMBO, true );
		SetSelection( hWnd, IDC_PROMPT_CLOSE_COMBO, options->GetOptionValue( CHECK_IN_ON_CLOSE_OPTION ) );

		InitCombo( hWnd, IDC_PROMPT_OPEN_COMBO, true );
		SetSelection( hWnd, IDC_PROMPT_OPEN_COMBO, options->GetOptionValue( CHECK_OUT_ON_OPEN_OPTION ) );

		InitCombo( hWnd, IDC_PROMPT_REOPEN_COMBO, true );
		SetSelection( hWnd, IDC_PROMPT_REOPEN_COMBO, options->GetOptionValue( CHECK_OUT_ON_REOPEN_OPTION ) );

		InitCombo( hWnd, IDC_CHECK_IN_ADD_COMBO, true );
		SetSelection( hWnd, IDC_CHECK_IN_ADD_COMBO, options->GetOptionValue( CHECK_IN_ON_ADD_OPTION ) );

		InitCombo( hWnd, IDC_PROMPT_CLOSE_COMBO, true );
		SetSelection( hWnd, IDC_PROMPT_CLOSE_COMBO, options->GetOptionValue( REVERT_ON_CLOSE_OPTION ) );

		if ( dialog->HasRefs() ) {
			InitCombo( hWnd, IDC_FILE_REF_COMBO, true );
			SetSelection( hWnd, IDC_FILE_REF_COMBO, options->GetOptionValue( SYNC_FILE_REF_OPTION ) );
		}
		else {
			ShowWindow( GetDlgItem( hWnd, IDC_REFS_STATIC ), FALSE );
			ShowWindow( GetDlgItem( hWnd, IDC_FILE_REF_COMBO ), FALSE );
		}

		Centre( hWnd );
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: {
			OptionsDialog *dialog = (OptionsDialog *)DialogObject( hWnd );

			PerforceOptions *options = dialog->Options();

			options->SetOptionValue( SHOW_CONFIRMATION_OPTION, GetSelection( hWnd, IDC_SHOW_DIALOGS_COMBO ) );
			options->SetOptionValue( OTHER_DOCUMENTS_OPTION, GetSelection( hWnd, IDC_OTHER_DOCS_COMBO ) );
			options->SetOptionValue( LOCK_ON_CHECK_OUT_OPTION, GetSelection( hWnd, IDC_AUTO_LOCK_COMBO ) );
			options->SetOptionValue( CHECK_IN_ON_CLOSE_OPTION, GetSelection( hWnd, IDC_PROMPT_CLOSE_COMBO ) );
			options->SetOptionValue( CHECK_OUT_ON_OPEN_OPTION, GetSelection( hWnd, IDC_PROMPT_OPEN_COMBO ) );
			options->SetOptionValue( CHECK_OUT_ON_REOPEN_OPTION, GetSelection( hWnd, IDC_PROMPT_REOPEN_COMBO ) );
			options->SetOptionValue( CHECK_IN_ON_ADD_OPTION, GetSelection( hWnd, IDC_CHECK_IN_ADD_COMBO ) );
			options->SetOptionValue( REVERT_ON_CLOSE_OPTION, GetSelection( hWnd, IDC_PROMPT_CLOSE_COMBO ) );
			if ( dialog->HasRefs() ) {
				options->SetOptionValue( SYNC_FILE_REF_OPTION, GetSelection( hWnd, IDC_FILE_REF_COMBO ) );
			}

			EndDialog( hWnd, TRUE );
			return true;
			}
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			return true;
		}
		break;
	}

	return false;
}

PerforceOptions *OptionsDialog::Options()
{
	return options;
}
