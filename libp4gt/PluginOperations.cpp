/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PluginOperations.h"
#include "FileOpDialog.h"
#include "FullSubmitDialog.h"
#include "OpenFilesDialog.h"
#include "TabbedOptionsDialog.h"
#include "AboutDialog.h"
#include "OpenDepotDialog.h"
#include "HistoryDialog.h"
#include "StatusDialog.h"
#include "OldVersionDialog.h"
#include "ExternalRefDialog.h"
#include "ResultsDialog.h"
#include "ResolveDialog.h"
#include "CheckInAddDialog.h"
#include "PluginHost.h"
#include "debug.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

#include <commctrl.h>
#include <string>

PluginOperations::PluginOperations( PluginHost *host )
{
	this->host = host;
	login = false;
	gotInfo = false;
	extendedSubmit = false;
	options = new PerforceOptions( host->GetName() );
	online = (BOOL)options->GetOptionValue( SERVER_ONLINE );

	// Load windows common controls.
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof( INITCOMMONCONTROLSEX );
	icex.dwICC  = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_USEREX_CLASSES;
	InitCommonControlsEx( &icex );
}

PluginOperations::~PluginOperations()
{
	delete options;
}

void PluginOperations::ToggleOnline()
{
	online = online ? 0: 1;
	options->SetOptionValue( SERVER_ONLINE, online );
}

MenuState *PluginOperations::GetMenuState()
{
	if ( !online || host->GetNumDocs() == 0 ) {
		return new MenuState( online, NULL, false );
	}
	else {
		UniStrBuf path;
		bool fileExists = false;
//		bool fileExists = host->GetCurrDocPath( path );
		host->GetCurrDocPath( path );
		if ( path.Text() == NULL ) {
			return new MenuState( online, NULL, false );
		}

		// Use our own results as we do not
		// want the menu stat to appear in results of
		// last command
		PerforceResults statResults;
		fileExists = Stat( path.Text(), &stat, &statResults );

		return new MenuState( online, &stat, fileExists );
	}
}

void PluginOperations::FileOpenEvent()
{
	UniStrBuf filename;
	if (!online || !host->GetCurrDocPath( filename ) ) {
		return;
	}

	// Use our own results as we do not
	// want the internal stat to appear in results of
	// last command
	PerforceResults statResults;

	// Is file under Perforce
	if ( Stat( filename.Text(), &stat, &statResults ) ) {

		// If file is checked in
		if (!stat.IsOpen()) {
			// Test for case when file is not (officially) on client
			// i.e. haveRev will be NULL
			StrBuf haveRev;
			if ( stat.HaveRevision() == NULL ) {
				haveRev << "0";
			}
			else {
				haveRev << stat.HaveRevision();
			}

			// If file is not the latest version
			if (!stat.IsLatestVersion()) {
				OldVersionDialog dialog( this, GetFilename( filename.Text() ), &haveRev, stat.HeadRevision() );
				if ( dialog.Show( host->GetInstance(), host->GetWin()) ) {
					if (dialog.ExitCode() == IDSYNC) {
						Sync( filename.Text() );
						host->ReloadCurrDoc();
					}
					else {
						Edit();
					}
				}
			}
			else {
				bool doEdit = false;
				bool doPrompt = false;
				if ( !host->IsReloading() ) {
					switch ( options->GetOptionValue( CHECK_OUT_ON_OPEN_OPTION ) ) {
					case PROMPT_OPTION:
						doPrompt = true;
						break;
					case ALWAYS_OPTION:
						doEdit = true;
						break;
					}
				}
				// Reload after a revert
				else {
					switch ( options->GetOptionValue( CHECK_OUT_ON_REOPEN_OPTION ) ) {
					case PROMPT_OPTION:
						doPrompt = true;
						break;
					case ALWAYS_OPTION:
						doEdit = true;
						break;
					}
				}
				if ( doPrompt ) {
					UniStrBuf buff;
					buff.Set( P4_OPERATIONS_DOC );
					buff.Append( GetFilename( filename.Text() ) );
					buff.Append( P4_OPERATIONS_IN_PERFORCE  );
					buff.Append( P4_OPERATIONS_OK_CHECKOUT );
					if ( MessageBox( host->GetWin(), buff.Text(), P4_OPERATIONS_OPENING_DOC, MB_ICONQUESTION | MB_OKCANCEL ) == IDOK ) {
						doEdit = true;
					}
				}
				if ( doEdit ) {
					Edit();
				}
			}
		}
	}
}

