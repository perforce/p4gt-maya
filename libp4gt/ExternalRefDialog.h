/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_EXTERNAL_REF_DIALOG
#define INC_EXTERNAL_REF_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"
#include "StrBufArray.h"

/*
 * ExternalRefDialog - Dialog allowing user to sync external
 * reference files.
 */
class ExternalRefDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
	 * results - Results of doing a p4 fstat on the files
	 */

							ExternalRefDialog( PluginOperations *ops, PerforceResults *results );

	/*
	 * Destructor
	 */
							~ExternalRefDialog();

	/*
	 * Return the results of the p4 fstat
	 */
	PerforceResults			*Results() { return results; };

	/*
	 * Get list of files to sync
	 */
	StrBufArray				*GetSyncFiles() {return syncFiles;};

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

	/*
	 * Set list of files to sync
	 *
	 * syncFiles - Array of files to sync
	 */
	void					SetSyncFiles( StrBufArray *syncFiles ) {this->syncFiles = syncFiles;};

	PerforceResults			*results;
	StrBufArray				*syncFiles;
};

#endif