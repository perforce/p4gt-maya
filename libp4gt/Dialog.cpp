/*
 * Copyright 2004-2022 Perforce Software.  All rights reserved.
 *
 * Original by Data Shades Ltd.
 */

#include "Dialog.h"
#include "debug.h"

int Dialog::Show( HINSTANCE instance, HWND parent ) 
{
	this->instance = instance;
	// Casting to LPARAM is ok. 
#pragma warning(push)
#pragma warning(disable: 4244)
	int ret = DialogBoxParam( instance, 
		                      MAKEINTRESOURCE( resourceId ),
						      parent,
						      dialogProc,
						      ( LPARAM )this );
#pragma warning(pop)

	// Check if we have an error
	if ( errMsg.Length() > 0 ) {
		throw std::exception( errMsg.Text() );
	}

	return ret;
}

HINSTANCE Dialog::Instance() 
{
	return instance;
}

void Dialog::SetError( HWND dialog, exception *e ) 
{
	errMsg.Set( e->what() );
	// Got error so exit dialog
	EndDialog( dialog, FALSE );
}

void Dialog::SaveDialogObject( HWND dialog, void *obj ) 
{
	SetWindowLongPtr( dialog, (int) (GWLP_USERDATA), (LONG_PTR)obj ); 
}

void *Dialog::DialogObject( HWND dialog ) 
{
	return ( void * )GetWindowLongPtr( dialog, GWLP_USERDATA );	
}

void Dialog::GetSizeGripRect( HWND dialog, LPRECT rect )
{
  GetClientRect( dialog, rect );
  rect->left = rect->right - ::GetSystemMetrics(SM_CXHSCROLL);
  rect->top = rect->bottom - ::GetSystemMetrics(SM_CYVSCROLL);
}

void Dialog::InitCaption( HWND dialog )
{
    if ( caption.Length() ) {
        SetWindowText( dialog, caption.Text() );
	}
}

void Dialog::PaintSizeGrip( HWND dialog ) 
{
	PAINTSTRUCT paint;
	HDC hdc = BeginPaint( dialog, &paint ); 

	RECT rect;
	// For 3ds max draw a bitmap to get the 3ds max grip style 
	// For Softimage draw a bitmap with the Softimage colour scheme
	if ( _tcscmp( ops->GetHost()->GetName(), _T("P4GT-3dsmax") ) == 0 ||
	     _tcscmp( ops->GetHost()->GetName(), _T("P4GT-Softimage") ) == 0 ) {
		int resource = IDB_MAX_GRIP;
		if ( _tcscmp( ops->GetHost()->GetName(), _T("P4GT-Softimage") ) == 0 ) {
			resource = IDB_SOFT_GRIP;
		}
		GetClientRect( dialog, &rect );
		HBITMAP bitmap = LoadBitmap( instance, MAKEINTRESOURCE( resource ) ) ;
		HDC mem = CreateCompatibleDC( hdc );
		HGDIOBJ old = SelectObject( mem, bitmap ); 
		BitBlt( hdc, rect.right - 11, rect.bottom - 11, 11, 11, mem, 0, 0, SRCCOPY ); 
		SelectObject( mem, old );
		DeleteObject( bitmap );
	}
	else {
		GetSizeGripRect( dialog, &rect );
		DrawFrameControl( hdc, &rect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP );
	}

	EndPaint( dialog, &paint );
}

BOOL Dialog::HitTestSizeGrip( HWND dialog, LPARAM lParam ) 
{
  POINT point;
  point.x = LOWORD(lParam);
  point.y = HIWORD(lParam);
  RECT rect;
  GetSizeGripRect( dialog, &rect );
  // The point to be tested is in screen coordinates
  ClientToScreen( dialog, (LPPOINT)&rect.left );
  ClientToScreen( dialog, (LPPOINT)&rect.right );
  // When in the size grip, pretend to be a portion of the sizing border
  if ( PtInRect( &rect, point ) ) {
	  SetWindowLongPtr( dialog, (int) DWLP_MSGRESULT, HTBOTTOMRIGHT );
	  return TRUE;
  }
  else {
	  return FALSE;
  }
}

