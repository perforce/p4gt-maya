/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_STRBUF_ARRAY
#define INC_STRBUF_ARRAY

#include <tchar.h>

#include "PerforceAPI.h"

/*
 * Array of StrBuf's
 */
class StrBufArray : public VarArray {

public:

	/* 
	 * Destructor.
	 */
						~StrBufArray();

	/* 
	 * Add string to array.
	 */
	void				Put( TCHAR *v );

	/*
	 * Get string from array
	 *
	 * i - The element to get
	 */
	TCHAR				*Get( int i ); 

};

#endif