void PluginOperations::FileCloseEvent()
{
	UniStrBuf filename;
	if (!online || !host->GetCurrDocPath( filename ) ) {
		return;
	}

	// Use our own results as we do not
	// want the internal stat to appear in results of
	// last command
	PerforceResults statResults;
	bool fileExists = Stat( filename.Text(), &stat, &statResults );
	if ( fileExists && stat.IsOpen() ) {
		bool doPrompt = false;
		bool doSubmit = false;
		switch ( options->GetOptionValue( CHECK_IN_ON_CLOSE_OPTION ) ) {
		case PROMPT_OPTION:
			doPrompt = true;
			break;
		case ALWAYS_OPTION:
			doSubmit = true;
			break;
		}
		if ( doPrompt ) {
			if ( MessageBox( host->GetWin(), P4_OPERATIONS_DOC_CHECKED_OUT, P4_OPERATIONS_CLOSING_DOC, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
				doSubmit = true;
			}
		}
		if ( doSubmit ) {
			Submit();
		}
		else {
			doPrompt = false;
			bool doRevert = false;
			if ( options->GetOptionValue( REVERT_ON_CLOSE_OPTION ) ) {
				if ( MessageBox( host->GetWin(), P4_OPERATIONS_CLOSE_REVERT_MSG, P4_OPERATIONS_CLOSING_DOC, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
					doRevert = true;
				}
			}
			if ( doRevert ) {
				Revert();
			}
		}
	}
}

void PluginOperations::SetFileDir()
{
	oldDir[0] = '\0';

	if ( currFile.Length() == 0 ) {
		return;
	}

	// Change current directory to where the file is so that P4CONFIG works
	TCHAR *filename = currFile.Text();
	TCHAR *end = filename + _tcslen( filename ) - 1;
	for ( ; *end != '\\' && *end != '/' && end != filename; end-- ) {
		;
	}
	if ( end != filename ) {
		TCHAR dir[MAX_PATH];
		_tcsncpy( dir, filename, end - filename );
		dir[end - filename] = '\0';
		GetCurrentDirectory( MAX_PATH, oldDir );
		SetCurrentDirectory( dir );
	}
}

void PluginOperations::RestoreOldDir()
{
	if ( _tcslen( oldDir ) > 0 ) {
		SetCurrentDirectory( oldDir );
	}
}

bool PluginOperations::Stat( TCHAR *filename, PerforceFileStat *stat, PerforceResults *results )
{
	// Store the current file
	currFile.Clear();
	currFile.Set( filename );

	if ( _tcsicmp(filename,_T(""))==0  ) {
		return false;
	}

	if ( results == NULL ) {
		results = &this->results;
	}
	try {
		CONNECTION( this );
		PerforceCommand cmd( _T("fstat"), filename );
		con.Exec( &cmd, results );
		if ( results->Count() == 1 ) {
			UniStrDict* dict = (UniStrDict *)results->Get( 0 );
			if ( dict->GetVar( "depotFile" ) != NULL ) {
				stat->Set( dict );
				return true;
			}
		}
	}
	catch (exception e) {
		DisplayError( e );
	}

	return false;
}


void PluginOperations::DisplayError( exception &err )
{
	UniStrBuf buff;
	Translate::ToWinChar( err.what(), &buff );
	MessageBox( host->GetWin(), buff.Text(), P4_OPERATIONS_SERVER_ERROR, MB_ICONERROR | MB_OK );
	// If server connect error then go to offline operation
	if ( _tcsstr( buff.Text(), P4_OPERATIONS_CHECK_P4PORT ) != NULL ) {
		ToggleOnline();
	}
}

void PluginOperations::ShowOpenFiles()
{
	try {
		CONNECTION( this );
		PerforceCommand cmd( _T("opened"), _T("-c"), _T("default") );
		con.Exec( &cmd, &results );
		OpenFilesDialog dialog( this, &results );
		dialog.Show( host->GetInstance(), host->GetWin() );
	}
	catch (exception e) {
		DisplayError( e );
	}
}

void PluginOperations::ShowOptions()
{
	TabbedOptionsDialog dialog( this, options, host->HasRefs() );
	dialog.Show( host->GetInstance(), host->GetWin() );
}

void PluginOperations::ShowOpenDepot( StrBufArray *filetypes )
{
	try {
		OpenDepotDialog dialog( this, options, filetypes );
		if ( dialog.Show( host->GetInstance(), host->GetWin() ) == TRUE ) {
			StrPtr *clientFile = dialog.SelectedFile();
			if ( clientFile != NULL ) {
				// If host cannot display multiple documents then we need to
				// lose the current document
				if ( !host->HasMultiDocs() && host->IsCurrDocDirty() ) {
					if ( MessageBox( host->GetWin(), P4_OPERATIONS_SAVE_LATEST_CHECKIN, P4_OPERATIONS_OPEN, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
						host->SaveCurrDoc();
					}
				}
				host->LoadDoc( clientFile );
			}
		}
	}
	catch ( exception e ) {
		DisplayError( e );
	}
}

void PluginOperations::ShowAbout()
{
	AboutDialog dialog( this, host->GetDesc(), host->GetVersion(), host->GetHostVersion() );
	dialog.Show( host->GetInstance(), host->GetWin() );
}

void PluginOperations::ShowHelp()
{
	HKEY helpKey;

    std::wstring regKeyName = L"Software\\Perforce\\";
	regKeyName += std::wstring(host->GetName()); // P4GT-3dsmax, P4GT-Maya, or P4GT-Photoshop.
	

	bool foundURL = false;

	/* 
	 * The installer saves the URL to the help file in
	 * a product-specific registry key.
     */

    if (RegOpenKeyExW( HKEY_LOCAL_MACHINE, regKeyName.c_str(), 0, KEY_QUERY_VALUE, &helpKey) == ERROR_SUCCESS ) {
		DWORD type;
		WCHAR path[MAX_PATH];
		DWORD size = MAX_PATH;
		long ret = RegQueryValueExW( helpKey, L"HelpFileURL", NULL, &type, ( LPBYTE )path, &size );
		RegCloseKey( helpKey );
		if ( ret == ERROR_SUCCESS ) {
			foundURL = true;
			ShellExecute(0, 0, path, 0, 0 , SW_SHOW );
		}
	}
	
	if (!foundURL) {
		std::wstring msg;
		msg.assign( P4_OPERATIONS_HELP_REGISTRY_NOT_FOUND );
		msg.append( L"HKEY_LOCAL_MACHINE\\");
		msg.append( regKeyName );
		MessageBoxW( host->GetWin(), msg.c_str(), P4_OPERATIONS_ERROR, MB_ICONERROR | MB_OK );
	}
}

void PluginOperations::ShowInfo()
{
	try {
		CONNECTION( this );
		PerforceCommand cmd( _T("info") );
		con.Exec( &cmd, &results );
		ResultsDialog dialog( this, _T("info"), &results, &con.GetCwd() );
		dialog.Show( host->GetInstance(), host->GetWin() );
	}
	catch ( exception e ) {
		DisplayError( e );
	}
}

void PluginOperations::ShowStatus()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		// We need to do another stat after the menu stat to
		// so we can store the results.
		Stat( filename.Text(), &stat );
		FileOpDialog dialog( this, _T("fstat"), filename.Text(), &results, options );
		//StatusDialog dialog( &stat );
		dialog.Show( host->GetInstance(), host->GetWin() );
	}
}

void PluginOperations::NeedSaveDialog()
{
	MessageBox( host->GetWin(), P4_OPERATIONS_SAVE_DOC_ADD, P4_OPERATIONS_ADD_ERROR, MB_ICONEXCLAMATION | MB_OK);
}

void PluginOperations::ShowHistory()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		try {
			CONNECTION( this );
			PerforceCommand cmd( _T("filelog"), _T("-l") , filename.Text() );
			con.Exec( &cmd, &results );
			HistoryDialog dialog( this, &results );
			BOOL ret = dialog.Show( host->GetInstance(), host->GetWin() );
			if ( ret && dialog.ExitCode() == ID_VIEW ) {
				// If host cannot display multiple documents then we need to
				// lose the current document
				if ( !host->HasMultiDocs() && host->IsCurrDocDirty() ) {
					if ( MessageBox( host->GetWin(), P4_OPERATIONS_SAVE_LATEST_PREV, P4_OPERATIONS_VIEW_PREV, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
						host->SaveCurrDoc();
					}
				}
				host->LoadDoc( dialog.TempFile() );
			}
			else if ( ret && dialog.ExitCode() == ID_SYNC ) {
				if ( host->IsCurrDocDirty() ) {
					if ( MessageBox( host->GetWin(), P4_OPERATIONS_SAVE_LATEST_PREV, P4_OPERATIONS_VIEW_PREV, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
						host->SaveCurrDoc();
					}
				}
				host->ReloadCurrDoc();
			}
		}
		catch ( exception e ) {
			DisplayError( e );
		}
	}
}

void PluginOperations::SyncReferences( StrBufArray *files )
{
	int syncOption = options->GetOptionValue( SYNC_FILE_REF_OPTION );
	if ( syncOption == NEVER_OPTION ) {
		return;
	}

	try {
		// Store the first file in order to check for P4CONFIG
		TCHAR *filename = (TCHAR *)files->Get( 0 );
		currFile.Clear();
		currFile.Set( filename );

		CONNECTION( this );
		PerforceCommand cmd( _T("fstat") );
		for ( int i=0; i<files->Count(); i++ ) {
			cmd.Append( (TCHAR *)files->Get( i ) );
		}
		PerforceResults statResults;
		con.Exec( &cmd, &statResults );

		// Check if we have any old files
		bool gotOld = false;
		PerforceFileStat refStat;
		for ( int i=0; i<statResults.Count(); i++ ) {
			UniStrDict* dict = (UniStrDict *)statResults.Get( i );
			if ( dict->GetVar( "depotFile" ) != NULL ) {
				refStat.Set( dict );
				if ( !refStat.IsLatestVersion() ) {
					gotOld = true;
				}
			}
		}
		if ( gotOld ) {
			bool doSync = true;
			ExternalRefDialog dialog( this, &statResults );
			if ( syncOption == PROMPT_OPTION ) {
				if ( dialog.Show( host->GetInstance(), host->GetWin() ) == TRUE ) {
					// Files storage is owned by dialog
					files = dialog.GetSyncFiles();
				}
				else {
					doSync = false;
				}
			}
			if ( doSync ) {
				PerforceCommand cmd( _T("sync") );
				for ( int i=0; i<files->Count(); i++ ) {
					cmd.Append( files->Get( i ) );
				}
				con.Exec( &cmd, &results );
			}
		}
	}
	catch (exception e) {
		DisplayError( e );
	}
}

void PluginOperations::Sync( TCHAR *filename, bool show )
{
	DoFileOp( _T("sync"), filename, show );
}

void PluginOperations::Add()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		bool showDialog = options->GetOptionValue( SHOW_CONFIRMATION_OPTION ) ? true : false;
		if ( DoFileOp( _T("add"), filename.Text(), showDialog ) ) {
			switch ( options->GetOptionValue( CHECK_IN_ON_ADD_OPTION ) ) {
			case ALWAYS_OPTION:
				Submit();
				break;
			case PROMPT_OPTION:
				CheckInAddDialog dialog( this, options );
				if ( dialog.Show( host->GetInstance(), host->GetWin() ) == TRUE ) {
					Submit();
				}
				break;
			}
		}
	}
	else {
		NeedSaveDialog();
	}
}

