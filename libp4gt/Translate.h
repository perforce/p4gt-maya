/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_TRANSLATE
#define INC_TRANSLATE

#include <windows.h>
#include <tchar.h>

#include "PerforceAPI.h"
#include "UniStrBuf.h"

/*
 * Unicode StrBuf
 */
class Translate {

public:

	static void		ToWinChar( const char *s, UniStrBuf *b );
	static void		FromWinChar( TCHAR *s, StrBuf *b );
};

#endif