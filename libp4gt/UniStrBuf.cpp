/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "UniStrBuf.h"

#include "debug.h"

void UniStrBuf::Append( const TCHAR *buf, p4size_t len )
{
	StrBuf::Extend( (char *)buf, len );
	StrBuf::Extend( "\0\0", 2 );
	length -= 2;
}

void UniStrBuf::Append( const TCHAR *buf ) 
{
	// Casting and truncation of data ok. 
	p4size_t len = (p4size_t)(_tcslen( buf ) * sizeof( TCHAR ));
	Append( buf, len );
}

void UniStrBuf::Append( const StrPtr *s )
{
	Append( (TCHAR *)s->Text() );
}

void UniStrBuf::Set( const TCHAR *buf, p4size_t len )
{
	Clear();
	StrBuf::Extend( (char *)buf, len );
	StrBuf::Extend( "\0\0", 2 );
	length -= 2;
}

void UniStrBuf::Set( const TCHAR *buf )
{
	// Casting and truncation of data ok. 
	p4size_t len = (p4size_t)(_tcslen( buf ) * sizeof( TCHAR ));
	Set( buf, len );
}

void UniStrBuf::Set( const StrPtr *s )
{
	Set( (TCHAR *)s->Text() );
}

int UniStrBuf::Length() const
{
	return length / sizeof( TCHAR );
}