void PluginOperations::Edit()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		bool doEdit = true;
		bool doLock = true;

		// Check if file is exclusively locked by someone else.
		if ( stat.IsOtherExclLocked() ) {
			doEdit = false;
			doLock = false;
			UniStrBuf msg;
			msg.Set( P4_OPERATIONS_CHECKED_IN );
			msg.Append( P4_OPERATIONS_EXCL_LOCKED );
			msg.Append( stat.OtherOpened() );
			msg.Append( _T(".\r\n\r\n") );
			msg.Append( P4_OPERATIONS_UNABLE_CHECKOUT );

			MessageBox( host->GetWin(), msg.Text(), P4_OPERATIONS_CHECKOUT, MB_ICONERROR | MB_OK );
		}
		else if ( stat.IsOtherLocked() && options->GetOptionValue( LOCK_ON_CHECK_OUT_OPTION ) ) {
				UniStrBuf msg;
				msg.Set( P4_OPERATIONS_CHECKED_IN );
				msg.Append( P4_OPERATIONS_LOCKED_BY );
				msg.Append( stat.GetOtherLockedUserClient() );
				msg.Append( _T(".\r\n\r\n") );
				msg.Append( P4_OPERATIONS_UNABLE_AUTO_LOCK );
				msg.Append( P4_OPERATIONS_CHECKOUT_NO_LOCK);

				if ( MessageBox( host->GetWin(), msg.Text(), P4_OPERATIONS_CHECKOUT, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
					doEdit = true;
					doLock = false;
				}
				else {
					doEdit = false;
					doLock = false;
				}
		}
		if ( doEdit ) {
			bool showDialog = options->GetOptionValue( SHOW_CONFIRMATION_OPTION ) ? true : false;
			DoFileOp( _T("edit"), filename.Text(), showDialog );
			if ( doLock ) {
				// Don't show lock operation in results of last command
				PerforceResults lockResults;
				switch ( options->GetOptionValue( LOCK_ON_CHECK_OUT_OPTION ) ) {
				case PROMPT_OPTION:
					if ( MessageBox( host->GetWin(), P4_OPERATIONS_LOCK, P4_OPERATIONS_CHECKOUT, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
						DoFileOp( _T("lock"), filename.Text(), false, &lockResults );
					}
					break;
				case ALWAYS_OPTION:
					DoFileOp( _T("lock"), filename.Text(), false, &lockResults );
					break;
				}
			}
		}
	}
}

