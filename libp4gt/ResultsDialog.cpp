/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "ResultsDialog.h"
#include "debug.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

ResultsDialog::ResultsDialog( PluginOperations *ops, TCHAR *op, PerforceResults *results, const StrPtr *currDir ) : Dialog( ops, IDD_RESULTS_DIALOG, DialogWndProc )
{
	this->op = op;
	this->results = results;
	this->currDir = currDir;
}

void ResultsDialog::ParseInfo( PerforceResults &results, UniStrBuf *msg )
{
	StrPtr *value;
	UniStrDict *dict;

	// New style info output
	if ( results.Count() == 1 ) {
		dict = (UniStrDict *)results.Get( 0 );
		value = dict->GetVar( "userName" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_USER, value );
		}
		value = dict->GetVar( "clientName" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_CLIENT, value );
		}
		value = dict->GetVar( "clientHost" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_CLIENT_HOST, value );
		}
		value = dict->GetVar( "clientRoot" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_CLIENT_ROOT, value );
		}

		AppendLine( msg, P4_RESULTS_CLIENT_CWD, (StrPtr *)GetCwd() );

		value = dict->GetVar( "clientAddress" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_CLIENT_ADDR, value );
		}
		value = dict->GetVar( "unicode" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_UNICODE, value );
		}
		value = dict->GetVar( "serverAddress" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_SERVER_ADDR, value );
		}
		value = dict->GetVar( "serverRoot" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_SERVER_ROOT, value );
		}
		value = dict->GetVar( "serverDate" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_SERVER_DATE, value );
		}
		value = dict->GetVar( "serverVersion" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_SERVER_VERS, value );
		}
		value = dict->GetVar( "serverLicense" );
		if ( value ) {
			AppendLine( msg, P4_RESULTS_SERVER_LIC, value );
		}
	}
	// Old style info output
	else {
		for ( int i=0; i<results.Count(); i++ ) {
			dict = (UniStrDict *)results.Get( i );
			value = dict->GetVar( "msg" );
			if ( value ) {
				AppendLine( msg, _T(""), value );
			}
		}
	}
}

void ResultsDialog::AppendLine( UniStrBuf *buff, TCHAR *var, StrPtr *value )
{
	buff->Append( var );
	buff->Append( value );
	buff->Append( _T("\r\n") );
}

INT_PTR CALLBACK ResultsDialog::DialogWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND results = GetDlgItem( hWnd, IDC_RESULTS_EDIT );
	HWND okButton = GetDlgItem( hWnd, IDOK );

	switch(uMsg) {

	case WM_INITDIALOG: {
		ResultsDialog *dialog = ( ResultsDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		TCHAR *op = dialog->Operator();
		if ( _tcscmp( op, _T("submit") ) == 0 ) {
			SetWindowText( hWnd, P4_RESULTS_CONF_CHECKIN );
		}
		else if ( _tcscmp( op, _T("last") ) == 0 ) {
			SetWindowText( hWnd, P4_RESULTS_RESULT_CMD );
		}
		else if ( _tcscmp( op, _T("info") ) == 0 ) {
			SetWindowText( hWnd, P4_RESULTS_CONN_INFO );
		}
		if ( _tcscmp( op, _T("info") ) == 0 ) {
			if ( dialog->Results()->Count() > 0 ) {
				UniStrBuf msg;
				dialog->ParseInfo( *dialog->Results(), &msg );
				SetWindowText( results, (TCHAR *)msg.Text() );
			}
		}
		else {
			SetWindowText( results, dialog->Results()->GetText() );
		}
		SetFocus( okButton );

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( results, RESIZE_WIDTH | RESIZE_HEIGHT );
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
		ResultsDialog *dialog = ( ResultsDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_CTLCOLORSTATIC:
		// Make the background of edit boxes white instead of the read-only grey.
		if ( ( HWND )lParam == results ) {
			return GetStockObject( WHITE_BRUSH ) != NULL;
		}
		break;

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
