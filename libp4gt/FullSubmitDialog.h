/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_FULL_SUBMIT_DIALOG
#define INC_FULL_SUBMIT_DIALOG

#include "Dialog.h"
#include "PerforceResults.h"
#include "PerforceConnection.h"
#include "UniStrBuf.h"

/*
 * FullSubmitDialog - Show dialog box for submitting files. Includes list
 * of files to submit.
 */
class FullSubmitDialog : public Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - The plugin operations object.
	 * con - The connection to use to get job info
	 * results - Results of running p4 opened command
	 * curr - The depot path of the current file
	 * showOther - Whether or not to display other files in the changelist
	 */
						FullSubmitDialog( PluginOperations *ops, PerforceConnection *con, PerforceResults *results, StrPtr *curr, BOOL showOther );

	/*
	 * Destructor
	 */
						~FullSubmitDialog();
	/*
	 * Set the changelist description.
	 */
	void				SetDescription( TCHAR *desc ) { this->desc = desc; };

	/*
	 * Set the list of checked files
	 *
	 * files - List of files to submit seperated by \n\t
	 */
	void				SetSubmitFiles( StrBuf *files ) { submitFiles = files; };

	/*
	 * Get the list of files to submit
	 */
	StrPtr				*GetSubmitFiles() { return submitFiles; };

	/*
	 * Set the list of checked jobs
	 *
	 * jobs - List of jobs to submit seperated by \n\t
	 */
	void				SetSubmitJobs( StrBuf *jobs ) { submitJobs = jobs; };

	/*
	 * Get the list of jobs to submit
	 */
	StrPtr				*GetSubmitJobs() { return submitJobs; };

	/*
	 * Get the changelist description.
	 *
	 * buff - The buffer to return the description in.
	 */
	void				GetDescription( UniStrBuf &buff ) { buff.Set( desc ); };

	/*
	 * Set flag to say whether or not to keep file(s) checked out
	 *
	 * keep - The flag
	 */
	void				SetKeepOut( BOOL keep ) { this->keep = keep; };

	/*
	 * Return whether or not to keep file(s) checked out
	 */
	BOOL				GetKeepOut() { return keep; };

	/*
	 * Set flag to say whether or not to leave unchanged file(s) in default changelist
	 *
	 * leave - The flag
	 */
	void				SetLeaveUnchanged ( BOOL leave ) { this->leave = leave; };

	/*
	 * Return whether or not to leave unchanged file(s) in default changelist
	 */
	BOOL				GetLeaveUnchanged() { return leave; };

	/*
	 * Return whether or not to show other files in the changelist
	 */
	BOOL				ShowOther() { return showOther; };

	/*
	 * Get results of p4 opened command
	 */
	PerforceResults			*Results() { return results; };

	/*
	 * Get depot path of current file
	 */
	StrPtr					*CurrFile() { return curr; };

	/*
	 * Get the connection
	 */
	PerforceConnection		*GetConnection() { return con; }

	/*
	 * Fill the combo with possible job status values
	 *
	 * combo - Handle of status combo
	 */
	void					FillJobStatus( HWND combo );

	/*
	 * Set flag to say user has selected job status
	 */
	void					SetJobStatus() { jobStatus = TRUE; };

	/*
	 * Return true if user has selected job status
	 */
	BOOL					GetJobStatus() { return jobStatus; };

	/*
	 * Set flag to say user has selected/deselected all files in pending changelist(s)
	 */
	void					SetSelectAll( BOOL selectAll ) { this->selectAll = selectAll; };

	/*
	 * Return flag if user has selected/deselected all files in pending changelist(s)
	 */
	BOOL					GetSelectAll() { return selectAll; };

	/*
	 * Set the submit option.
	 */
	void					SetSubmitOption( TCHAR *submitOption ) { this->submitOption = submitOption; };

	/*
	 * Get the submit option.
	 */
	TCHAR					*GetSubmitOption() { return submitOption; };

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

	TCHAR					*desc;
	PerforceResults			*results;
	StrPtr					*curr;
	StrBuf					*submitFiles;
	StrBuf					*submitJobs;
	BOOL					keep;
	BOOL					leave;
	BOOL					showOther;
	PerforceConnection		*con;
	BOOL					jobStatus;
	BOOL					selectAll;
	TCHAR					*submitOption;
};

#endif
