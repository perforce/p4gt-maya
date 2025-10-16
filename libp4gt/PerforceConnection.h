/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_CONNECTION
#define INC_PERFORCE_CONNECTION

#include <exception>
using namespace std;

#include "PerforceResults.h"
#include "PerforceCommand.h"
#include "PluginOperations.h"

class PluginOperations;

#define INVALID_PASSWORD_MSG		_T("P4 password (P4PASSWD) invalid or unset.")
// There are two expired messages:
// "Your session was logged out, please login again."
// "Your session has expired, please login again."
#define SESSION_EXPIRED_MSG			_T("please login again")

// Number return from get protocol if server support login, logout etc.
#define SERVER_SECURITY_PROTOCOL	18

// Minimum server level supporting extended submit options
#define SERVER_EXTENDED_SUBMIT		22

/*
 * Class to establish a connection to the server, execute commands and
 * return results
 */
class PerforceConnection : public ClientApi, public ClientUser {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 */
						PerforceConnection( PluginOperations *ops );

	/*
	 * Destructor
	 */
//						~PerforceConnection() throw ( exception );
						~PerforceConnection() throw ( );

	/*
	 * Execute command and result results
	 */
//	void				Exec( PerforceCommand* cmd, PerforceResults* results ) throw ( exception );
	// C++ 11 requires new exception syntax.
	void				Exec( PerforceCommand* cmd, PerforceResults* results ) throw ( ... );

	/*
	 * Capture Perforce form output
	 *
	 * varList - Dictionary containing form data
	 */
	void				OutputStat( StrDict* varList );

	/*
	 * Capture Perforce into output
	 *
	 * level - Level of message
	 * data - The message
	 */
	void				OutputInfo( char level, const char *data );

	/*
	 * Capture Perforce error output
	 *
	 * data - The error message
	 */
	void				OutputError( const char *data );

	/*
	 * Return input form to Perforce
	 *
	 * strbuf - Buffer to return form
	 * e - Error information
	 */
	void				InputData( StrBuf *strbuf, Error *e );

	/*
	 * Override ClientUser::Prompt to provide input to p4 login
	 */
	void				Prompt( const StrPtr &msg, StrBuf &rsp, int noEcho, Error *e );

	/*
	 * Get Current directory (in unicode)
	 */
	const StrPtr		&GetCwd();

private:

	/*
	 * Connect to server.
	 */
//	void				Connect() throw ( exception );
	void				Connect() throw ( );

	/*
	 * Disconnect from server.
	 */
//	void				Disconnect() throw ( exception );
	void				Disconnect() throw ( );

	/*
	 * Get information about the current connection
	 */
	void				GetInfo();

	/*
	 * Obtain a ticket by doing p4 login
	 *
	 * password - Password to use for login
	 */
	void				DoLogin( TCHAR *password );

	/*
	 * Throw Perforce error.
	 */
	void				ThrowError( Error &err );

	PluginOperations	*ops;
	PerforceCommand		*cmd;
	bool				connected;
	PerforceResults		*results;
	TCHAR				*loginPassword;
};

//
// Fix to get P4CONFIG to work. It seems the current directory must be
// set before the connection is created. SetCwd does not work here
//
#define CONNECTION(x) x->SetFileDir();PerforceConnection con(x);x->RestoreOldDir()

#endif