void Dialog::Centre( HWND dialog ) 
{
       int       cxWin,         // Delta X for parent window
                 cyWin,         // Delta Y for parent window
                 xDlg,          // New X coordinate for dialog box
                 yDlg,          // New Y coordinate for dialog box
                 cxDlg,         // Delta X for dialog box
                 cyDlg,         // Delta Y for dialog box
                 cxScreen,      // Delta X for screen
                 cyScreen;      // Delta Y for screen
       RECT      rcDlg,         // Rectangle for dialog box
                 rcPrnt;        // Rectangle for parent window
       HWND      hWndParent;    // Handle the parent window

       // Determine the width and height of the screen.
       cxScreen = GetSystemMetrics( SM_CXSCREEN );
       cyScreen = GetSystemMetrics( SM_CYSCREEN );

       // Determine rectangle for parent window of dialog box.
       hWndParent = ( HWND )GetWindowLongPtr( dialog, GWLP_HWNDPARENT );
       if( IsWindow( hWndParent ) ) {
            // Use parent coordinates (rect) to determine where the
            // dialog needs to be on the Windows desktop
            GetWindowRect( hWndParent, &rcPrnt );
            cxWin = rcPrnt.right - rcPrnt.left;
            cyWin = rcPrnt.bottom - rcPrnt.top;
       }
       else {
            // We were unable to determine who the parent is,
            // therefore centering the dialog is pointless!  Place
            // it in the upper left corner of the Windows desktop.
            cxWin = 0;     // Force the centering calculation to
            cyWin = 0;     // think that the dialog is centered on
                           // the Windows desktop
			rcPrnt.left = 0;
			rcPrnt.top = 0;
       }

       // Determine rectangle for dialog box itself.
       GetWindowRect( dialog, &rcDlg );
       cxDlg = rcDlg.right - rcDlg.left;
       cyDlg = rcDlg.bottom - rcDlg.top;

       // Determine new dialog X and Y coordinate
       xDlg = ( cxWin - cxDlg ) / 2;
       xDlg += rcPrnt.left;
       yDlg = ( cyWin - cyDlg ) / 2;
       yDlg += rcPrnt.top;

       // Center the dialog using new coordinates
       if ( ( xDlg < 0 ) || ( yDlg < 0 ) || ( xDlg+cxDlg > cxScreen ) ||
            ( yDlg+cyDlg > cyScreen ) ) {
            // If by centering the dialog on top of its parent it
            // would not be 100% visible, then center it on the
            // Windows desktop instead.
            xDlg = ( cxScreen - cxDlg ) / 2;
            yDlg = ( cyScreen - cyDlg ) / 2;
       }

       // Move the dialog box to its new home on the desktop
       MoveWindow( dialog, xDlg, yDlg, cxDlg, cyDlg, TRUE );
}

void Dialog::StoreWindowRect( HWND hWnd, LPRECT rect ) 
{
	// The cast from a LONG to a HANDLE is OK.
#pragma warning(push)
#pragma warning(disable: 4312)
	SetProp( hWnd, _T("top"), (HANDLE)rect->top );
	SetProp( hWnd, _T("bottom"), (HANDLE)rect->bottom );
	SetProp( hWnd, _T("left"), (HANDLE)rect->left );
	SetProp( hWnd, _T("right"), (HANDLE)rect->right );
#pragma warning(pop)
}

void Dialog::DeleteWindowRect( HWND hWnd ) 
{
	RemoveProp( hWnd, _T("top") );
	RemoveProp( hWnd, _T("bottom") );
	RemoveProp( hWnd, _T("left") );
	RemoveProp( hWnd, _T("right") );
}

void Dialog::GetControlPos( HWND dialog, HWND control, RECT *rect )
{
	GetWindowRect( control, rect );
	ScreenToClient( dialog, (LPPOINT)&rect->left );
	ScreenToClient( dialog, (LPPOINT)&rect->right );
}

void Dialog::MoveControlY( HWND dialog, HWND control, int y )
{
	RECT rect;
	GetControlPos( dialog, control, &rect );
	MoveWindow( control, rect.left, y, rect.right - rect.left, rect.bottom - rect.top, FALSE );
}

