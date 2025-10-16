/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "FullSubmitDialog.h"
#include "debug.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

FullSubmitDialog::FullSubmitDialog( PluginOperations *ops, PerforceConnection *con, PerforceResults *results, StrPtr *curr, BOOL showOther ) : Dialog( ops, IDD_FULL_SUBMIT_DIALOG, DialogWndProc )
{
    desc = NULL;
	submitFiles = NULL;
	submitJobs = NULL;
	jobStatus = FALSE;

	this->con = con;
	this->results = results;
	this->curr = curr;
	this->showOther = showOther;
}

FullSubmitDialog::~FullSubmitDialog()
{
	if ( desc != NULL ) {
		delete []desc;
	}
	if ( submitFiles != NULL ) {
		delete submitFiles;
	}
	if ( submitJobs != NULL ) {
		delete submitJobs;
	}
}

INT_PTR CALLBACK FullSubmitDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HWND changeDesc = GetDlgItem( hWnd, IDC_CHANGE_DESC_EDIT );
	HWND selectButton = GetDlgItem( hWnd, IDC_SELECT_ALL_CHECK );
	HWND openFile = GetDlgItem( hWnd, IDC_OPEN_FILE_LIST );
	HWND jobsLabel = GetDlgItem( hWnd, IDC_JOBS_LABEL );
	HWND openJob = GetDlgItem( hWnd, IDC_OPEN_JOB_LIST );
	HWND statusLabel = GetDlgItem( hWnd, IDC_STATUS_LABEL );
	HWND statusCombo = GetDlgItem( hWnd, IDC_JOB_STATUS_COMBO );
	HWND keepOut = GetDlgItem( hWnd, IDC_KEEP_OUT_CHECK );
	HWND optionsLabel = GetDlgItem( hWnd, IDC_SUBMIT_OPTIONS_LABEL );
	HWND optionsCombo = GetDlgItem( hWnd, IDC_SUBMIT_OPTIONS_COMBO );
	HWND optionsDefault = GetDlgItem( hWnd, IDC_SUBMIT_OPTIONS_DEFAULT );
	HWND okButton = GetDlgItem( hWnd, IDOK );
	HWND cancelButton = GetDlgItem( hWnd, IDCANCEL );

	switch(uMsg) {

	case WM_INITDIALOG: {
		FullSubmitDialog *dialog = ( FullSubmitDialog * )lParam;
		SaveDialogObject( hWnd, dialog );

		SendMessage( changeDesc, WM_SETTEXT, 0, (LPARAM) P4_FULLSUBMIT_DESC );
		SendMessage( changeDesc, EM_SETSEL, 0, -1 );
		SetFocus( changeDesc );

		int numMainWin = 1;
		HWND lastControl = changeDesc;

		DWORD style = ListView_GetExtendedListViewStyle( openFile );
		style |= LVS_EX_CHECKBOXES;
		ListView_SetExtendedListViewStyle( openFile, style );

		style = ListView_GetExtendedListViewStyle( openJob );
		style |= LVS_EX_CHECKBOXES;
		ListView_SetExtendedListViewStyle( openJob, style );

		// Can't use images with checks as it creates alignment problems
		//SetListViewImageLists( fileList );

		LV_COLUMN col;
		col.mask = LVCF_SUBITEM | LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		col.fmt = LVCFMT_LEFT;
		col.cx = 200;
		col.pszText = P4_FULLSUBMIT_DOC;
		col.iSubItem = 0;
		ListView_InsertColumn( openFile, 0, &col );

		col.cx = 400;
		ListView_InsertColumn( openJob, 0, &col );

		LV_ITEM item;
		item.mask = LVIF_TEXT;
		item.iSubItem = 0;

		StrBuf fileText;
		RECT rect;

		PerforceResults *results = dialog->Results();

		StrPtr *curr = dialog->CurrFile();
		if ( results->Count() > 0 ) {
			UniStrDict *dict = ( UniStrDict * )dialog->Results()->Get( 0 );
			StrBuf key;
			for ( int i=0;;i++ ) {
				key.Clear();
				key << "Files" << i;
				StrPtr *filename = dict->GetVar ( key );
				if ( filename == NULL ) {
					break;
				}
				item.pszText = (TCHAR *)filename->Text();
				item.iItem = i;
				//item.iImage = FileIcon( filename );
				ListView_InsertItem( openFile, &item );
				if ( *filename == *curr ) {
					ListView_SetCheckState( openFile, i, TRUE );
				}
			}

#ifndef LANG_ja
			if ( dict->GetVar( "Jobs0" ) ) {
				dialog->FillJobStatus( statusCombo );
				StrPtr *data = dict->GetVar( "data" );
				TCHAR *start = _tcsstr( (TCHAR *)data->Text(), P4_FULLSUBMIT_JOBS );
				UniStrBuf fullDesc;
				TCHAR jobDesc[80];
				for ( int i=0;;i++ ) {
					key.Clear();
					key << "Jobs" << i;
					StrPtr *jobname = dict->GetVar ( key );
					if ( jobname == NULL ) {
						break;
					}

					// Get the job description
					TCHAR *begin = _tcschr( start, '#' ) + 2;
					for ( start = begin; *start != '\n'; start++ ) {
					}
					_tcsncpy( jobDesc, begin, start - begin );
					jobDesc[start - begin] = '\0';

					fullDesc.Set( jobname );
					fullDesc.Append( _T(" - ") );
					fullDesc.Append( jobDesc );

					item.pszText = fullDesc.Text();
					item.iItem = i;
					ListView_InsertItem( openJob, &item );
				}
				lastControl = openJob;
				numMainWin++;
			}
			else {
#endif  // LANG_ja
				ShowWindow( openJob, SW_HIDE );
				EnableWindow( openJob, FALSE );
				ShowWindow( statusCombo, SW_HIDE );
				EnableWindow( statusCombo, FALSE );
				ShowWindow( jobsLabel, SW_HIDE );
				EnableWindow( jobsLabel, FALSE );
				ShowWindow( statusLabel, SW_HIDE );
				EnableWindow( statusLabel, FALSE );
#ifndef LANG_ja
			}
#endif  // LANG_ja
		}

		// Only if server supports new submit options
		if ( dialog->ops->GetExtSubmit() )
		{
			// Fill submit options ComboBox
			SendMessage( optionsCombo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) P4_FULLSUBMIT_OPTION1 );
			SendMessage( optionsCombo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) P4_FULLSUBMIT_OPTION2 );
			SendMessage( optionsCombo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) P4_FULLSUBMIT_OPTION3 );

			// Set default option
			SendMessage( optionsCombo, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) P4_FULLSUBMIT_OPTION1 );
		}

		if ( dialog->ShowOther() ) {
			GetControlPos( hWnd, lastControl, &rect );
			MoveControlY( hWnd, selectButton, rect.bottom + 20 );
			GetControlPos( hWnd, selectButton, &rect );
			MoveControlY( hWnd, openFile, rect.bottom + 4 );
			lastControl = openFile;
			numMainWin++;
		}
		else {
			ShowWindow( selectButton, SW_HIDE );
			EnableWindow( selectButton, FALSE );
			ShowWindow( openFile, SW_HIDE );
			EnableWindow( openFile, FALSE );
		}

		GetControlPos( hWnd, lastControl, &rect );
		int lastBottom = rect.bottom;
		MoveControlY( hWnd, keepOut, lastBottom + 5 );
		MoveControlY( hWnd, okButton, lastBottom + 40 );
		MoveControlY( hWnd, cancelButton, lastBottom + 40 );

		// Only if server supports new submit options
		if ( dialog->ops->GetExtSubmit() ) {
			MoveControlY( hWnd, optionsLabel, lastBottom + 5 );
			MoveControlY( hWnd, optionsCombo, lastBottom + 5 );
			MoveControlY( hWnd, optionsDefault, lastBottom + 5 );
		}
		else {
			ShowWindow( optionsLabel, SW_HIDE );
			EnableWindow( optionsLabel, FALSE );
			ShowWindow( optionsCombo, SW_HIDE );
			EnableWindow( optionsCombo, FALSE );
			ShowWindow( optionsDefault, SW_HIDE );
			EnableWindow( optionsDefault, FALSE );
		}

		// Set height of dialog
		GetWindowRect( hWnd, &rect );
		MoveWindow( hWnd, rect.left, rect.top, rect.right - rect.left, lastBottom + 110, FALSE );

		ListView_SetColumnWidth( openFile, 0, LVSCW_AUTOSIZE );

		Centre( hWnd );
		SaveInitialSizes( hWnd );

		if ( numMainWin == 3 ) {
			SetResizeFlags( changeDesc, RESIZE_WIDTH | RESIZE_HEIGHT_THIRD );
		}
		else if ( numMainWin == 2 ) {
			SetResizeFlags( changeDesc, RESIZE_WIDTH | RESIZE_HEIGHT_HALF );
		}
		else {
			SetResizeFlags( changeDesc, RESIZE_WIDTH | RESIZE_HEIGHT );
		}

		if ( numMainWin == 3 ) {
			SetResizeFlags( jobsLabel, RESIZE_TOP_THIRD );
			SetResizeFlags( openJob, RESIZE_TOP_THIRD | RESIZE_WIDTH | RESIZE_HEIGHT_THIRD );
			SetResizeFlags( statusLabel, RESIZE_TOP_THIRD );
			SetResizeFlags( statusCombo, RESIZE_TOP_THIRD | RESIZE_WIDTH );
		}
		else {
			SetResizeFlags( jobsLabel, RESIZE_TOP_HALF );
			SetResizeFlags( openJob, RESIZE_TOP_HALF | RESIZE_WIDTH | RESIZE_HEIGHT_HALF );
			SetResizeFlags( statusLabel, RESIZE_TOP_HALF );
			SetResizeFlags( statusCombo, RESIZE_TOP_HALF | RESIZE_WIDTH );
		}

		if ( numMainWin == 3 ) {
			SetResizeFlags( selectButton, RESIZE_TOP_TWOTHIRD );
			SetResizeFlags( openFile, RESIZE_TOP_TWOTHIRD | RESIZE_WIDTH | RESIZE_HEIGHT_THIRD );
			SetResizeFlags( keepOut, RESIZE_TOP );
			SetResizeFlags( optionsLabel, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( optionsCombo, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( optionsDefault, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( okButton, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( cancelButton, RESIZE_TOP | RESIZE_LEFT_HALF );
		}
		else {
			SetResizeFlags( selectButton, RESIZE_TOP_HALF );
			SetResizeFlags( openFile, RESIZE_TOP_HALF | RESIZE_WIDTH | RESIZE_HEIGHT_HALF );
			SetResizeFlags( keepOut, RESIZE_TOP );
			SetResizeFlags( optionsLabel, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( optionsCombo, RESIZE_TOP | RESIZE_LEFT_HALF | RESIZE_WIDTH_HALF );
			SetResizeFlags( optionsDefault, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( okButton, RESIZE_TOP | RESIZE_LEFT_HALF );
			SetResizeFlags( cancelButton, RESIZE_TOP | RESIZE_LEFT_HALF );
		}

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
		FullSubmitDialog *dialog = ( FullSubmitDialog * )DialogObject( hWnd );
		dialog->PaintSizeGrip( hWnd );
		break;
	}

	case WM_NCHITTEST: {
		return HitTestSizeGrip( hWnd, lParam );
	}

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case IDOK: {
			FullSubmitDialog *dialog = ( FullSubmitDialog * )DialogObject( hWnd );
			// If LRESULT is over MAXINT, this will truncate the value.
			int len = (int)SendMessage( changeDesc, WM_GETTEXTLENGTH, 0, 0 ) + 1;
			TCHAR *text = new TCHAR[ len ];
			GetWindowText( changeDesc, text, len );

			bool valid = true;

			// Check user a has enter a description
			if ( _tcslen( text ) == 0 || _tcscmp ( text, P4_FULLSUBMIT_DESC ) == 0 ) {
				MessageBox( hWnd, _T("Please enter a description."), P4_FULLSUBMIT_CHECKIN, MB_ICONEXCLAMATION | MB_OK );
				SetFocus( changeDesc );
				valid = false;
			}

			// Check user has selected at least one file
			if ( valid ) {
				HWND fileList = GetDlgItem( hWnd, IDC_OPEN_FILE_LIST );
				int count = ListView_GetItemCount( fileList );
				bool gotOne = false;
				for ( int i=0; i<count; i++ ) {
					if ( ListView_GetCheckState( fileList, i ) == TRUE ) {
						gotOne = true;
						break;
					}
				}
				if ( !gotOne ) {
					MessageBox( hWnd, _T("Please select at least one file."), P4_FULLSUBMIT_CHECKIN, MB_ICONEXCLAMATION | MB_OK );
					SetFocus( fileList );
					valid = false;
				}
			}

			if ( valid ) {
				// Remove carriage returns and add tabs
				for ( int i=0; text[i] != '\0'; i++ ) {
					if ( text[i] == '\r' && text[i + 1] == '\n' ) {
						text[i] = '\n';
						text[i + 1] = '\t';
					}
				}
				dialog->SetDescription( text );

				PerforceResults *results = dialog->Results();
				UniStrDict *dict = ( UniStrDict * )dialog->Results()->Get( 0 );
				StrBuf key;
				StrPtr *value;

				// Get checked files
				UniStrBuf *files = new UniStrBuf();
				int count = ListView_GetItemCount( openFile );
				for ( int i=0; i<count; i++ ) {
					if ( ListView_GetCheckState( openFile, i ) == TRUE ) {
						key.Clear();
						key << "Files" << i;
						value = dict->GetVar ( key );
						files->Append( _T("\n\t") );
						files->Append( value );
					}
				}
				dialog->SetSubmitFiles( files );

				TCHAR selectedStatus[128] = {'\0'};
				// Get the job status
				// If LRESULT is over MAXINT, this will truncate the value.
				int idx = (int)SendMessage( statusCombo, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0 );
				// Ignore first "(default)" entry
				if ( idx > 0 ) {
					dialog->SetJobStatus();
					SendMessage( statusCombo, CB_GETLBTEXT, (WPARAM) idx, (LPARAM) selectedStatus );
				}
				// Get checked jobs
				UniStrBuf *jobs = new UniStrBuf();
				count = ListView_GetItemCount( openJob );
				for ( int i=0; i<count; i++ ) {
					if ( ListView_GetCheckState( openJob, i ) == TRUE ) {
						key.Clear();
						key << "Jobs" << i;
						value = dict->GetVar ( key );
						jobs->Append( _T("\n\t") );
						jobs->Append( value );
						if ( _tcslen( selectedStatus ) > 0 ) {
							jobs->Append( _T(" ") );
							jobs->Append( selectedStatus );
						}
					}
				}
				dialog->SetSubmitJobs( jobs );

				HWND check = GetDlgItem( hWnd, IDC_KEEP_OUT_CHECK );
				dialog->SetKeepOut( (int)SendMessage( check, BM_GETCHECK, 0, 0 ) );

				// Only if server supports new submit options
				if ( dialog->ops->GetExtSubmit() )
				{
					// Get the selected submit option in combo box
					TCHAR optionSelected[80] = {'\0'};
					// If LRESULT is over MAXINT, this will truncate the value.
					idx = (int)SendMessage( optionsCombo, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0 );
					SendMessage( optionsCombo, CB_GETLBTEXT, (WPARAM) idx, (LPARAM) optionSelected );

					if ( _tcsstr( optionSelected, P4_FULLSUBMIT_OPTION1 ) != NULL ) {
						if ( dialog->GetKeepOut() ) {
							dialog->SetSubmitOption( _T("submitunchanged+reopen") );
						}
						else {
							dialog->SetSubmitOption( _T("submitunchanged") );
						}
					}
					else
					{
						if ( _tcsstr( optionSelected, P4_FULLSUBMIT_OPTION2 ) != NULL ) {
							if ( dialog->GetKeepOut() ) {
								dialog->SetSubmitOption( _T("leaveunchanged+reopen") );
							}
							else {
								dialog->SetSubmitOption( _T("leaveunchanged") );
							}
						}
						else
						{
							if ( _tcsstr( optionSelected, P4_FULLSUBMIT_OPTION3 ) != NULL ) {
								if ( dialog->GetKeepOut() ) {
									dialog->SetSubmitOption( _T("revertunchanged+reopen") );
								}
								else {
									dialog->SetSubmitOption( _T("revertunchanged") );
								}
							}
						}
					}
				}
				FinishDialog( hWnd, TRUE );
			}
			else {
				delete []text;
			}
			return true;
		}
		case IDCANCEL:
			FinishDialog( hWnd, FALSE );
			return true;

		case IDC_SELECT_ALL_CHECK:
			FullSubmitDialog *dialog = ( FullSubmitDialog * )DialogObject( hWnd );

			// Grab select all checkbox setting
			HWND selectAll = GetDlgItem( hWnd, IDC_SELECT_ALL_CHECK );
			dialog->SetSelectAll( (int)SendMessage( selectAll, BM_GETCHECK, 0, 0 ) );

			// [un]Select all opened files
			HWND fileList = GetDlgItem( hWnd, IDC_OPEN_FILE_LIST );
			int count = ListView_GetItemCount( fileList );

			for ( int i=0; i<count; i++ ) {
				ListView_SetCheckState( fileList, i, dialog->GetSelectAll() );
			}
			return true;
		}
	}

	return false;
}

void FullSubmitDialog::FillJobStatus( HWND combo )
{
	PerforceResults results;
	PerforceCommand cmd( _T("jobspec"), _T("-o") );
	con->Exec( &cmd, &results );

	if ( results.Count() > 0 ) {
		UniStrDict *dict = ( UniStrDict * )results.Get( 0 );
		StrBuf key;
		TCHAR statusName[80] = { '\0' };
		for ( int i=0;;i++ ) {
			key.Clear();
			key << "Fields" << i;
			StrPtr *fieldname = dict->GetVar ( key );
			if ( fieldname == NULL ) {
				break;
			}
			TCHAR *text = (TCHAR *)fieldname->Text();
			// Look for status field
			if ( _tcsncmp( text, _T("102"), 3 ) == 0 ) {
				TCHAR *end = _tcschr( text + 4, ' ');
				_tcsncpy( statusName, text + 4, end - ( text + 4 ) );
				statusName[end - ( text + 4 ) ] = '\0';
				break;
			}
		}
		if ( _tcslen( statusName ) > 0 ) {
			for ( int i=0;;i++ ) {
				key.Clear();
				key << "Values" << i;
				StrPtr *valuename = dict->GetVar ( key );
				if ( valuename == NULL ) {
					break;
				}
				TCHAR *text = (TCHAR *)valuename->Text();
				// Look for status field
				if ( _tcsncmp( text, statusName, _tcslen( statusName ) ) == 0 ) {
					SendMessage( combo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) P4_FULLSUBMIT_DFLT );
					TCHAR *start = _tcschr( text, ' ') + 1;
					TCHAR *end;
					TCHAR buff[80];
					while ( end = _tcschr( start, '/' ), end != NULL ) {
						_tcsncpy( buff, start, end - start );
						buff[end - start] = '\0';
						start = end + 1;
						SendMessage( combo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) buff );
					}
					SendMessage( combo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) start );
					break;
				}
			}
		}
	}
}
