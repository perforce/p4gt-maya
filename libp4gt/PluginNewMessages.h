/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 *
 * This file contains all plugin text and messages
 */

/*
 * Messages used in ..\photoshop\PerforceMenu.cpp, ..\softimage\P4GTSoftImage.cpp
 */
#define	P4_MENU_ADD																_T("&Add to P4")
#define	P4_MENU_CHECKOUT												_T("Check &Out")
#define	P4_MENU_CHECKIN														_T("Check &In...")
#define	P4_MENU_RESULT_CMD											_T("&Result of Last Command...")
#define	P4_MENU_DOC_STAT													_T("File &Status...")
#define	P4_MENU_DOC_HIST													_T("Fi&le History...")
#define	P4_MENU_DOC_CHECKED_OUT							_T("Checked Out &Files...")
#define	P4_MENU_SERVER_INFO											_T("&P4 Information...")
#define	P4_MENU_GET_LATEST											_T("Get Latest Revision &from P4...")
#define	P4_MENU_OPTIONS														_T("Op&tions...")
#define	P4_MENU_HELP																_T("&Help...")
#define	P4_MENU_ABOUT															_T("A&bout P4GT...")
#define	P4_MENU_SERVER_ONLINE									_T("Ser&ver Online")
#define	P4_MENU_PERFORCE												_T("&P4")
#define	P4_MENU_LOCK																_T("Lock File")
#define	P4_MENU_UNLOCK														_T("Unlock File")
#define	P4_MENU_LOCK_UNLOCK											_T("Lock/Unlock File")
#define	P4_MENU_UNDO_ADD												_T("Undo Add")
#define	P4_MENU_UNDO_CHECKOUT								_T("Undo Check Out")
#define	P4_MENU_UNDO_ADD_CHECKOUT					_T("Undo Add/Check Out")

/*
 * Messages used in PerforceConnection.cpp
 */
#define	P4_CONNECTION_INVALID_PASSWORD			_T("P4 password (P4PASSWD) invalid or unset.")
#define	P4_CONNECTION_LOGIN											_T("please login again")

/*
 * Messages used in ExternalRefDialog.cpp
 */
#define	P4_EXTERNALREF_NAME										_T("Name");
#define	P4_EXTERNALREF_HAVE											_T("Have Revision");
#define	P4_EXTERNALREF_HEAD											_T("Head Revision");

/*
 * Messages used in FileOpDialog.cpp
 */
#define	P4_FILEOP_DEPOT_FILE											_T("Depot file: ")
#define	P4_FILEOP_YOUR_DOC												_T("Your file: ")
#define	P4_FILEOP_ACTION														_T("Latest (head) action in depot: ")
#define	P4_FILEOP_TYPE															_T("File type: ")
#define	P4_FILEOP_LAST_MOD												_T("Last modified in depot: ")
#define	P4_FILEOP_HEAD_REV												_T("Latest (head) revision in depot: ")
#define	P4_FILEOP_HEAD_CHG												_T("Latest (head) changelist in depot: ")
#define	P4_FILEOP_HAVE															_T("Revision in your workspace: ")
#define	P4_FILEOP_OPENED_YOU_FOR							_T("Open by you for: ")
#define	P4_FILEOP_IN_CHG														_T("In changelist: ")
#define	P4_FILEOP_OPENED_OTHER									_T("Open by P4 user: ")
#define	P4_FILEOP_OPENED_OTHER_ALSO					_T("Also open by: ")
#define	P4_FILEOP_OPENED_OTHER_FOR						_T("Opened by them for: ")
#define	P4_FILEOP_LOCKED_YOU											_T("Locked by you")
#define	P4_FILEOP_LOCKED_OTHER									_T("Locked by someone else")
#define	P4_FILEOP_CONF_CHECKOUT								_T("P4GT - Confirmation of Check Out from P4")
#define	P4_FILEOP_CONF_ADD												_T("P4GT - Confirmation of Add to P4")
#define	P4_FILEOP_CONF_ADD_CHECKOUT					_T("P4GT - Confirmation of Undo Add/Check Out")
#define	P4_FILEOP_CONF_LOCK												_T("P4GT - Confirmation of Lock File")
#define	P4_FILEOP_CONF_UNLOCK										_T("P4GT - Confirmation of Unlock File")
#define	P4_FILEOP_CONF_CHECKIN										_T("P4GT - Confirmation of Check In to P4")
#define	P4_FILEOP_DOC_STATUS										_T("P4GT - File Status")

/*
 * Messages used in FullSubmitDialog.cpp
 */
#define	P4_FULLSUBMIT_DOC													_T("Files:")
#define	P4_FULLSUBMIT_JOBS												_T("\nJobs:\n")

											// DO NOT TRANSLATE NEXT LINE
#define	P4_FULLSUBMIT_DESC												_T("<enter description here>")

