/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "ConnectionDialog.h"

ConnectionDialog::ConnectionDialog( PluginOperations *ops, PerforceResults *results ) : Dialog( ops, IDD_CONNECTION_DIALOG, DialogWndProc ) {
	this->results = results;
}

INT_PTR CALLBACK ConnectionDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	HWND userName = GetDlgItem( hWnd, IDC_USER_NAME_EDIT );
	HWND clientName = GetDlgItem( hWnd, IDC_CLIENT_NAME_EDIT );
	HWND clientRoot = GetDlgItem( hWnd, IDC_CLIENT_ROOT_EDIT );
	HWND clientHost = GetDlgItem( hWnd, IDC_CLIENT_HOST_EDIT );
	HWND clientHostLabel = GetDlgItem( hWnd, IDC_CLIENT_HOST_LABEL );
	HWND clientAddress = GetDlgItem( hWnd, IDC_CLIENT_ADDRESS_EDIT );
	HWND clientAddressLabel = GetDlgItem( hWnd, IDC_CLIENT_ADDRESS_LABEL );
	HWND serverAddress = GetDlgItem( hWnd, IDC_SERVER_ADDRESS_EDIT );
	HWND serverRoot = GetDlgItem( hWnd, IDC_SERVER_ROOT_EDIT );
	HWND serverDate = GetDlgItem( hWnd, IDC_SERVER_DATE_EDIT );
	HWND serverVersion = GetDlgItem( hWnd, IDC_SERVER_VERSION_EDIT );
	HWND currentDirectory = GetDlgItem( hWnd, IDC_CURRENT_DIRECTORY_EDIT );
	HWND serverLicense = GetDlgItem( hWnd, IDC_SERVER_LICENSE_EDIT );
	HWND okButton = GetDlgItem( hWnd, IDOK );

	switch(uMsg) {

	case WM_INITDIALOG:	{	
		ConnectionDialog *dialog = ( ConnectionDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		PerforceResults *results = dialog->Results();

		if ( results->Count() > 0 ) {
			StrBufDict *dict = (StrBufDict *)results->Get( 0 );	
			if ( dict->GetVar( "userName" ) != NULL ) {
				SetWindowText( userName, (TCHAR *)dict->GetVar( "userName" )->Text() );
			}
			if ( dict->GetVar( "clientName" ) != NULL ) {
				SetWindowText( clientName, (TCHAR *)dict->GetVar( "clientName" )->Text() );
			}
			if ( dict->GetVar( "clientRoot" ) != NULL ) {
				SetWindowText( clientRoot, (TCHAR *)dict->GetVar( "clientRoot" )->Text() );
			}
			if ( dict->GetVar( "clientHost" ) != NULL ) {
				SetWindowText( clientHost, (TCHAR *)dict->GetVar( "clientHost" )->Text() );
			}
			if ( dict->GetVar( "clientAddress" ) != NULL ) {
				SetWindowText( clientAddress, (TCHAR *)dict->GetVar( "clientAddress" )->Text() );
			}
			if ( dict->GetVar( "serverAddress" ) != NULL ) {
				SetWindowText( serverAddress, (TCHAR *)dict->GetVar( "serverAddress" )->Text() );
			}
			if ( dict->GetVar( "serverRoot" ) != NULL ) {
				SetWindowText( serverRoot, (TCHAR *)dict->GetVar( "serverRoot" )->Text() );
			}
			if ( dict->GetVar( "serverDate" ) != NULL ) {
				SetWindowText( serverDate, (TCHAR *)dict->GetVar( "serverDate" )->Text() );
			}
			if ( dict->GetVar( "serverVersion" ) != NULL ) {
				SetWindowText( serverVersion, (TCHAR *)dict->GetVar( "serverVersion" )->Text() );
			}
			if ( dict->GetVar( "serverLicense" ) != NULL ) {
				SetWindowText( serverLicense, (TCHAR *)dict->GetVar( "serverLicense" )->Text() );
			}

			TCHAR dir[MAX_PATH];
			GetCurrentDirectory(sizeof(dir), dir);
			SetDlgItemText( hWnd, IDC_CURRENT_DIRECTORY_EDIT, dir );
		}

		dialog->Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( userName, RESIZE_WIDTH_HALF );
		SetResizeFlags( clientName, RESIZE_WIDTH_HALF );
		SetResizeFlags( clientHost, RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
		SetResizeFlags( clientHostLabel, RESIZE_LEFT_HALF );
		SetResizeFlags( clientAddress, RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
		SetResizeFlags( clientAddressLabel, RESIZE_LEFT_HALF );
		SetResizeFlags( clientRoot, RESIZE_WIDTH );
		SetResizeFlags( currentDirectory, RESIZE_WIDTH );
		SetResizeFlags( serverAddress, RESIZE_WIDTH );
		SetResizeFlags( serverDate, RESIZE_WIDTH );
		SetResizeFlags( serverVersion, RESIZE_WIDTH );
		SetResizeFlags( serverLicense, RESIZE_WIDTH );
		SetResizeFlags( serverRoot, RESIZE_WIDTH );
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
		ConnectionDialog *dialog = ( ConnectionDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd ); 
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam ); 
	}

	case WM_CTLCOLORSTATIC:
		
		// Make the background of edit boxes white instead of the read-only grey.
		if ( ( HWND )lParam == userName ||
			 ( HWND )lParam == clientName ||
			 ( HWND )lParam == clientRoot ||
			 ( HWND )lParam == clientHost ||
			 ( HWND )lParam == clientAddress ||
			 ( HWND )lParam == serverAddress ||
			 ( HWND )lParam == serverRoot ||
			 ( HWND )lParam == serverDate ||
			 ( HWND )lParam == serverVersion ||
			 ( HWND )lParam == currentDirectory ||
			 ( HWND )lParam == serverLicense ) {

			if ( IsWindowEnabled( ( HWND )lParam ) ) {
				return GetStockObject( WHITE_BRUSH ) != NULL;
			}
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
