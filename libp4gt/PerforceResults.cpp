/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PerforceResults.h"

#include "debug.h"

PerforceResults::PerforceResults() : text( NULL ), hasErrors( false )
{
}

PerforceResults::~PerforceResults()
{
	Reset();
}

void PerforceResults::Reset()
{
	// Delete all dictionarys
	for ( int i=0; i<Count(); i++ ) {
		delete ( StrBufDict * )Get( i );
	}
	if ( text != NULL ) {
		delete []text;
		text = NULL;
	}
	// Clear array members
	Clear();

	// Clear errors
	hasErrors = false;
}

TCHAR *PerforceResults::GetText()
{
	if ( text == NULL ) {
		UniStrBuf buff;
		for ( int i=0; i<Count(); i++ ) {
			UniStrDict *dict = ( UniStrDict * )Get( i );
			StrPtr *msg = dict->GetVar( "msg" );
			// Result was just a message
			if ( msg != NULL ) {
				buff.Append( msg );
				buff.Append( _T("\n") );
			}
			// Result was a dictionary
			else {
				dict->Print( &buff );
			}
		}

		// Replace all newlines with carriage return\ newlines.
		text = new TCHAR[ ( buff.Length() * 2 ) + 1 ];
		int j = 0;
		TCHAR *unibuf = (TCHAR *)buff.Text();
		int len = buff.Length();
		for ( int i = 0; i < len; i++ ) {
			TCHAR ch = unibuf[i];
			if ( ch == 0x0A ) {
				text[j++] = '\r';
				text[j++] = '\n';
			}
			else {
				text[j++] = ch;
			}
		}
		text[j++] = '\0';
	}

	return text;
}

void PerforceResults::Append( StrDict* result )
{
	UniStrDict* dict = new UniStrDict();
	StrRef var, val;
	for( int i = 0; result->GetVar( i, var, val ); i++ ) {
		if ( var != "func" ) {
			dict->SetVar( var.Text(), val.Text() );
		}
	}
	dict->SetVar( RESULT_KEY , RESULT_DICTIONARY );
	Put( dict );
}

void PerforceResults::AppendError( const char *msg )
{
	UniStrDict* dict = new UniStrDict();
	dict->SetVar( RESULT_KEY, RESULT_DICTIONARY );
	dict->SetVar( "msg", msg );
	Put( dict );
	// Ignore
	//		"no such file(s)"				when doing p4 fstat on file not
	//										under perforce
	//		"file(s) up-to-date"			when doing p4 sync
	//		"is not under client's root"	when doing p4 fstat on file opened
	//										outside root messages
	//		"must refer to client			when doing p4 dirs on depot that
	//										does not exist
	//		"file(s) not in client view		when doing p4 fstat on file opend
	//										outside view (but not outside root)

	TCHAR *umsg = (TCHAR *)dict->GetVar( "msg" )->Text();

	if ( _tcsstr ( umsg, _T("no such file(s)") ) == NULL &&
		 _tcsstr ( umsg, _T("file(s) up-to-date") ) == NULL &&
		 _tcsstr ( umsg, _T("must refer to client") ) == NULL &&
		 _tcsstr ( umsg, _T("is not under client's root") ) == NULL &&
		 _tcsstr ( umsg, _T("file(s) not in client view") ) == NULL ) {
		hasErrors = true;
	}
}

void PerforceResults::Append( char level, const char *msg )
{
	UniStrDict* dict = new UniStrDict();
	dict->SetVar( RESULT_KEY, RESULT_MESSAGE );
	dict->SetVar( "level", (int)level );
	dict->SetVar( "msg", msg );
	Put( dict );
}