#define	P4_FULLSUBMIT_CHECKIN										_T("Check In to P4")
#define	P4_FULLSUBMIT_SELECT											_T("Please select at least one file.")
#define	P4_FULLSUBMIT_DFLT												_T("(default)")
#define P4_FULLSUBMIT_OPTION1										_T("Submit all selected files")
#define P4_FULLSUBMIT_OPTION2										_T("Don't submit unchanged files")
#define P4_FULLSUBMIT_OPTION3										_T("Revert unchanged files")

/*
 * Messages used in HistoryDialog.cpp
 */
#define	P4_HISTORY_VER															_T("Ver-")
#define	P4_HISTORY_REV															_T("Revision")
#define	P4_HISTORY_CHG														_T("Changelist")
#define	P4_HISTORY_DATE														_T("Date")
#define	P4_HISTORY_USER														_T("User")
#define	P4_HISTORY_ACTION													_T("Action")
#define	P4_HISTORY_DESC														_T("Description")
#define	P4_HISTORY_HIST														_T("File History for ")
#define	P4_HISTORY_DESC_CHG											_T("Description for changelist ")

/*
 * Messages used in MenuState.cpp
 */
#define	P4_MENUSTATE_LOCK_UNLOCK							_T("Lock/Unlock File")
#define	P4_MENUSTATE_UNDO_ADD_CHECKOUT		_T("Undo Add/Check Out")
#define	P4_MENUSTATE_DISC												_T("Disconnect from Ser&ver")
#define	P4_MENUSTATE_CONN												_T("Connect to Ser&ver")
#define	P4_MENUSTATE_UNLOCK										_T("Un&lock File")
#define	P4_MENUSTATE_LOCK												_T("&Lock File")
#define	P4_MENUSTATE_UNDO_ADD								_T("U&ndo Add")
#define	P4_MENUSTATE_UNDO_CHECKOUT					_T("U&ndo Check Out")

/*
 * Messages used in OldVersionDialog.cpp
 */
#define	P4_OLDVERSION_DOC												_T("The file '")
#define	P4_OLDVERSION_CHECKED_IN								_T("' is in P4.")
#define	P4_OLDVERSION_OLD_REV									_T("You have an old version of this file (")
#define	P4_OLDVERSION_HEAD_REV									_T(") and the newest version in the P4 depot is ")

/*
 * Messages used in OpenDepotDialog.cpp
 */
#define	P4_OPENDEPOT_ALL													_T("All Files (*.*)")
#define	P4_OPENDEPOT_NAME												_T("Name")
#define	P4_OPENDEPOT_HAVE												_T("Have Revision")
#define	P4_OPENDEPOT_HEAD												_T("Head Revision")
#define	P4_OPENDEPOT_TYPE												_T("Type")
#define	P4_OPENDEPOT_LOCKED_BY								_T("Locked by")
#define	P4_OPENDEPOT_LARGE_ICON								_T("Large Icons")
#define	P4_OPENDEPOT_SMALL_ICON								_T("Small Icons")
#define	P4_OPENDEPOT_LIST												_T("List")
#define	P4_OPENDEPOT_DETAILS										_T("Details")
#define	P4_OPENDEPOT_UP_ONE										_T("Up One Level")
#define	P4_OPENDEPOT_YOU												_T("you")

/*
 * Messages used in OpenFilesDialog.cpp
 */
#define	P4_OPENFILES																_T("Files:")

/*
 * Messages used in PasswordDialog.cpp
 */
#define	P4_PASSWORD_PASSWORD									_T("A password is required for user '")
#define	P4_PASSWORD_SERVER											_T("' on server '")

/*
 * Messages used in PluginOperations.cpp
 */
#define	P4_OPERATIONS_ADD_DOC									_T("P4GT - Add File to P4")
#define	P4_OPERATIONS_ADDED_DOC								_T("You have successfully added the file to P4 -\r\n") \
																												_T("would you like to check it in now?")
#define	P4_OPERATIONS_OPENING_DOC						_T("P4GT - Opening File")
#define	P4_OPERATIONS_DOC_IN_P4									_T("This file is in P4.\r\n\r\n") \
																												_T("Click OK to check out.")
#define	P4_OPERATIONS_CLOSING_DOC							_T("P4GT - Closing File")
#define	P4_OPERATIONS_DOC_CHECKED_OUT			_T("This file is checked-out from P4.\r\n\r\n") \
																												_T("Do want to check-in this file?")
