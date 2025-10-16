/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "Translate.h"

#include "debug.h"

#ifdef _UNICODE
#ifndef LANG_ja

void Translate::ToWinChar( const char *s, UniStrBuf *b )
{
	b->Clear();
	int nchars = MultiByteToWideChar( CP_UTF8, 0, s, -1, NULL, 0);
	TCHAR *buff = (TCHAR *)b->Alloc( ( nchars + 1 ) * sizeof( TCHAR ) );
	int ret = MultiByteToWideChar( CP_UTF8, 0, s, -1, buff, nchars );
	buff[nchars] = '\0';
}

void Translate::FromWinChar( TCHAR *s, StrBuf *b )
{
	b->Clear();
	int buflen = WideCharToMultiByte( CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL );
	char *buff = b->Alloc( buflen );
	WideCharToMultiByte( CP_UTF8, 0, s, -1, buff, buflen, NULL, NULL );
	b->SetLength( buflen-1 );
}

#else

void Translate::ToWinChar( const char *s, UniStrBuf *b )
{
	b->Clear();
	int nchars = MultiByteToWideChar( 932, 0, s, -1, NULL, 0);
	TCHAR *buff = (TCHAR *)b->Alloc( ( nchars + 1 ) * sizeof( TCHAR ) );
	int ret = MultiByteToWideChar( 932, 0, s, -1, buff, nchars );
	buff[nchars] = '\0';
}

void Translate::FromWinChar( TCHAR *s, StrBuf *b )
{
	b->Clear();
	int buflen = WideCharToMultiByte( 932, 0, s, -1, NULL, 0, NULL, NULL );
	char *buff = b->Alloc( buflen );
	WideCharToMultiByte( 932, 0, s, -1, buff, buflen, NULL, NULL );
	b->SetLength( buflen-1 );
}

#endif	// LANG=ja

#else

void Translate::ToWinChar( const char *s, UniStrBuf *b )
{
	b->Set( s );
}

void Translate::FromWinChar( TCHAR *s, StrBuf *b )
{
	b->Set( s );
}

#endif	// _UNICODE