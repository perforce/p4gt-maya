/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "StrBufArray.h"

StrBufArray::~StrBufArray() 
{
	for ( int i=0; i<Count(); i++ ) {
		StrBuf *buff = (StrBuf *)VarArray::Get( i );
		delete buff;
	}
}

void StrBufArray::Put( TCHAR *v ) 
{
	StrBuf *buff = new StrBuf();
	// Possible loss of data if size_t larger then MAXINT.
	p4size_t len = (p4size_t)_tcslen( v );
	buff->Set( (const char *)v, (len + 1) * sizeof( TCHAR ) );
	buff->SetLength( len * sizeof(TCHAR) );
	VarArray::Put( buff );
}

TCHAR *StrBufArray::Get( int i ) 
{
	StrBuf *buff = (StrBuf *)VarArray::Get( i );
	return (TCHAR *)buff->Text();
}
