/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "UniStrDict.h"

#include "debug.h"


UniStrDict::UniStrDict( UniStrDict *input )
{
	StrRef var, val;
	for( int i = 0; input->GetVar( i, var, val ); i++ ) {
		StrBuf buff;
		Translate::FromWinChar( (TCHAR *)val.Text(), &buff );
		SetVar( var.Text(), buff.Text() );
	}
}

UniStrDict::~UniStrDict()
{
	StrRef var, val;
	for( int i = 0; StrPtrDict::GetVar( i, var, val ); i++ ) {
		delete []var.Text();
		delete (UniStrBuf *)val.Text();
	}
}

int UniStrDict::GetVar( int x, StrRef &var, StrRef &val )
{
	int ret = StrPtrDict::GetVar( x, var, val );
	if ( ret ) {
		val.Set( (StrPtr *)val.Text() );
	}
	return ret;
}

StrPtr *UniStrDict::GetVar( const char *var )
{
	StrPtr *p = StrPtrDict::GetVar( var );
	if ( p != NULL ) {
		return (UniStrBuf *)p->Text();
	}
	return NULL;
}

StrPtr *UniStrDict::GetVar( StrPtr &var )
{
	return GetVar( var.Text() );
}

void UniStrDict::SetVar( const char *var, const char *value )
{
	char *varbuff = new char[ strlen( var ) + 1];
	strcpy( varbuff, var );
	UniStrBuf *valbuff = new UniStrBuf();
	Translate::ToWinChar( value, valbuff );
	StrPtrDict::SetVar( varbuff, (char *)valbuff );
}

void UniStrDict::SetVar( const char *var, int value )
{
	char buff[24];
	itoa( value, buff, 10 );
	SetVar( var, buff );
}

void UniStrDict::Print( UniStrBuf *buff )
{
	StrRef var, val;
	UniStrBuf wideVar;
	for( int i = 0; StrPtrDict::GetVar( i, var, val ); i++ ) {
		if ( var != RESULT_KEY ) {
			Translate::ToWinChar( var.Text(), &wideVar );
			buff->Append( &wideVar );
			buff->Append( _T(": ") );
			buff->Append( (StrPtr *)val.Text() );
			buff->Append( _T("\n") );
		}
	}
}

