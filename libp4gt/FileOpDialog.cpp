/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <time.h>

#include "FileOpDialog.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

FileOpDialog::FileOpDialog( PluginOperations *ops, TCHAR *op, TCHAR *filename, PerforceResults *results, PerforceOptions *options ) : Dialog( ops, IDD_FILE_OP_DIALOG, DialogWndProc )
{
	this->op = op;
	this->filename = filename;
	this->results = results;
	this->options = options;
}

void FileOpDialog::AppendLine( UniStrBuf *buf, TCHAR *name, StrPtr *value )
{
	buf->Append( name );
	buf->Append( value );
	buf->Append( _T("\r\n") );
}

INT_PTR CALLBACK FileOpDialog::DialogWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND fileName = GetDlgItem( hWnd, IDC_FILENAME_EDIT );
	HWND results = GetDlgItem( hWnd, IDC_RESULTS_EDIT );
	HWND hide = GetDlgItem( hWnd, IDC_HIDE_CHECK );
	HWND okButton = GetDlgItem( hWnd, IDOK );

	switch(uMsg) {

	case WM_INITDIALOG: {
		FileOpDialog *dialog = ( FileOpDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		SetWindowText( fileName, dialog->Filename() );
		TCHAR *op = dialog->Operator();
		if ( _tcscmp( op, _T("fstat") ) == 0 ) {
			UniStrBuf msg;
			if ( dialog->Results()->Count() > 0 ) {
				UniStrDict *dict = (UniStrDict *)dialog->Results()->Get( 0 );
				StrPtr *value = dict->GetVar( "depotFile" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_DEPOT_FILE, value );
				}
				value = dict->GetVar( "clientFile" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_YOUR_DOC,value );
				}
				value = dict->GetVar( "headAction" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_ACTION, value );
				}
				value = dict->GetVar( "headType" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_TYPE, value );
				}
				value = dict->GetVar( "headTime" );
				if ( value ) {
					time_t seconds = _ttol( (TCHAR *)value->Text() );
					struct tm *tm = localtime( &seconds );
					UniStrBuf timeBuff;
					TCHAR *buff = (TCHAR *)timeBuff.Alloc( 128 );
					// Format time as YY/MM/DD HH:MM:SS
					_stprintf( buff, _T("%04d/%02d/%02d %02d:%02d:%02d"),
									tm->tm_year+1900, tm->tm_mon + 1, tm->tm_mday,
									tm->tm_hour, tm->tm_min, tm->tm_sec );
					msg.Append( P4_FILEOP_LAST_MOD );
					msg.Append( buff );
					msg.Append( _T("\r\n") );
				}
				value = dict->GetVar( "headRev" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_HEAD_REV, value );
				}
				value = dict->GetVar( "headChange" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_HEAD_CHG, value );
				}
				value = dict->GetVar( "haveRev" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_HAVE, value );
				}
				value = dict->GetVar( "action" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_OPENED_YOU_FOR, value );
				}
				value = dict->GetVar( "change" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_IN_CHG, value );
				}
				value = dict->GetVar( "actionOwner" );
				if ( value ) {
					AppendLine( &msg, P4_FILEOP_OPENED_OTHER, value );
				}
				StrBuf key;
				for (int i=0;;i++) {
					key.Clear();
					key << "otherOpen" << i;
					value = dict->GetVar( key.Text() );
					if ( value == NULL ) {
						break;
					}
					AppendLine( &msg, P4_FILEOP_OPENED_OTHER_ALSO, value );
					key.Clear();
					key << "otherAction" << i;
					value = dict->GetVar( key.Text() );
					if ( value ) {
						AppendLine( &msg, P4_FILEOP_OPENED_OTHER_FOR, value );
					}
				}
				value = dict->GetVar( "ourLock" );
				if ( value ) {
					msg.Append( P4_FILEOP_LOCKED_YOU  );
					msg.Append( _T("\r\n") );
				}
				value = dict->GetVar( "otherLock" );
				if ( value ) {
					msg.Append( P4_FILEOP_LOCKED_OTHER );
					msg.Append( _T("\r\n") );
				}

			}
			SetWindowText( results, (TCHAR *)msg.Text() );
		}
		else {
			SetWindowText( results, (TCHAR *)dialog->Results()->GetText() );
		}

		if ( _tcscmp( op, _T("edit") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_CHECKOUT );
		}
		else if ( _tcscmp( op, _T("add") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_ADD );
		}
		else if ( _tcscmp( op, _T("revert") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_ADD_CHECKOUT );
			ShowWindow( hide, FALSE );
		}
		else if ( _tcscmp( op, _T("lock") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_LOCK );
			ShowWindow( hide, FALSE );
		}
		else if ( _tcscmp( op, _T("unlock") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_UNLOCK );
			ShowWindow( hide, FALSE );
		}
		else if ( _tcscmp( op, _T("submit") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_CONF_CHECKIN );
			ShowWindow( hide, FALSE );
		}
		else if ( _tcscmp( op, _T("fstat") ) == 0 ) {
			SetWindowText( hWnd, P4_FILEOP_DOC_STATUS );
			ShowWindow( hide, FALSE );
		}

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( fileName, RESIZE_WIDTH );
		SetResizeFlags( results, RESIZE_WIDTH | RESIZE_HEIGHT );
		SetResizeFlags( hide, RESIZE_TOP );
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
		FileOpDialog *dialog = ( FileOpDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_CTLCOLORSTATIC:
		// Make the background of edit boxes white instead of the read-only grey.
		if ( ( HWND )lParam == GetDlgItem( hWnd, IDC_FILENAME_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_RESULTS_EDIT ) ) {
			return GetStockObject( WHITE_BRUSH ) != NULL;
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case IDOK: {
			HWND check = GetDlgItem( hWnd, IDC_HIDE_CHECK );
			int hide =  (int)SendMessage( check, BM_GETCHECK, 0, 0 );
			if ( hide ) {
				FileOpDialog *dialog = ( FileOpDialog * )DialogObject( hWnd );
				PerforceOptions *options = dialog->Options();
				options->SetOptionValue( SHOW_CONFIRMATION_OPTION, FALSE );
			}
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
