/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_COMMAND
#define INC_PERFORCE_COMMAND

#include <tchar.h>

#include "PerforceAPI.h"
#include "Translate.h"

/*
 * Class to execute a Perforce command
 */
class PerforceCommand {

public:

	/*
	 * Constructer for executing a single command.
	 *
	 * cmd - Command to execute.
	 */
				PerforceCommand( TCHAR *cmd );

	/* 
	 * Constructor for executing a command with 1 argument
	 *
	 * cmd - Command to execute
	 * arg1 - First argument to command
	 */
				PerforceCommand( TCHAR* cmd, TCHAR *arg1 );

	/* 
	 * Constructor for executing a command with 2 arguments
	 *
	 * cmd - Command to execute
	 * arg1 - First argument to command
	 * arg2 - Second argument to command
	 */
				PerforceCommand( TCHAR* cmd, TCHAR *arg1, TCHAR *arg2 );

	/* 
	 * Constructor for executing a command with 3 arguments
	 *
	 * cmd - Command to execute
	 * arg1 - First argument to command
	 * arg2 - Second argument to command
	 * arg3 - Third argument to command
	 */
				PerforceCommand( TCHAR* cmd, TCHAR *arg1, TCHAR *arg2, TCHAR *arg3 );

	/* 
	 * Constructor for executing a command with 4 arguments
	 *
	 * cmd - Command to execute
	 * arg1 - First argument to command
	 * arg2 - Second argument to command
	 * arg3 - Third argument to command
	 * arg4 - Fourth argument to command
	 */
				PerforceCommand( TCHAR* cmd, TCHAR *arg1, TCHAR *arg2, TCHAR *arg3, TCHAR *arg4 );

	/*
	 * Destructor
	 */
				~PerforceCommand();
	
	/*
	 * Get the command
	 */
	char		*GetCommand();

	/*
	 * Get the command arguments
	 */
	char		**GetArgs();

	/*
	 * Get the number of arguments
	 */
	int			GetNumArgs();

	/*
	 * Append another argument
	 */
	void		Append( TCHAR *arg );

	/* 
	 * Get the input dictionary
	 */
	StrDict		*GetInput();


	/*
	 * Set an input variable
	 *
	 * var - Name of variable
	 * val - Value of variable
	 */
	void		SetInputVar( char *var, TCHAR *val );

private:

	TCHAR		*command;
	VarArray	args;
	VarArray	buffs;
	StrBufDict	input;
};

#endif