void Dialog::SaveInitialSizes( HWND dialog ) 
{
	// Store initial dialog client size
	RECT rect;
	GetClientRect( dialog, &rect );
	StoreWindowRect( dialog, &rect );

	// Store minimum size
	GetWindowRect(dialog, &rect );
	// The cast from a LONG to a HANDLE is OK.
#pragma warning(push)
#pragma warning(disable: 4312)
    SetProp( dialog, _T("width"), (HANDLE)( rect.right - rect.left ) );
    SetProp( dialog, _T("height"), (HANDLE)( rect.bottom - rect.top ) );
#pragma warning(pop)

	// Store sizes of all child controls
	HWND control = GetWindow( dialog, GW_CHILD );
	while ( control ) {
		//GetWindowRect( control, &rect );
		//ScreenToClient( dialog, (LPPOINT)&rect.left );
		//ScreenToClient( dialog, (LPPOINT)&rect.right );
		GetControlPos( dialog, control, &rect );
		StoreWindowRect( control, &rect );
		control = GetWindow( control, GW_HWNDNEXT );
	}
}

void Dialog::RemoveInitialSizes( HWND dialog )
{
	DeleteWindowRect( dialog );
	RemoveProp( dialog, _T("width") );
	RemoveProp( dialog, _T("height") );

	HWND control = GetWindow( dialog, GW_CHILD );
	while ( control ) {
		DeleteWindowRect( control );
		RemoveProp( control, _T("resize") );
		control = GetWindow( control, GW_HWNDNEXT );
	}
}

void Dialog::SetResizeFlags( HWND control, int flags ) 
{
	// The cast from a LONG to a HANDLE is OK.
#pragma warning(push)
#pragma warning(disable: 4312)
	SetProp( control, _T("resize"), (HANDLE)flags );
#pragma warning(pop)
}

void Dialog::Resize( HWND dialog, LPARAM lParam ) 
{
	// Casting and truncation of data ok. 
#pragma warning(push)
#pragma warning(disable: 4311 4302)
	int orgDialogWidth = (int)GetProp( dialog, _T("right") );
	int orgDialogHeight = (int)GetProp( dialog, _T("bottom") );
#pragma warning(pop)
	int newDialogWidth = LOWORD(lParam);
	int newDialogHeight = HIWORD(lParam);

	// Store sizes of all child controls
	HWND control = GetWindow( dialog, GW_CHILD );
	while ( control ) {
		if ( GetProp( control, _T("resize") ) != NULL ) {
			// Casting and truncation of data ok. 
#pragma warning(push)
#pragma warning(disable: 4311 4302)
			int flags = (int)GetProp( control, _T("resize") );
			int top = (int)GetProp( control, _T("top") );
			int left = (int)GetProp( control, _T("left") );
			int width = (int)GetProp( control, _T("right") ) - left;
			int height = (int)GetProp(control, _T("bottom") ) - top;
#pragma warning(pop)

			if ( flags & RESIZE_TOP ) {
				top += newDialogHeight - orgDialogHeight;
			}
			if ( flags & RESIZE_TOP_HALF ) {
				top += ( newDialogHeight - orgDialogHeight ) / 2;
			}
			if ( flags & RESIZE_TOP_THIRD ) {
				top += ( newDialogHeight - orgDialogHeight ) / 3;
			}
			if ( flags & RESIZE_TOP_TWOTHIRD ) {
				top += ( ( newDialogHeight - orgDialogHeight ) / 3 ) * 2;
			}
			if ( flags & RESIZE_LEFT ) {
				left += newDialogWidth - orgDialogWidth ;
			}
			if ( flags & RESIZE_LEFT_HALF ) {
				left += ( newDialogWidth - orgDialogWidth ) / 2;
			}
			if ( flags & RESIZE_LEFT_THIRD ) {
				left += ( newDialogWidth - orgDialogWidth ) / 3;
			}
			if ( flags & RESIZE_WIDTH ) {
				width += newDialogWidth - orgDialogWidth;
			}
			if ( flags & RESIZE_WIDTH_HALF ) {
				width += ( newDialogWidth - orgDialogWidth ) / 2;
			}
			if ( flags & RESIZE_WIDTH_THIRD ) {
				width += ( newDialogWidth - orgDialogWidth ) / 3;
			}
			if ( flags & RESIZE_HEIGHT ) {
				height += newDialogHeight - orgDialogHeight;
			}
			if ( flags & RESIZE_HEIGHT_HALF ) {
				height += ( newDialogHeight - orgDialogHeight ) / 2;
			}
			if ( flags & RESIZE_HEIGHT_THIRD ) {
				height += ( newDialogHeight - orgDialogHeight ) / 3;
			}

			MoveWindow( control, left, top, width, height, TRUE );
		}

		control = GetWindow( control, GW_HWNDNEXT );
	}
	//InvalidateRect( dialog, NULL, FALSE );
	InvalidateRect( dialog, NULL, TRUE );
}

