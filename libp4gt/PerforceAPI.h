/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_API
#define INC_PERFORCE_API

/*
 * Include all Perforce client API headers
 */
#include <clientapi.h>
#include <vararray.h>
#include <strtable.h>
#include <spec.h>

// This is defined to the following in stdhdrs.h:
// # define tolower(x) ((x)>='A'&&(x)<='Z'?(x)-'A'+'a':(x))
// and it causes major compilation failures when including <iostream> after it.
// Therefore remove as we don't use it.
#undef tolower

#endif