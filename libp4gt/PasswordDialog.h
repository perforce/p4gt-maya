/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PASSWORD_DIALOG
#define INC_PASSWORD_DIALOG

#include "Dialog.h"
#include "PerforceAPI.h"

/*
 * PasswordDialog - allow user to enter a password.
 */

class PasswordDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * user - Username.
	 * port - P4PORT.
	 *
	 */

						PasswordDialog( PluginOperations *ops, const TCHAR *user, const TCHAR *port );

	/* 
	 * Destructor
	 */
						~PasswordDialog();

	/*
	 * Set the password.
	 */
	void				SetPassword( TCHAR *password ) { this->password = password; };


	/*
	 * Get the password.
	 */
	TCHAR				*GetPassword() { return password; };

	/*
	 * Get the user.
	 */
	const TCHAR			*GetUser() { return user; };

	/*
	 * Get the port.
	 */
	const TCHAR			*GetPort() { return port; };


private:
	
	/* 
	 * Dialog windows procedure
	 *
	 * hWnd - Handle of dialog window.
	 * uMsg - Windows message.
	 * wParam - Message wParam.
	 * lParam - Message lParam.
	 */
	static INT_PTR CALLBACK	DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	TCHAR					*password;
	const TCHAR				*port;
	const TCHAR				*user;
};

#endif