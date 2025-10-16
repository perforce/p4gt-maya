/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_RESULTS_DIALOG
#define INC_RESULTS_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"

/*
 * ResultsDialog - dialog box to show result of last command and
 * submit results.
 */

class ResultsDialog : public Dialog {

public:

	/*
	 * Constructor.
	 *
	 * ops - The plugin operations object.
	 * con - The current connection.
	 * op - Mode for this for dialog.
	 * currDir - The current directory.
	 */

							ResultsDialog( PluginOperations *ops, TCHAR *op, PerforceResults *results, const StrPtr *currDir = NULL );

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
	 * Get the results of the last operation
	 */
	PerforceResults			*Results() { return results; };

	/*
	 * Append line to output window message
	 */
	void					AppendLine( UniStrBuf *buff, TCHAR *var, StrPtr *value );

	/*
	 * Get the mode for this dialog
	 */
	TCHAR					*Operator() { return op; };

	/*
	 * Parse output of p4 info
	 */
	void					ParseInfo( PerforceResults &results, UniStrBuf *msg );

	const StrPtr			*GetCwd() { return currDir; };

	PerforceResults			*results;
	TCHAR					*op;
	const StrPtr			*currDir;
};

#endif