/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "TabbedOptionsDialog.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

extern HINSTANCE hInstance;

typedef HRESULT (WINAPI * ENABLETHEMEDIALOGTEXTURE) (HWND, DWORD);

TabbedOptionsDialog::TabbedOptionsDialog( PluginOperations *ops, PerforceOptions *options, bool refs ) : Dialog( ops, IDD_TABBED_OPTIONS_DIALOG, DialogWndProc )
{
	this->options = options;
	this->refs = refs;
}

void TabbedOptionsDialog::SetCheckSelection( int value, HWND check )
{
	SendMessage( check, (UINT)BM_SETCHECK, (WPARAM) value ? BST_CHECKED : BST_UNCHECKED, (LPARAM) 0);
}

int TabbedOptionsDialog::GetCheckSelection( HWND check )
{
	if ( SendMessage( check, (UINT)BM_GETSTATE, (WPARAM)0, (LPARAM)0 ) == BST_CHECKED ) {
		return 1;
	}
	else {
		return 0;
	}
}

void TabbedOptionsDialog::SetRadioSelection( int value, HWND radio1, HWND radio2, HWND radio3 )
{
	if ( value == 2) {
		SendMessage( radio3, (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM) 0);
	}
	else if ( value == 0 ) {
		SendMessage( radio2, (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM) 0);
	}
	else {
		SendMessage( radio1, (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM) 0);
	}
}

int TabbedOptionsDialog::GetRadioSelection( HWND radio1, HWND radio2, HWND radio3 )
{
	if ( SendMessage( radio3, (UINT)BM_GETSTATE, (WPARAM)0, (LPARAM)0 ) == BST_CHECKED ) {
		return 2;
	}
	else if ( SendMessage( radio2, (UINT)BM_GETSTATE, (WPARAM)0, (LPARAM)0 ) == BST_CHECKED ) {
		return 0;
	}
	else {
		return 1;
	}
}

void TabbedOptionsDialog::SaveOptions( HWND hWnd )
{
	TabbedOptionsDialog *dialog = (TabbedOptionsDialog *)DialogObject( hWnd );
	PerforceOptions *options = dialog->Options();

	int result = GetRadioSelection( GetDlgItem( dialog->page1, IDC_ADDING_ALWAYS_RADIO ),
									GetDlgItem( dialog->page1, IDC_ADDING_NEVER_RADIO ),
									GetDlgItem( dialog->page1, IDC_ADDING_PROMPT_RADIO ) );
	options->SetOptionValue( CHECK_IN_ON_ADD_OPTION, result );

	result = GetRadioSelection( GetDlgItem( dialog->page1, IDC_LOCK_ALWAYS_RADIO ),
								GetDlgItem( dialog->page1, IDC_LOCK_NEVER_RADIO ),
								GetDlgItem( dialog->page1, IDC_LOCK_PROMPT_RADIO ) );
	options->SetOptionValue( LOCK_ON_CHECK_OUT_OPTION, result );

	result = GetCheckSelection( GetDlgItem( dialog->page1, IDC_OTHER_DOCS_CHECK ) );
	options->SetOptionValue( OTHER_DOCUMENTS_OPTION, result );

	result = GetCheckSelection( GetDlgItem( dialog->page1, IDC_CONFIRM_CHECK ) );
	options->SetOptionValue( SHOW_CONFIRMATION_OPTION, result );

	result = GetRadioSelection( GetDlgItem( dialog->page2, IDC_OPENING_ALWAYS_RADIO ),
									GetDlgItem( dialog->page2, IDC_OPENING_NEVER_RADIO ),
									GetDlgItem( dialog->page2, IDC_OPENING_PROMPT_RADIO ) );
	options->SetOptionValue( CHECK_OUT_ON_OPEN_OPTION, result );

	result = GetRadioSelection( GetDlgItem( dialog->page2, IDC_REOPENING_ALWAYS_RADIO ),
									GetDlgItem( dialog->page2, IDC_REOPENING_NEVER_RADIO ),
									GetDlgItem( dialog->page2, IDC_REOPENING_PROMPT_RADIO ) );
	options->SetOptionValue( CHECK_OUT_ON_REOPEN_OPTION, result );

	result = GetRadioSelection( GetDlgItem( dialog->page2, IDC_CLOSING_ALWAYS_RADIO ),
									GetDlgItem( dialog->page2, IDC_CLOSING_NEVER_RADIO ),
									GetDlgItem( dialog->page2, IDC_CLOSING_PROMPT_RADIO ) );
	options->SetOptionValue( CHECK_IN_ON_CLOSE_OPTION, result );

	result = GetCheckSelection( GetDlgItem( dialog->page2, IDC_REVERT_ON_CLOSE_CHECK ) ),
	options->SetOptionValue( REVERT_ON_CLOSE_OPTION, result );

	if ( dialog->HasRefs() ) {
		result = GetRadioSelection( GetDlgItem( dialog->page2, IDC_SYNC_ALWAYS_RADIO ),
									GetDlgItem( dialog->page2, IDC_SYNC_NEVER_RADIO ),
									GetDlgItem( dialog->page2, IDC_SYNC_PROMPT_RADIO ) );
		options->SetOptionValue( SYNC_FILE_REF_OPTION, result );
	}
}

