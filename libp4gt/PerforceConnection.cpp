/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PerforceConnection.h"
#include "PasswordDialog.h"
#include "Translate.h"
#include "debug.h"
#include <tchar.h>
#include "version.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

#include <i18napi.h>

PerforceConnection::PerforceConnection( PluginOperations *ops ) : connected( false ), loginPassword( NULL )
{
	this->ops = ops;
}

PerforceConnection::~PerforceConnection()
{
	if ( connected ) {
		Disconnect();
	}
}

void PerforceConnection::Exec( PerforceCommand* cmd, PerforceResults* results )
{
	this->cmd = cmd;
	this->results = results;

	// Connect if not connected
	if ( !connected ) {
		Connect();
	}

	// Set the program name and version for server reporting
	SetProg( "p4gt" );
	StrBuf buff;
	Translate::FromWinChar( _T(P4GT_VERSION), &buff );
	SetVersion( buff.Text() );

	// Check if we have got connection info
	if ( !ops->GotConnectionInfo() ) {
		GetInfo();
	}

	while ( true ) {

		// Check if we have a previously entered password
		if ( ops->GetPassword().Length() > 0 ) {
			SetPassword( &ops->GetPassword() );
		}

		// Clear any existing results
		results->Reset();

		// Set the command args
		SetArgv( cmd->GetNumArgs(),  (char *const *)cmd->GetArgs() );

		// Execute the command
		Run( cmd->GetCommand(), this );

		// Check for errors
		if ( results->HasErrors() ) {
			TCHAR *msg = results->GetText();
			if ( _tcsncmp( msg, P4_CONNECTION_INVALID_PASSWORD, _tcslen( P4_CONNECTION_INVALID_PASSWORD ) ) == 0 ||
				 _tcsstr( msg, P4_CONNECTION_LOGIN ) != NULL ) {
				UniStrBuf user;
				Translate::ToWinChar( GetUser().Text(), &user );
				UniStrBuf port;
				Translate::ToWinChar( GetPort().Text(), &port );
				PasswordDialog dialog( ops, user.Text(), port.Text() );
				if ( dialog.Show( ops->GetHost()->GetInstance(), ops->GetHost()->GetWin() ) == TRUE ) {
					if ( ops->GetUseLogin() ) {
						DoLogin( dialog.GetPassword() );
					}
					else {
						ops->SetPassword( dialog.GetPassword() );
					}
					continue;
				}
			}

			// Ignore error if server supports new submit options
			//    i.e. "p4 submit -f leaveunchanged"
			//
			if ( ops->GetExtSubmit() ) {
				if ( _tcsstr( msg, _T("No files to submit.") ) != NULL ) {
					break;
				}
			}
			TCHAR *err = results->GetText();
			StrBuf utf8;
			Translate::FromWinChar( err, &utf8 );
			throw exception( utf8.Text() );
		}
		else {
			break;
		}
	}
}

void PerforceConnection::Prompt( const StrPtr &msg, StrBuf &rsp, int noEcho, Error *e )
{
	if ( loginPassword != NULL ) {
		StrBuf buff;
		Translate::FromWinChar( loginPassword, &buff );
		rsp << buff.Text();
	}
}

void PerforceConnection::OutputStat( StrDict* varList )
{
	SpecDataTable specData;
	StrPtr *data = varList->GetVar( "data" );
	StrPtr *spec = varList->GetVar( "specdef" );
	StrDict *dict = varList;

	// Check if have form output
	if ( spec && data ) {
		// Parse the form data using the spec
		// and load the result into the SpecDataTable
		// That stores its contents in a StrDict.
		// Use that for our results
		Error e;
		Spec s( spec->Text(), "", &e );
		s.Parse( data->Text(), &specData, &e );
		if ( e.Test() ) {
			HandleError( &e );
			return;
		}
		dict = specData.Dict();
		dict->SetVar( "data", data );
	}

	// Append dictionary output to results
	results->Append( dict );
}

void PerforceConnection::OutputInfo( char level, const char *data )
{
	// Append info output to results
	results->Append( level, data );
}

void PerforceConnection::OutputError( const char *data )
{
	// Append error output to results
	results->AppendError( data );
}

void PerforceConnection::InputData( StrBuf *strbuf, Error *e )
{
	StrRef var, val;
	StrDict *input = cmd->GetInput();

	strbuf->Clear();

	for( int i = 0; input->GetVar( i, var, val ); i++ ) {
		*strbuf << var << ":\t" << val << "\n";
	}
}

void PerforceConnection::DoLogin( TCHAR *password )
{
	// Clear any existing results
	results->Reset();

	// Set the command args
	SetArgv( 0,  NULL );

	loginPassword = password;
	Run( "login", this );
	loginPassword = NULL;
}

void PerforceConnection::GetInfo()
{
	// Clear any existing results
	results->Reset();

	// Execute the "info" command
	Run( "info" , this );

	// Check server level
	StrPtr *version = GetProtocol( "server2" );
	if ( version != NULL ) {

		// Login/logout capable [2004.2 higher]
		if ( atol( version->Text() ) >= SERVER_SECURITY_PROTOCOL ) {
			ops->SetUseLogin();
		}

		// Supports new submit options [2006.2 higher]
		if ( atol( version->Text() ) >= SERVER_EXTENDED_SUBMIT ) {
			ops->SetExtSubmit();
		}
	}

	ops->SetGotConnectionInfo();
}

void PerforceConnection::Connect()
{
	Error err;

	SetProtocol( "specstring", "" );
	SetProtocol( "tag", "" );
	SetProtocol( "api", "57" );
	SetProtocol( "enableStreams", "" );

#ifndef LANG_ja
	StrPtr cs = GetCharset();
	if ( cs.Length() && CharSetApi::Lookup( cs.Text() ) > 0 ) {
		SetTrans( CharSetApi::UTF_8 );
	}
#else
		SetTrans( CharSetApi::SHIFTJIS );
#endif  // LANG_ja

	// Connect to server
	Init( &err );
	if ( err.Test() ) {
		ThrowError( err );
	}
	else {
		// Make sure we get english messages
		SetLanguage( "en" );

		// Clear any existing results
		results->Reset();

		PerforceCommand* cmd = new PerforceCommand( _T("trust"), _T("-y") );

		// Set the command args
		SetArgv( cmd->GetNumArgs(),  (char *const *)cmd->GetArgs() );

		// Execute the command
		Run( cmd->GetCommand(), this );

		connected = true;
	}
}

void PerforceConnection::Disconnect()
{
	Error err;
	Final( &err );
	if ( err.Test() ) {
		// Ignore disconnect errors
	}
	else {
		connected = false;
	}
}

void PerforceConnection::ThrowError( Error &err )
{
	StrBuf buf;
	err.Fmt( buf, EF_NEWLINE );
	throw exception( buf.Text() );
}

const StrPtr &PerforceConnection::GetCwd()
{
	StrPtr cd = ClientApi::GetCwd();
	static UniStrBuf buff;
	Translate::ToWinChar( cd.Text(), &buff );
	return buff;
}
