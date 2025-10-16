/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_MENU_STATE
#define INC_MENU_STATE

#include <windows.h>
#include <tchar.h>

#include "PerforceFileStat.h"

/*
 * Class to work out the state of various menu options
 */
class MenuState {

public:

	/*
	 * Constructor.
	 */
						MenuState( BOOL online, PerforceFileStat *stat, bool exists );

	bool				AddEnabled();
	bool				CheckOutEnabled();
	bool				CheckInEnabled();
	bool				DocumentStatusEnabled();
	bool				DocumentHistoryEnabled();
	bool				LockUnlockEnabled();
	bool				UndoEnabled();
	bool				ResultsEnabled();
	bool				CheckedOutEnabled();
	bool				InfoEnabled();
	bool				OpenEnabled();

	TCHAR				*LockUnlockLabel();
	TCHAR				*UndoLabel();
	TCHAR				*EnableDisableLabel();

private:


	bool				addOption;
	bool				checkOutOption;
	bool				checkInOption;
	bool				documentStatusOption;
	bool				documentHistoryOption;
	bool				lockUnlockOption;
	bool				undoOption;
	bool				resultsOption;
	bool				checkedOutOption;
	bool				infoOption;
	bool				openOption;

	TCHAR				*lockUnlockString;
	TCHAR				*undoString;
	TCHAR				*enableDisableString;
};

#endif