void PluginOperations::Revert()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		bool doRevert = false;
		if ( stat.IsOpenForAdd() ) {
			doRevert = true;
		}
		else if (MessageBox( host->GetWin(), P4_OPERATIONS_UNDO_CHECKOUT, P4_OPERATIONS_REVERT_PREV, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
			doRevert = true;
		}
		if ( doRevert ) {
			DoFileOp( _T("revert"), filename.Text() );
		}
	}
}

void PluginOperations::ToggleLock()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		if ( stat.IsOurLocked() ) {
			DoFileOp( _T("unlock"), filename.Text() );
		}
		else {
			DoFileOp( _T("lock"), filename.Text() );
		}
	}
}

void PluginOperations::Submit()
{
	UniStrBuf filename;
	if ( host->GetCurrDocPath( filename ) ) {
		// Need to make sure we stat the file.
		// This is because it would not happen if it is a new file
		// that is being automatically submitted after an add
		PerforceResults statResults;
		Stat( filename.Text(), &stat, &statResults );

		// Check if file needs to be saved before submit
		if ( host->IsCurrDocDirty() ) {
			if ( MessageBox( host->GetWin(), P4_OPERATIONS_SAVE_LATEST_CHECKIN, P4_OPERATIONS_CHECKIN, MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
				host->SaveCurrDoc();
			}
		}

		bool doSubmit = true;

		CONNECTION( this );

		// Check if need to resolve.
		if ( !stat.IsLatestVersion() ) {
			ResolveDialog dialog( this );
			if ( dialog.Show( host->GetInstance(), host->GetWin() ) == TRUE ) {
				int option = dialog.GetOption();
				if ( option == IDC_UNDO_RADIO ) {
					Revert();
					doSubmit = false;
				}
				else if ( option == IDC_CHECK_IN_RADIO ) {
					try {
						Sync( (TCHAR *)stat.DepotPath()->Text(), false );
						PerforceCommand cmd( _T("resolve"), _T("-ay"), (TCHAR *)stat.DepotPath()->Text() );
						con.Exec( &cmd, &results );
					}
					catch ( exception e ) {
						DisplayError( e );
						doSubmit = false;
					}
				}
				else {
					doSubmit = false;
				}
			}
			else {
				doSubmit = false;
			}
		}

		if ( !doSubmit ) {
			return;
		}

		try {
			PerforceCommand cmd( _T("change"), _T("-o") );
			con.Exec( &cmd, &results );
			FullSubmitDialog dialog( this, &con, &results, stat.DepotPath(), options->GetOptionValue( OTHER_DOCUMENTS_OPTION ) );
			if ( dialog.Show( host->GetInstance(), host->GetWin() ) == TRUE ) {
				UniStrBuf desc;
				dialog.GetDescription( desc );
				StrPtr *files = dialog.GetSubmitFiles();
				StrPtr *jobs = dialog.GetSubmitJobs();

				PerforceCommand cmd( _T("submit") );

				// Only if server supports extended submit options
				if ( GetExtSubmit() ) {
					TCHAR *option = (TCHAR *)dialog.GetSubmitOption();
					cmd.Append( _T("-f") );
					cmd.Append( (TCHAR *)option );
				}
				else
				{
					if ( dialog.GetKeepOut() ) {
						cmd.Append( _T("-r") );
					}
				}
				if ( dialog.GetJobStatus() ) {
					cmd.Append( _T("-s") );
				}
				cmd.Append( _T("-i") );

				cmd.SetInputVar( "Change", _T("new") );
				cmd.SetInputVar( "Description", desc.Text() );
				cmd.SetInputVar( "Files", (TCHAR *)files->Text() );
				if ( jobs->Length() > 0 ) {
					cmd.SetInputVar( "Jobs", (TCHAR *)jobs->Text() );
				}
				con.Exec( &cmd, &results );

				ResultsDialog resultsDialog( this, _T("submit"), &results );
				resultsDialog.Show( host->GetInstance(), host->GetWin() );

				try {
					// Relock document if keeping checked out
					if ( dialog.GetKeepOut() && options->GetOptionValue( LOCK_ON_CHECK_OUT_OPTION ) ) {
						DoFileOp( _T("lock"), filename.Text(), false );
					}
				}
				catch ( exception e ) {
					// Ignore any errors from attempting lock
				}
			}
		}
		catch ( exception e ) {
			DisplayError( e );
		}
	}
}

void PluginOperations::ShowLastResults()
{
	ResultsDialog dialog( this, _T("last"), &results );
	dialog.Show( host->GetInstance(), host->GetWin() );
}

bool PluginOperations::GetDepotPath( PerforceConnection& con, TCHAR* filename, StrBuf& buff )
{
	PerforceCommand cmd( _T("opened"), filename );
	con.Exec( &cmd, &results );
	if ( results.Count() == 1 ) {
		StrDict* dict = ( StrDict * )results.Get( 0 );
		StrPtr* depotFile = dict->GetVar( "depotFile" );
		if ( depotFile != NULL ) {
			buff.Set( depotFile );
			return true;
		}
	}
	return false;
}

TCHAR *PluginOperations::AddOutsideRoot( TCHAR *op, PerforceResults &results )
{
	if ( _tcscmp( op, _T("add") ) == 0 && results.Count() > 0 ) {
		UniStrDict *dict = (UniStrDict *)results.Get( 0 );
		StrPtr *msg = dict->GetVar( "msg" );
		if ( msg != NULL ) {
			TCHAR *root = _tcsstr( (TCHAR *)msg->Text(), P4_OPERATIONS_NOT_UNDER_ROOT );
			if ( root != NULL ) {
				return root + 27;
			}
		}
	}
	return NULL;
}

bool PluginOperations::AddOutsideView( TCHAR *op, PerforceResults &results )
{
	if ( _tcscmp( op, _T("add") ) == 0 && results.Count() > 0 ) {
		UniStrDict *dict = (UniStrDict *)results.Get( 0 );
		StrPtr *msg = dict->GetVar( "msg" );
		if ( msg != NULL ) {
			if ( _tcsstr( (TCHAR *)msg->Text(), P4_OPERATIONS_NOT_IN_VIEW ) != NULL ) {
				return true;
			}
		}
	}
	return false;
}

bool PluginOperations::DoFileOp( TCHAR *op, TCHAR *filename, bool show, PerforceResults *res )
{
	if ( res == NULL ) {
		res = &results;
	}

	try {
		CONNECTION( this );
		PerforceCommand cmd( op, filename );
		con.Exec( &cmd, res );
		// Check for adding files not under the client root
		TCHAR *root = AddOutsideRoot( op, *res );
		if ( root != NULL ) {
			UniStrBuf msg;
			msg.Set( P4_OPERATIONS_UNABLE_ADD_ROOT );
			msg.Append( root );
			MessageBox( host->GetWin(), msg.Text(), P4_OPERATIONS_ADD_ERROR, MB_ICONERROR | MB_OK );
			return false;
		}
		// Check for adding files outside client view
		else if ( AddOutsideView( op, results ) ) {
			UniStrBuf msg;
			msg.Set( P4_OPERATIONS_UNABLE_ADD_VIEW );
			MessageBox( host->GetWin(), msg.Text(), P4_OPERATIONS_ADD_ERROR, MB_ICONERROR | MB_OK );
			return false;
		}
		else if ( show ) {
			FileOpDialog dialog( this, op, filename, res, options );
			dialog.Show( host->GetInstance(), host->GetWin() );
		}
		return true;
	}
	catch ( exception e ) {
		DisplayError( e );
		return false;
	}
}

void PluginOperations::SetPassword( TCHAR *password )
{
	this->password.Set( password );
}

// Make the function Unicode compatible.
//
TCHAR *PluginOperations::GetFilename( TCHAR *path )
{
	for ( size_t i=_tcslen(path)-1; i>=0; i-- ) {
		if ( path[i] == _T('\\') || path[i] == _T('/') ) {
			return path + i + 1;
		}
	}
	return path;
}

// Show file contents for user to see.
// By using ShellExecuteEx, the file will be opened with whichever
// app the user has associated with that file extension.
// For PDF, the browser may open the file if Adobe Reader is not installed.
bool PluginOperations::ShowFile( HWND hwndCaller, TCHAR *path )
{
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = hwndCaller;
	ShExecInfo.lpVerb = NULL; // This does the default, which is "open".
	ShExecInfo.lpFile = path;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE; // Don't show command prompt.
	ShExecInfo.hInstApp = NULL;
	return ShellExecuteEx(&ShExecInfo) != 0; // Avoid warning C4800.
}