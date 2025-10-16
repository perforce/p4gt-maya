/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "StatusDialog.h"

StatusDialog::StatusDialog( PluginOperations *ops, PerforceFileStat *stat ) : Dialog( ops, IDD_STATUS_DIALOG, DialogWndProc ) 
{
	this->stat = stat;
}

INT_PTR CALLBACK StatusDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	HWND depotPath = GetDlgItem( hWnd, IDC_DEPOT_PATH_EDIT );
	HWND clientPath = GetDlgItem( hWnd, IDC_CLIENT_PATH_EDIT );
	HWND fileType = GetDlgItem( hWnd, IDC_FILE_TYPE_EDIT );
	HWND headRevision = GetDlgItem( hWnd, IDC_HEAD_REVISION_EDIT );
	HWND haveRevision = GetDlgItem( hWnd, IDC_HAVE_REVISION_EDIT );
	HWND haveRevisionLabel = GetDlgItem( hWnd, IDC_HAVE_REVISION_LABEL );
	HWND headAction = GetDlgItem( hWnd, IDC_HEAD_ACTION_EDIT );
	HWND headChange = GetDlgItem( hWnd, IDC_HEAD_CHANGE_EDIT );
	HWND headChangeLabel = GetDlgItem( hWnd, IDC_HEAD_CHANGE_LABEL );
	HWND headTime = GetDlgItem( hWnd, IDC_HEAD_TIME_EDIT );
	HWND openedBy = GetDlgItem( hWnd, IDC_OPENED_BY_EDIT );
	HWND lockedBy = GetDlgItem( hWnd, IDC_LOCKED_BY_EDIT );
	HWND lockedByLabel = GetDlgItem( hWnd, IDC_LOCKED_BY_LABEL );
	HWND action = GetDlgItem( hWnd, IDC_ACTION_EDIT );
	HWND change = GetDlgItem( hWnd, IDC_CHANGE_EDIT );
	HWND changeLabel = GetDlgItem( hWnd, IDC_CHANGE_LABEL );
	HWND okButton = GetDlgItem( hWnd, IDOK );

	switch(uMsg) {

	case WM_INITDIALOG:	{	
		StatusDialog *dialog = ( StatusDialog * )lParam;
		SaveDialogObject( hWnd, dialog );
		PerforceFileStat *stat = dialog->FileStatus();

		SetWindowText( depotPath, (TCHAR *)stat->DepotPath()->Text() );
		SetWindowText( clientPath, (TCHAR *)stat->ClientPath()->Text() );

		// If file has already been submitted to depot
		if  ( stat->HeadRevision() != NULL ) {
			SetWindowText( fileType, (TCHAR *)stat->FileType()->Text() );
			SetWindowText( headRevision, (TCHAR *)stat->HeadRevision()->Text() );
			SetWindowText( haveRevision, (TCHAR *)stat->HaveRevision()->Text() );
			SetWindowText( headAction, (TCHAR *)stat->HeadAction()->Text() );
			SetWindowText( headChange, (TCHAR *)stat->HeadChange()->Text() );
			SetWindowText( headTime, (TCHAR *)stat->ModifiedTime()->Text() );
		}
		else {
			EnableWindow( fileType, FALSE );	
			EnableWindow( headRevision, FALSE );	
			EnableWindow( haveRevision, FALSE );	
			EnableWindow( headAction, FALSE );	
			EnableWindow( headChange, FALSE );	
			EnableWindow( headTime, FALSE );	
		}

		// If file is checked out by this user
		if ( stat->Action() != NULL ) {
			SetWindowText( action, (TCHAR *)stat->Action()->Text() );
			SetWindowText( change, (TCHAR *)stat->Change()->Text() );
		}
		else {
			EnableWindow( action, FALSE );
			EnableWindow( change, FALSE );
		}
		// If file is opened by other users 
		if ( stat->OtherOpened() != NULL ) {
			SetWindowText( openedBy, (TCHAR *)stat->OtherOpened()->Text() );
		}
		else {
			EnableWindow( openedBy, FALSE );
		}

		// If file is locked
		if ( stat->LockedBy() != NULL ) {
			SetWindowText( lockedBy, (TCHAR *)stat->LockedBy()->Text() );
		}
		else {
			EnableWindow( lockedBy , FALSE );
		}

		Centre( hWnd );
		SaveInitialSizes( hWnd );
		SetResizeFlags( depotPath, RESIZE_WIDTH );
		SetResizeFlags( clientPath, RESIZE_WIDTH );
		SetResizeFlags( fileType, RESIZE_WIDTH_HALF );
		SetResizeFlags( headRevision, RESIZE_WIDTH_HALF );
		SetResizeFlags( headAction, RESIZE_WIDTH_HALF );
		SetResizeFlags( action, RESIZE_WIDTH_HALF );
		SetResizeFlags( haveRevision, RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
		SetResizeFlags( haveRevisionLabel, RESIZE_LEFT_HALF );
		SetResizeFlags( headChange, RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
		SetResizeFlags( headChangeLabel, RESIZE_LEFT_HALF );
		SetResizeFlags( change, RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
		SetResizeFlags( changeLabel, RESIZE_LEFT_HALF );
		SetResizeFlags( headTime, RESIZE_WIDTH );
		SetResizeFlags( openedBy, RESIZE_WIDTH | RESIZE_HEIGHT );
		SetResizeFlags( lockedBy, RESIZE_WIDTH | RESIZE_TOP );
		SetResizeFlags( lockedByLabel, RESIZE_TOP );
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
		StatusDialog *dialog = ( StatusDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd ); 
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam ); 
	}

	case WM_CTLCOLORSTATIC:
		
		// Make the background of edit boxes white instead of the read-only grey.
		if ( ( HWND )lParam == GetDlgItem( hWnd, IDC_DEPOT_PATH_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_CLIENT_PATH_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_FILE_TYPE_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_HEAD_REVISION_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_HAVE_REVISION_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_HEAD_ACTION_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_HEAD_CHANGE_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_HEAD_TIME_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_OPENED_BY_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_LOCKED_BY_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_ACTION_EDIT ) ||
			 ( HWND )lParam == GetDlgItem( hWnd, IDC_CHANGE_EDIT ) ) {

			if (IsWindowEnabled( ( HWND )lParam ) ) {
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