INT_PTR CALLBACK TabbedOptionsDialog::TabWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg) {
	case WM_INITDIALOG: {
		HWND parent = GetParent( hWnd );
		TabbedOptionsDialog *dialog = (TabbedOptionsDialog *)DialogObject( parent );
		dialog->SaveDialogObject( hWnd, dialog );

		// Get controls on tabs to render correctly under XP
		HINSTANCE dll = LoadLibrary( _T("uxtheme.dll") );
		// Don't enable themes for Maya.
		if ( _tcscmp( dialog->ops->GetHost()->GetName(), _T("P4GT-Maya") ) != 0 && dll != NULL ) {
			ENABLETHEMEDIALOGTEXTURE func;
			func = (ENABLETHEMEDIALOGTEXTURE)GetProcAddress( dll, "EnableThemeDialogTexture" );
			if ( func != NULL ) {
				// EnableThemeDialogTexture( hWnd, ETDT_ENABLETAB );
				func( hWnd, 0x02 | 0x04 );
			}
			FreeLibrary( dll );
		}
		return true;
	}
	case WM_COMMAND:
		if ( HIWORD( wParam ) == BN_CLICKED ) {
			HWND parent = GetParent( hWnd );
			HWND apply = GetDlgItem( parent, IDAPPLY );
			if ( apply ) {
				EnableWindow( apply, TRUE );
				return true;
			}
		}
	}

	return false;
}

