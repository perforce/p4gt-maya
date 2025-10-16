/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_OPTIONS
#define INC_PERFORCE_OPTIONS

#include <commctrl.h>

#include "PerforceAPI.h"
#include "UniStrBuf.h"

#define CHECK_IN_ON_CLOSE_OPTION		1
#define CHECK_OUT_ON_OPEN_OPTION		2
#define CHECK_OUT_ON_REOPEN_OPTION		3
#define CHECK_IN_ON_ADD_OPTION			4
#define LOCK_ON_CHECK_OUT_OPTION		5
#define SHOW_CONFIRMATION_OPTION		6
#define OTHER_DOCUMENTS_OPTION			7
#define SYNC_FILE_REF_OPTION			8
#define PERFORCE_OPEN_VIEW_TYPE			9
#define PERFORCE_OPEN_WIDTH				10
#define PERFORCE_OPEN_HEIGHT			11
#define SERVER_ONLINE					12
#define PERFORCE_OPEN_DIR				13
#define PERFORCE_OPEN_FILE_TYPE			14
#define REVERT_ON_CLOSE_OPTION			15

#define NEVER_OPTION					0
#define ALWAYS_OPTION					1
#define PROMPT_OPTION					2

#define SOFTWARE_KEY					_T("Software")
#define PERFORCE_KEY					_T("P4")

/*
 * Class to handle saving/getting of perforce options in the registry
 */
class PerforceOptions {

public:

	/*
	 * Constructor.
	 *
	 * name - Name of main key to store options under.
	 */
			PerforceOptions( TCHAR *name );

	/*
	 * Get the value of the option. Returns a default of the value is not
	 * found in the registry.
	 *
	 * option - The code for the option.
	 */
	DWORD	GetOptionValue( int option );

	/*
	 * Get the string value of the option. Returns false if the value is not
	 * found in the registry.
	 *
	 * option - The code for the option.
	 */
	bool	GetOptionString( int option, UniStrBuf *buff );

	/*
	 * Set the option value.
	 *
	 * option - The option code.
	 * value - The value to set.
	 */
	void	SetOptionValue( int option, DWORD value );

	/*
	 * Set the option string.
	 *
	 * option - The option code.
	 * value - The string value to set.
	 */
	void	SetOptionString( int option, TCHAR *value );

private:

	/*
	 * Get the name of the option in the registry given the option code.
	 *
	 * option - The option code.
	 */
	TCHAR		*GetOptionName( int option );

	TCHAR		*name;
	UniStrBuf	regKeyName;
};

#endif
