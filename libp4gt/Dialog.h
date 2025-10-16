/*
 * Copyright 2004-2022 Perforce Software.  All rights reserved.
 *
 * Original by Data Shades Ltd.
 */

#ifndef INC_DIALOG
#define INC_DIALOG

#include <windows.h>
#include <commctrl.h>
#include <wchar.h>

#include "libp4gt_rc.h"
#include "PerforceAPI.h"
#include "PluginOperations.h"
#include "UniStrBuf.h"

#define UNILEN(b)				((b)->Length() / sizeof( TCHAR ))
//#define UNISET(b, s)			(b)->Set(s, _tcslen(s) * sizeof(TCHAR)); (b)->Terminate()		
//#define UNISETN(b, s, l)		(b)->Set(s, l); (b)->Terminate()
//#define UNIAPPEND(b, s)			(b)->Append(s, _tcslen(s) * sizeof(TCHAR)); (b)->Terminate()

/*
 * Dialog - base class for dialog box classes.
 */

class Dialog {

public:

	/*
	 * Constructor
	 *
	 * ops - PluginOperations object.
	 * id - dialog resource ID.
	 * proc - dialog procedure.
	 */

							Dialog( PluginOperations *ops, int id, DLGPROC proc ) {this->ops = ops, resourceId = id; dialogProc = proc; };


	/*
	 * Show dialog box
	 *
	 * instance - Instance of DLL.
	 * parent - Handle of dialog parent window.
	 */
	int						Show( HINSTANCE instance, HWND parent );

	/*
	 * Get instance of this DLL
	 */
	HINSTANCE				Instance();

	/*
	 * Set dialog caption.
	 */
	void					SetCaption(const WCHAR *text) {caption.Set( text );}

	/*
	 * Get dialog caption
	 */
	StrPtr					*Caption() { return &caption; };

protected:

    /*
     * Initialise caption if set
     */
    void                    InitCaption( HWND dialog );

	/*
	 * Centre dialog within parent window
	 *
	 * dialog - Handle to dialog window
	 */
	static void				Centre( HWND dialog );

	/*
	 * Get rectangle for corner resize grip
	 *
	 * dialog - Handle to dialog window.
	 * rect - rectangle returned in this.
	 */
	static void				GetSizeGripRect( HWND dialog, LPRECT rect );

	/*
	 * Check if cursor over corner grip
	 *
	 * dialog - Handle to dialog window
	 * lParam - Contains x,y location of cursor
	 */ 
	static BOOL				HitTestSizeGrip( HWND dialog, LPARAM lParam );

	/*
	 * Paint corner resize grip
	 * 
	 * dialog - The dialog on which to paint the grip
	 *
	 */
	void					PaintSizeGrip( HWND dialog );

	/*
	 * Save initial size of dialog and all controls
	 *
	 * dialog - Handle to dialog window
	 */
	static void				SaveInitialSizes( HWND dialog );

	/*
	 * Set flags to say how a control should be resized
	 *
	 * control - Handle for control
	 * flags - Flags indicating how control is to be resized
	 */
	static void				SetResizeFlags( HWND control, int flags );

	/*
	 * Resize dialog controls
	 *
	 * dialog - Handle to dialog window
	 * lParam - Contains new size of dialog
	 */
	static void				Resize( HWND dialog, LPARAM lParam );

	/*
	 * Sets minimum size of dialog
	 *
	 * dialog - Handle to dialog window
	 * info	- Structure in which to set minimum size info
	 */
	static void				SetMinimumSize( HWND dialog, LPMINMAXINFO info );

	/*
	 * Get dimensions of dialog control
	 *
	 * dialog - Handle to dialog window
	 * control - Handle to control
	 * rect - Returns dimensions in here
	 */
	static void				GetControlPos( HWND dialog, HWND control, RECT *rect );

	/*
	 * Move Y position of dialog control
	 *
	 * dialog - Handle to dialog window
	 * control - Handle to control
	 * y - New Y position of control
	 */
	static void				MoveControlY( HWND dialog, HWND control, int y );

	/* 
	 * Dialog finish handling
	 *
	 * dialog - Handle to dialog window
	 * result - Result to return
	 */
	static void				FinishDialog( HWND dialog, BOOLEAN result );

	/*
	 * Save the object for this dialog
	 *
	 * dialog - Handle to dialog window
	 * obj - object to save
	 */
	static void				SaveDialogObject( HWND dialog, void *obj );

	/*
	 * Get the object for this dialog
	 *
	 * dialog - Handle to dialog window
	 */
	static void				*DialogObject( HWND dialog );

	/*
	 * Replace newlines \n with a new line seperator such as \r\n. Returned buffer
	 * must be free'ed by caller
	 * 
	 * text - text with \n newlines
	 * sep - the new seperator string
	 *
	 */
	static WCHAR			*ReplaceNewlines( StrPtr *text, WCHAR *sep );

	/* 
	 * Extract the filename part from the full path
	 *
	 * path - The full path
	 */
	static WCHAR			*ExtractFilename( StrPtr *path );

	/*
	 * Setup system image lists for list view control
	 *
	 * list - Handle of list control.
	 */
	static void				SetListViewImageLists( HWND list );

	/*
	 * Get the index for a folder icon in the system image list.
	 */
	static int				FolderIcon();

	/*
	 * Get the index for a file icon in the system image list.
	 *
	 * name - Name of the file.
	 */
	static int				FileIcon( StrPtr *name );


	/*
	 * Handle dialog box error. Save message and close dialog
	 *
	 * dialog - Handle of dialog window
	 * msg - Error message
	 */
	void					SetError( HWND dialog, exception *e );

	/*
	 * Plugin operations object
	 */
	PluginOperations		*ops;

private:
	
	/*
	 * Save size of dialog or control
	 *
	 * hWnd - Handle to dialog or control
	 * rect - Contains dialog or control size
	 */
	static void				StoreWindowRect( HWND hWnd, LPRECT rect );

	/* 
	 * Delete saved dialog or control size
	 *
	 * hWnd - Handle to dialog or control
	 */
	static void				DeleteWindowRect( HWND hWnd ); 

	/*
	 * Remove saved initial size from dialog and controls
	 *
	 * dialog - Handle to dialog 
	 */
	static void				RemoveInitialSizes( HWND dialog );

	/* 
	 * dialog resource ID 
	 */
	int						resourceId;

	/* 
	 * dialog procedure 
	 */
	DLGPROC					dialogProc;

	/*
	 * Instance handle for this DLL
	 */
	HINSTANCE				instance;

	StrBuf					errMsg;

	UniStrBuf				caption;	// Allows it to be changed
};

/*
 * Flags to indicate how dialog controls should be resized
 */
#define RESIZE_TOP			0x0001
#define RESIZE_LEFT			0x0002
#define RESIZE_WIDTH		0x0004
#define RESIZE_HEIGHT		0x0008

#define RESIZE_TOP_HALF		0x0010
#define RESIZE_LEFT_HALF	0x0020
#define RESIZE_WIDTH_HALF	0x0040
#define RESIZE_HEIGHT_HALF	0x0080

#define RESIZE_TOP_THIRD	0x0100
#define RESIZE_LEFT_THIRD	0x0200
#define RESIZE_WIDTH_THIRD	0x0400
#define RESIZE_HEIGHT_THIRD	0x0800

#define RESIZE_TOP_TWOTHIRD	0x1000

#endif