INT_PTR CALLBACK TabbedOptionsDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HWND tabCtrl = GetDlgItem( hWnd, IDC_OPTIONS_TAB );

	switch(uMsg) {

	case WM_INITDIALOG: {
		TabbedOptionsDialog *dialog = ( TabbedOptionsDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		TCITEM tab;
		tab.mask = TCIF_TEXT;
		tab.pszText = P4_TABBEDOPTIONS_CHECKIN_CHECKOUT;
		TabCtrl_InsertItem( tabCtrl, 0, &tab );
		tab.pszText = P4_TABBEDOPTIONS_OPEN_CLOSE;
		TabCtrl_InsertItem( tabCtrl, 1, &tab );

		dialog->page1 = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_OPTIONS_PAGE1_DIALOG ), hWnd, TabWndProc );
		dialog->page2 = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_OPTIONS_PAGE2_DIALOG ), hWnd, TabWndProc );
		RECT rect;
		GetControlPos( hWnd, GetDlgItem( hWnd, IDC_PLACEHOLDER_LABEL ), &rect );
		SetWindowPos( dialog->page1, GetDlgItem( hWnd, IDC_PLACEHOLDER_LABEL ), rect.left, rect.top, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE );
		SetWindowPos( dialog->page2, GetDlgItem( hWnd, IDC_PLACEHOLDER_LABEL ), rect.left, rect.top, 0, 0, SWP_NOSIZE|SWP_NOACTIVATE );

		PerforceOptions *options = dialog->Options();
		SetRadioSelection( options->GetOptionValue( CHECK_IN_ON_ADD_OPTION ),
						   GetDlgItem( dialog->page1, IDC_ADDING_ALWAYS_RADIO ),
						   GetDlgItem( dialog->page1, IDC_ADDING_NEVER_RADIO ),
						   GetDlgItem( dialog->page1, IDC_ADDING_PROMPT_RADIO ) );
		SetRadioSelection( options->GetOptionValue( LOCK_ON_CHECK_OUT_OPTION ),
						   GetDlgItem( dialog->page1, IDC_LOCK_ALWAYS_RADIO ),
						   GetDlgItem( dialog->page1, IDC_LOCK_NEVER_RADIO ),
						   GetDlgItem( dialog->page1, IDC_LOCK_PROMPT_RADIO ) );
		SetCheckSelection( options->GetOptionValue( OTHER_DOCUMENTS_OPTION ), GetDlgItem( dialog->page1, IDC_OTHER_DOCS_CHECK ) );
		SetCheckSelection( options->GetOptionValue( SHOW_CONFIRMATION_OPTION ), GetDlgItem( dialog->page1, IDC_CONFIRM_CHECK ) );

		SetRadioSelection( options->GetOptionValue( CHECK_OUT_ON_OPEN_OPTION ),
						   GetDlgItem( dialog->page2, IDC_OPENING_ALWAYS_RADIO ),
						   GetDlgItem( dialog->page2, IDC_OPENING_NEVER_RADIO ),
						   GetDlgItem( dialog->page2, IDC_OPENING_PROMPT_RADIO ) );
		SetRadioSelection( options->GetOptionValue( CHECK_OUT_ON_REOPEN_OPTION ),
						   GetDlgItem( dialog->page2, IDC_REOPENING_ALWAYS_RADIO ),
						   GetDlgItem( dialog->page2, IDC_REOPENING_NEVER_RADIO ),
						   GetDlgItem( dialog->page2, IDC_REOPENING_PROMPT_RADIO ) );
		SetRadioSelection( options->GetOptionValue( CHECK_IN_ON_CLOSE_OPTION ),
						   GetDlgItem( dialog->page2, IDC_CLOSING_ALWAYS_RADIO ),
						   GetDlgItem( dialog->page2, IDC_CLOSING_NEVER_RADIO ),
						   GetDlgItem( dialog->page2, IDC_CLOSING_PROMPT_RADIO ) );
		SetCheckSelection( options->GetOptionValue( REVERT_ON_CLOSE_OPTION ), GetDlgItem( dialog->page2, IDC_REVERT_ON_CLOSE_CHECK ) );
		if ( dialog->HasRefs() ) {
			SetRadioSelection( options->GetOptionValue( SYNC_FILE_REF_OPTION ),
							   GetDlgItem( dialog->page2, IDC_SYNC_ALWAYS_RADIO ),
							   GetDlgItem( dialog->page2, IDC_SYNC_NEVER_RADIO ),
							   GetDlgItem( dialog->page2, IDC_SYNC_PROMPT_RADIO ) );
		}
		else {
			ShowWindow( GetDlgItem( dialog->page2, IDC_EXTERNAL_LABEL ), SW_HIDE );
			ShowWindow( GetDlgItem( dialog->page2, IDC_SYNC_ALWAYS_RADIO ), SW_HIDE );
			ShowWindow( GetDlgItem( dialog->page2, IDC_SYNC_NEVER_RADIO ), SW_HIDE );
			ShowWindow( GetDlgItem( dialog->page2, IDC_SYNC_PROMPT_RADIO ), SW_HIDE );
		}

		EnableWindow( GetDlgItem( hWnd, IDAPPLY ) , FALSE );
		ShowWindow( dialog->page1, SW_SHOW );
		Centre( hWnd );
		break;
	}

	case WM_NOTIFY: {
		LPNMHDR listEvent = ( LPNMHDR ) lParam;
		TabbedOptionsDialog *dialog = (TabbedOptionsDialog *)DialogObject( hWnd );
		if ((int)wParam  == IDC_OPTIONS_TAB && listEvent->code == TCN_SELCHANGE ) {
			int selection = TabCtrl_GetCurSel( tabCtrl );
			if ( selection == 0 ) {
				ShowWindow( dialog->page2, SW_HIDE );
				ShowWindow( dialog->page1, SW_SHOW );
			}
			else {
				ShowWindow( dialog->page1, SW_HIDE );
				ShowWindow( dialog->page2, SW_SHOW );
			}
		}
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDAPPLY:
			SaveOptions( hWnd );
			EnableWindow( GetDlgItem( hWnd, IDAPPLY ) , FALSE );
			return true;
		case IDOK:
			SaveOptions( hWnd );
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

PerforceOptions *TabbedOptionsDialog::Options()
{
	return options;
}