/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_FILE_OP_DIALOG
#define INC_FILE_OP_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"
#include "PerforceOptions.h"

/*
 * FileOpDialog - dialog box to show result of various file operations
 * such as Add, Edit, Lock etc.
 */

class FileOpDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * op - The operation being carried out "add", "edit", "lock" etc.
	 * filename - The name of the file.
	 * results - The results of the operation
	 * options - Interface to preference store
	 */

							FileOpDialog( PluginOperations *ops, TCHAR *op, TCHAR *filename, PerforceResults *results, PerforceOptions *options );

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


	static void				AppendLine( UniStrBuf *buf, TCHAR *name, StrPtr *value ); 

	/*
	 * Get the name of the file.
	 */
	TCHAR					*Filename() { return filename; };

	/*
	 * Get the file operation
	 */
	TCHAR					*Operator() { return op; };

	/*
	 * Get the results of the operation
	 */
	PerforceResults			*Results() { return results; };

	/*
	 * Get the options interface
	 */
	PerforceOptions			*Options() { return options; };

	TCHAR					*op;
	TCHAR					*filename;
	PerforceResults			*results;
	PerforceOptions			*options;
};

#endif