void Dialog::SetMinimumSize( HWND dialog, LPMINMAXINFO info ) 
{
	// Casting and truncation of data ok. 
#pragma warning(push)
#pragma warning(disable: 4311 4302)
	int width = (int)GetProp( dialog, _T("width") );
	int height = (int)GetProp( dialog, _T("height") );
#pragma warning(pop)
	info->ptMinTrackSize.x = width;
	info->ptMinTrackSize.y = height;
}

void Dialog::FinishDialog( HWND dialog, BOOLEAN result )
{
	RemoveInitialSizes( dialog );
	EndDialog( dialog, result );
}

TCHAR *Dialog::ExtractFilename( StrPtr *path ) 
{
	TCHAR *name = _tcsrchr( (TCHAR *)path->Text(), '/' );
	return _tcsinc( name );
}

TCHAR *Dialog::ReplaceNewlines( StrPtr *text, TCHAR *sep ) 
{
	size_t sepLen = _tcslen( sep );
	// Allocate the maximum possible buffer size. 
	TCHAR *buff = new TCHAR[ ( text->Length() * sepLen ) + 1 ];
	size_t idx = 0;
	for ( TCHAR *src = (TCHAR *)text->Text(); *src != '\0'; src++ ) {
		if ( *src == '\n' ) {
			_tcscpy( buff + idx, sep );
			idx += sepLen;
		}
		else {
			buff[idx++] = *src;
		}
	}
	buff[idx] = '\0';
	return buff;
}

void Dialog::SetListViewImageLists( HWND list ) 
{
	SHFILEINFO  info;
	TCHAR       dir[MAX_PATH];
	HIMAGELIST  smallIcons;
	HIMAGELIST  largeIcons;

	GetWindowsDirectory( dir, MAX_PATH );

	// Get small system image list
	smallIcons = ( HIMAGELIST )SHGetFileInfo( dir, 0, &info, sizeof( info ), 
										 SHGFI_SMALLICON | SHGFI_SYSICONINDEX);
	DestroyIcon( info.hIcon ); // Don't need icon

	// Get large system image list
	largeIcons = ( HIMAGELIST )SHGetFileInfo( dir, 0, &info, sizeof( info ), 
										 SHGFI_ICON | SHGFI_SYSICONINDEX);
	DestroyIcon( info.hIcon ); // Don't need icon

	ListView_SetImageList( list, largeIcons, LVSIL_NORMAL );
	ListView_SetImageList( list, smallIcons, LVSIL_SMALL );
}

int Dialog::FolderIcon() 
{
	TCHAR dir[MAX_PATH];
	GetWindowsDirectory( dir, MAX_PATH );

	SHFILEINFO info;
	SHGetFileInfo( dir, FILE_ATTRIBUTE_DIRECTORY, &info, sizeof( SHFILEINFO ),
		           SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON );
	// Do not need the icon only the index
	DestroyIcon( info.hIcon );
	return info.iIcon;
}

int Dialog::FileIcon( StrPtr *name ) 
{
	SHFILEINFO info;
	SHGetFileInfo( (TCHAR *)name->Text(), FILE_ATTRIBUTE_NORMAL, &info, sizeof( SHFILEINFO ),
		          SHGFI_USEFILEATTRIBUTES | SHGFI_ICON | SHGFI_SMALLICON );
	// Do not need the icon only the index
	DestroyIcon( info.hIcon );
	return info.iIcon;
}
