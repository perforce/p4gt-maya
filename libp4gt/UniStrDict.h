/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_UNISTRDICT
#define INC_UNISTRDICT

#include <tchar.h>

#include "PerforceAPI.h"
#include "PerforceResults.h"
#include "UniStrBuf.h"
#include "Translate.h"

/*
 * Unicode StrDict
 */
class UniStrDict : public StrPtrDict {

public:

	UniStrDict() {};
	UniStrDict( UniStrDict *input );
	~UniStrDict();

	StrPtr	*GetVar( const char *var );
	StrPtr  *GetVar( StrPtr &var );
	int		GetVar( int x, StrRef &var, StrRef &val );
	void	SetVar( const char *var, const char *value );
	void	SetVar( const char *var, int value );
	void	Print( UniStrBuf *buff );
};

#endif