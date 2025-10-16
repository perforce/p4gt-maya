/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_UNISTRBUF
#define INC_UNISTRBUF

#include <tchar.h>

#include "PerforceAPI.h"

/*
 * Unicode StrBuf
 */
class UniStrBuf : public StrBuf {

public:

	void	Append( const TCHAR *buf );

	void	Append( const TCHAR *buf, p4size_t len );

	void	Append( const StrPtr *s );

	void	Set( const TCHAR *buf );

	void	Set( const TCHAR *buf, p4size_t len );

	void	Set( const StrPtr *s );

	TCHAR	*Text() const {return (TCHAR *)StrBuf::Text();}

	int 	Length() const;
};

#endif
