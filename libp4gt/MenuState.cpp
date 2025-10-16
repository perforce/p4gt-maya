/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "MenuState.h"

#ifndef LANG_ja
#include "PluginNewMessages.h"
#else
#include "PluginNewMessages_ja.h"
#endif

MenuState::MenuState( BOOL online, PerforceFileStat *stat, bool exists )
{
	lockUnlockString = P4_MENUSTATE_LOCK_UNLOCK;
	undoString = P4_MENUSTATE_UNDO_ADD_CHECKOUT;
	enableDisableString = P4_MENUSTATE_DISC;
	resultsOption = true;
	checkedOutOption = true;
	infoOption = true;
	openOption = true;

	if ( !online ) {
		addOption = false;
		checkOutOption = false;
		checkInOption = false;
		documentStatusOption = false;
		documentHistoryOption = false;
		lockUnlockOption = false;
		undoOption = false;
		resultsOption = false;
		checkedOutOption = false;
		infoOption = false;
		openOption = false;
		enableDisableString = P4_MENUSTATE_CONN;
	}
	else if ( stat == NULL ) {
		addOption = false;
		checkOutOption = false;
		checkInOption = false;
		documentStatusOption = false;
		documentHistoryOption = false;
		lockUnlockOption = false;
		undoOption = false;
	}
	else {
		addOption = exists ? false : true;
		checkOutOption = !exists || stat->IsOpen() ? false : true;
		checkInOption = !exists || !stat->IsOpen() ? false : true;
		documentStatusOption = !exists ? false : true;
		documentHistoryOption = !exists ? false : true;

		if ( !exists || !stat->IsOpen() ) {
			lockUnlockOption = false;
			lockUnlockString = P4_MENUSTATE_UNLOCK;
		}
		else {
			lockUnlockOption = true;
			if ( stat->IsOurLocked() ) {
				lockUnlockString = P4_MENUSTATE_UNLOCK;
			}
			else {
				lockUnlockString = P4_MENUSTATE_LOCK;
			}
		}

		if ( !exists ) {
			undoOption = false;
			undoString = P4_MENUSTATE_UNDO_ADD_CHECKOUT;
		}
		else {
			if ( stat->IsOpenForAdd() ) {
				undoOption = true;
				undoString = P4_MENUSTATE_UNDO_ADD;
			}
			else {
				if ( stat->IsOpen() ) {
					undoOption = true;
				}
				else {
					undoOption = false;
				}
				undoString = P4_MENUSTATE_UNDO_CHECKOUT;
			}
		}
	}
}

bool MenuState::AddEnabled()
{
	return addOption;
}

bool MenuState::CheckOutEnabled()
{
	return checkOutOption;
}

bool MenuState::CheckInEnabled()
{
	return checkInOption;
}

bool MenuState::DocumentStatusEnabled()
{
	return documentStatusOption;
}

bool MenuState::DocumentHistoryEnabled()
{
	return documentHistoryOption;
}

bool MenuState::LockUnlockEnabled()
{
	return lockUnlockOption;
}

bool MenuState::UndoEnabled()
{
	return undoOption;
}
bool MenuState::ResultsEnabled()
{
	return resultsOption;
}

bool MenuState::CheckedOutEnabled()
{
	return checkedOutOption;
}

bool MenuState::InfoEnabled()
{
	return infoOption;
}

bool MenuState::OpenEnabled()
{
	return openOption;
}

TCHAR *MenuState::LockUnlockLabel()
{
	return lockUnlockString;
}

TCHAR *MenuState::UndoLabel()
{
	return undoString;
}

TCHAR *MenuState::EnableDisableLabel()
{
	return enableDisableString;
}