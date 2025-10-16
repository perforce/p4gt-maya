/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <windows.h>

#include "PerforceOptions.h"
#include "libp4gt_rc.h"

#include "debug.h"

PerforceOptions::PerforceOptions( TCHAR *name )
{
	this->name = name;

	// Generate full registry path;
	regKeyName.Append( SOFTWARE_KEY );
	regKeyName.Append( _T("\\") );
	regKeyName.Append( PERFORCE_KEY );
	regKeyName.Append( _T("\\") );
	regKeyName.Append( name );
}

bool PerforceOptions::GetOptionString( int option, UniStrBuf *buff )
{
	HKEY optionsKey;
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, regKeyName.Text(), 0, KEY_QUERY_VALUE, &optionsKey ) == ERROR_SUCCESS ) {
		DWORD type;
		TCHAR value[2048];
		DWORD size = sizeof( value );
		long ret = RegQueryValueEx( optionsKey, GetOptionName( option ), NULL, &type, ( LPBYTE )value, &size );
		RegCloseKey( optionsKey );
		// If option exists return value.
		if ( ret == ERROR_SUCCESS ) {
			buff->Set( value );
			return true;
		}
	}
	return false;
}

DWORD PerforceOptions::GetOptionValue( int option )
{
	HKEY optionsKey;
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, regKeyName.Text(), 0, KEY_QUERY_VALUE, &optionsKey ) == ERROR_SUCCESS ) {
		DWORD value, type;
		DWORD size = sizeof( DWORD );
		long ret = RegQueryValueEx( optionsKey, GetOptionName( option ), NULL, &type, ( LPBYTE )&value, &size );
		RegCloseKey( optionsKey );
		// If option exists return value.
		if ( ret == ERROR_SUCCESS ) {
			return value;
		}
	}

	// Value does not exist so return default
	switch ( option ) {
	case SHOW_CONFIRMATION_OPTION:
		return ALWAYS_OPTION;
	case LOCK_ON_CHECK_OUT_OPTION:
	case CHECK_IN_ON_CLOSE_OPTION:
	case CHECK_OUT_ON_OPEN_OPTION:
	case CHECK_IN_ON_ADD_OPTION:
	case SYNC_FILE_REF_OPTION:
	case CHECK_OUT_ON_REOPEN_OPTION:
		return PROMPT_OPTION;
	case OTHER_DOCUMENTS_OPTION:
		return ALWAYS_OPTION;
	case PERFORCE_OPEN_VIEW_TYPE:
		return LVS_REPORT;
	case PERFORCE_OPEN_WIDTH:
	case PERFORCE_OPEN_HEIGHT:
	case PERFORCE_OPEN_FILE_TYPE:
		return 0;
	case SERVER_ONLINE:
		return 1;
	case REVERT_ON_CLOSE_OPTION:
	default:
		return NEVER_OPTION;
	}
}

void PerforceOptions::SetOptionValue( int option, DWORD value )
{
	DWORD exists;
	HKEY softwareKey, perforceKey, optionsKey;

	RegOpenKeyEx( HKEY_CURRENT_USER, SOFTWARE_KEY, 0, KEY_CREATE_SUB_KEY, &softwareKey );
	RegCreateKeyEx( softwareKey, PERFORCE_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &perforceKey, &exists );
	RegCreateKeyEx( perforceKey, name, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &optionsKey, &exists );

	RegSetValueEx( optionsKey, GetOptionName( option ), 0, REG_DWORD, ( CONST BYTE * )&value, sizeof( DWORD ) );
}

void PerforceOptions::SetOptionString( int option, TCHAR *value )
{
	DWORD exists;
	HKEY softwareKey, perforceKey, optionsKey;

	RegOpenKeyEx( HKEY_CURRENT_USER, SOFTWARE_KEY, 0, KEY_CREATE_SUB_KEY, &softwareKey );
	RegCreateKeyEx( softwareKey, PERFORCE_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &perforceKey, &exists );
	RegCreateKeyEx( perforceKey, name, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &optionsKey, &exists );

	// Possible loss of data if size_t larger then MAXINT.
	RegSetValueEx( optionsKey, GetOptionName( option ), 0, REG_SZ, ( CONST BYTE * )value,
		(DWORD)(( _tcslen( value ) + 1 ) * sizeof( TCHAR )) );
}

TCHAR *PerforceOptions::GetOptionName( int option )
{
	switch (option) {
	case CHECK_IN_ON_CLOSE_OPTION:
		return _T("CheckInOnClose");
	case CHECK_OUT_ON_OPEN_OPTION:
		return _T("CheckOutOnOpen");
	case CHECK_OUT_ON_REOPEN_OPTION:
		return _T("CheckOutOnReopen");
	case CHECK_IN_ON_ADD_OPTION:
		return _T("CheckInOnAdd");
	case LOCK_ON_CHECK_OUT_OPTION:
		return _T("LockOnCheckOut");
	case SHOW_CONFIRMATION_OPTION:
		return _T("ShowConfirmationDialogs");
	case OTHER_DOCUMENTS_OPTION:
		return _T("OtherChangelistDocuments");
	case SYNC_FILE_REF_OPTION:
		return _T("UpdateFileReferencesOnOpen");
	case PERFORCE_OPEN_VIEW_TYPE:
		return _T("PerforceOpenViewType");
	case PERFORCE_OPEN_HEIGHT:
		return _T("PerforceOpenHeight");
	case PERFORCE_OPEN_WIDTH:
		return _T("PerforceOpenWidth");
	case PERFORCE_OPEN_DIR:
		return _T("PerforceOpenDir");
	case PERFORCE_OPEN_FILE_TYPE:
		return _T("PerforceOpenFileType");
	case SERVER_ONLINE:
		return _T("ServerOnline");
	case REVERT_ON_CLOSE_OPTION:
		return _T("RevertOnClose");
	default:
		return NULL;
	}
}
