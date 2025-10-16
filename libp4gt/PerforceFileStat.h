/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_PERFORCE_FILE_STAT
#define INC_PERFORCE_FILE_STAT

#include "PerforceAPI.h"
#include "UniStrDict.h"

/*
 * Class holding file status.
 */
class PerforceFileStat {

public:

	/*
	 * Constructor.
	 */
				PerforceFileStat();

	/*
	 * Destructor.
	 */
				~PerforceFileStat();

	/*
	 * Set new file properties.
	 *
	 * properties - results of p4 fstat
	 */
	void		Set( UniStrDict *properties );

	/*
	 * Return true if file is checked out.
	 */
	bool		IsOpen();

	/*
	 * Return true if file is locked by us.
	 */
	bool		IsOurLocked();

	/*
	 * Return true if file is locked by someone else.
	 */
	bool		IsOtherLocked();

	/*
	 * Return true if file is opened by someone else.
	 */
	bool		IsOtherOpened();

	/*
	 * Return true if file is exclusively locked by someone else.
	 */
	bool		IsOtherExclLocked();

	/*
	 * Return the user@client of the person who has this file locked.
	 */
	StrPtr		*GetOtherLockedUserClient();

	/*
	 * Return true if file is open for add.
	 */
	bool		IsOpenForAdd();

	/*
	 * Return true if the version of the file on the client is the latest version.
	 */
	bool		IsLatestVersion();

	/*
	 * Return client path of file.
	 */
	StrPtr		*ClientPath();

	/*
	 * Return depot path of file.
	 */
	StrPtr		*DepotPath();

	/*
	 * Get head revision number of file.
	 */
	StrPtr		*HeadRevision();

	/*
	 * Get have revision number of file.
	 */
	StrPtr		*HaveRevision();

	/*
	 * Get file type.
	 */
	StrPtr		*FileType();

	/*
	 * Get head action.
	 */
	StrPtr		*HeadAction();

	/*
	 * Get head change number.
	 */
	StrPtr		*HeadChange();

	/*
	 * Get file action.
	 */
	StrPtr		*Action();

	/*
	 * Get change number.
	 */
	StrPtr		*Change();

	/*
	 * Get last modified time as a formatted string.
	 */
	StrPtr		*ModifiedTime();

	/*
	 * Get a list of other users who have this file opened.
	 */
	StrPtr		*OtherOpened();

	/*
	 * Return who has this file locked.
	 */
	StrPtr		*LockedBy();

private:

	/*
	 * Clear all allocated memory/objects
	 */
	void		Clear();

	UniStrDict	*properties;
	UniStrBuf	*timeBuff;
	UniStrBuf	*otherOpened;
	UniStrBuf	*lockedBy;
};

#endif