#define	P4_OPERATIONS_SERVER_ERROR					_T("P4GT - P4 Error")
#define	P4_OPERATIONS_OPEN											_T("P4GT - Open From P4")
#define	P4_OPERATIONS_SAVE_LATEST							_T("Save the latest changes to the current file before opening P4 file?")
#define	P4_OPERATIONS_ADD_ERROR							_T("P4GT - Error Adding File to P4")
#define	P4_OPERATIONS_SAVE_DOC_ADD						_T("You need to save the file in a directory in the client workspace (P4 controlled area) before you can add it to P4.")
#define	P4_OPERATIONS_VIEW_PREV								_T("P4GT - View Previous Version")
#define	P4_OPERATIONS_SAVE_LATEST_PREV			_T("Save the latest changes to the current file before loading previous version?")
#define	P4_OPERATIONS_REVERT_PREV						_T("Are you sure you wish to discard changes and revert to the previous version?")
#define	P4_OPERATIONS_UNDO_CHECKOUT				_T("P4GT - Undo Check Out")
#define	P4_OPERATIONS_CHECKIN										_T("P4GT - Check In to P4")
#define	P4_OPERATIONS_SAVE_LATEST_CHECKIN	_T("Save the latest changes before checking in to P4?")
#define	P4_OPERATIONS_NOT_CKECKED_OUT			_T("Unable to do Check In as file is not checked out.")
#define	P4_OPERATIONS_ERROR										_T("P4GT - Error")
#define	P4_OPERATIONS_HELP_REGISTRY_NOT_FOUND        _T("Possible installer problem. Unable to find value named HelpFileURL under this registry key: ")
#define	P4_OPERATIONS_SHOW_HELP_ERROR				_T("Unable to show P4GT help file: ")
#define P4_OPERATIONS_CLOSE_REVERT_MSG			_T("This file is checked-out from P4.\r\n\r\n") \
																												_T("Do you want to revert this file?")
#define	P4_OPERATIONS_DOC												_T("The file '")
#define	P4_OPERATIONS_IN_PERFORCE							_T("' is in P4.\r\n\r\n")
#define	P4_OPERATIONS_OK_CHECKOUT						_T("Click OK to check out.")
#define	P4_OPERATIONS_CHECK_P4PORT						_T("check $P4PORT")
#define	P4_OPERATIONS_CHECKED_IN								_T("This file is checked-in to P4.\r\n\r\n")
#define	P4_OPERATIONS_EXCL_LOCKED							_T("The file is currently exclusively locked by someone else: ")
#define	P4_OPERATIONS_UNABLE_CHECKOUT			_T("Check Out is not possible at this time.\r\n")
#define	P4_OPERATIONS_CHECKOUT								_T("P4GT - Check Out File")
#define	P4_OPERATIONS_CHECKED_IN								_T("This file is checked-in to P4.\r\n\r\n")
#define	P4_OPERATIONS_LOCKED_BY								_T("The file has already been locked by someone else: ")
#define	P4_OPERATIONS_UNABLE_AUTO_LOCK			_T("Automatic locking after Check Out is not possible.\r\n")
#define	P4_OPERATIONS_CHECKOUT_NO_LOCK			_T("Do you still want to Check Out without locking it?")
#define	P4_OPERATIONS_CHECKOUT								_T("P4GT - Check Out File")
#define	P4_OPERATIONS_LOCK												_T("This file has been Checked Out. Do you now want to lock the file?")
#define	P4_OPERATIONS_NOT_UNDER_ROOT				_T("is not under client's root")
#define	P4_OPERATIONS_NOT_IN_VIEW							_T("file(s) not in client view")
#define	P4_OPERATIONS_UNABLE_ADD_ROOT			_T("This file cannot be added to P4 as it is outside the client root ")
#define	P4_OPERATIONS_UNABLE_ADD_VIEW				_T("This file cannot be added to P4 as it is outside the client view.")

/*
 * Messages used in ResultsDialog.cpp
 */
#define	P4_RESULTS_USER													_T("User name: ")
#define	P4_RESULTS_CLIENT													_T("Client name: ")
#define	P4_RESULTS_CLIENT_HOST									_T("Client host: ")
#define	P4_RESULTS_CLIENT_ROOT									_T("Client root: ")
#define	P4_RESULTS_CLIENT_CWD									_T("Current directory: ")
#define	P4_RESULTS_CLIENT_ADDR									_T("Client address: ")
#define	P4_RESULTS_UNICODE												_T("Unicode: ")
#define	P4_RESULTS_SERVER_ADDR								_T("Server address: ")
#define	P4_RESULTS_SERVER_ROOT								_T("Server root: ")
#define	P4_RESULTS_SERVER_DATE								_T("Server date: ")
#define	P4_RESULTS_SERVER_VERS								_T("Server version: ")
#define	P4_RESULTS_SERVER_LIC										_T("Server license: ")
#define	P4_RESULTS_CONF_CHECKIN								_T("P4GT - Confirmation of Check In")
#define	P4_RESULTS_RESULT_CMD									_T("P4GT - Result of Last P4 Command")
#define	P4_RESULTS_CONN_INFO										_T("P4GT - P4 Connection Information")

/*
 * Messages used in TabbedOptionsDialog.cpp
 */
#define	P4_TABBEDOPTIONS_CHECKIN_CHECKOUT	_T("Check In/Check Out")
#define	P4_TABBEDOPTIONS_OPEN_CLOSE					_T("Open/Close")