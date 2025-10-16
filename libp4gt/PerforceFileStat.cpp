/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <time.h>

#include "PerforceFileStat.h"

PerforceFileStat::PerforceFileStat() : properties( NULL ), timeBuff( NULL ), otherOpened( NULL ), lockedBy( NULL )
{
}

PerforceFileStat::~PerforceFileStat()
{
	Clear();
}

void PerforceFileStat::Clear()
{
	if ( properties != NULL ) {
		delete properties;
		properties = NULL;
	}
	if ( timeBuff != NULL ) {
		delete timeBuff;
		timeBuff = NULL;
	}
	if ( otherOpened != NULL ) {
		delete otherOpened;
		otherOpened = NULL;
	}
	if ( lockedBy != NULL ) {
		delete lockedBy;
		lockedBy = NULL;
	}
}

void PerforceFileStat::Set( UniStrDict *props )
{
	Clear();
	properties = new UniStrDict( props );
}

bool PerforceFileStat::IsOpen()
{
	if ( properties->GetVar( "action" ) == NULL ) {
		return false;
	}
	else {
		return true;
	}
}

bool PerforceFileStat::IsOurLocked()
{
	if ( properties->GetVar( "ourLock" ) == NULL ) {
		return false;
	}
	else {
		return true;
	}
}

bool PerforceFileStat::IsOtherLocked()
{
	if ( properties->GetVar( "otherLock" ) == NULL ) {
		return false;
	}
	else {
		return true;
	}
}

bool PerforceFileStat::IsOtherOpened()
{
	if ( properties->GetVar( "otherOpen" ) == NULL ) {
		return false;
	}
	else {
		return true;
	}
}

StrPtr *PerforceFileStat::GetOtherLockedUserClient()
{
	StrBuf key;
	StrPtr *value;
	for ( int i=0;; i++ ) {
		key.Clear();
   		key << "otherLock" << i;
		value = properties->GetVar( key.Text() );
		if ( value != NULL ) {
			return value;
		}
	}
	return NULL;
}

bool PerforceFileStat::IsLatestVersion()
{
	StrPtr *headRev = HeadRevision();
	StrPtr *haveRev = HaveRevision();
	if ( headRev ) {
		if ( !haveRev || _tcscmp( (TCHAR *)headRev->Text(), (TCHAR *)haveRev->Text() ) != 0 ) {
			return false;
		}
	}
	return true;
}

bool PerforceFileStat::IsOpenForAdd()
{
	StrPtr *action = properties->GetVar( "action" );
	if ( action != NULL && _tcscmp( (TCHAR *)action->Text(), _T("add") ) == 0 ) {
		return true;
	}
	else {
		return false;
	}
}

StrPtr *PerforceFileStat::HeadRevision()
{
	return properties->GetVar( "headRev" );
}

StrPtr *PerforceFileStat::HaveRevision()
{
	return properties->GetVar( "haveRev" );
}

StrPtr *PerforceFileStat::ClientPath()
{
	return properties->GetVar( "clientFile" );
}

StrPtr *PerforceFileStat::DepotPath()
{
	return properties->GetVar( "depotFile" );
}

StrPtr *PerforceFileStat::FileType()
{
	return properties->GetVar( "headType" );
}

StrPtr *PerforceFileStat::HeadAction()
{
	return properties->GetVar( "headAction" );
}

StrPtr *PerforceFileStat::HeadChange()
{
	return properties->GetVar( "headChange" );
}

StrPtr *PerforceFileStat::Action()
{
	return properties->GetVar( "action" );
}

StrPtr *PerforceFileStat::Change()
{
	return properties->GetVar( "change" );
}

StrPtr *PerforceFileStat::ModifiedTime()
{
	if ( timeBuff == NULL ) {
		time_t seconds = _ttol( (TCHAR *)properties->GetVar( "headTime" )->Text() );
		struct tm *time = localtime( &seconds );
		timeBuff = new UniStrBuf();
		TCHAR *buff = (TCHAR *)timeBuff->Alloc( 128 );
		// Format time as YY/MM/DD HH:MM:SS
		_stprintf( buff, _T("%04d/%02d/%02d %02d:%02d:%02d"),
				 time->tm_year+1900, time->tm_mon + 1, time->tm_mday,
				 time->tm_hour, time->tm_min, time->tm_sec );
	}
	return timeBuff;
}

StrPtr *PerforceFileStat::LockedBy()
{
	if ( lockedBy == NULL && ( properties->GetVar( "ourLock" ) || properties->GetVar ( "otherLock" ) ) ) {
		lockedBy = new UniStrBuf();
		// If we have the lock.
		if ( properties->GetVar( "ourLock" ) ) {
			lockedBy->Set( _T("you") );
		}
		else {
			StrBuf key;
			StrPtr *value;
			for ( int i=0;; i++ ) {
				key.Clear();
				key << "otherLock" << i;
				value = properties->GetVar( key.Text() );
				if ( value != NULL ) {
					lockedBy->Set( value );
					break;
				}
			}
		}
	}
	return lockedBy;
}

StrPtr *PerforceFileStat::OtherOpened()
{
	if ( otherOpened == NULL && properties->GetVar( "otherOpen" ) != NULL ) {
		otherOpened = new UniStrBuf();
		StrBuf key;
		StrPtr *value;
		for ( int i=0;; i++ ) {
			key.Clear();
			key << "otherOpen" << i;
			value = properties->GetVar( key.Text() );
			if ( value == NULL ) {
				break;
			}
			if (i > 0) {
				otherOpened->Append( _T("\r\n") );
			}
			otherOpened->Append( value );
			key.Clear();
			key << "otherAction" << i;
			value = properties->GetVar( key.Text() );
			otherOpened->Append( _T(" (") );
			otherOpened->Append( value );
			otherOpened->Append( _T(")") );
		}
	}
	return otherOpened;
}

bool PerforceFileStat::IsOtherExclLocked()
{
	if ( IsOtherOpened() ) {
		StrPtr *headType = properties->GetVar( "headType" );
		if ( headType != NULL && _tcsstr( (TCHAR *)headType->Text(), _T("+") ) != NULL ) {
			if ( _tcsstr( (TCHAR *)headType->Text(), _T("l") ) != NULL ) {
				return true;
			}
		}
	}
	return false;
}