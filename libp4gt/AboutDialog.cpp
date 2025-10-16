/*
 * Copyright 2004 - 2022 Perforce Software, Inc.  All rights reserved.
 *
 * Originally developed by Data Shades Ltd.
 */
#include "AboutDialog.h"

#define BUFSIZE		100

AboutDialog::AboutDialog( PluginOperations *ops, WCHAR *name, WCHAR *pluginVer, WCHAR *appVer ) : Dialog( ops, IDD_ABOUT_DIALOG, DialogWndProc ) 
{
	pluginName = name;
	pluginVersion = pluginVer;
	appVersion = appVer;
}

// Starting with Windows 8, GetVersionEx no longer works.
// Microsoft doesn't want app developers depending on the Windows
// version for app compatibility, so they make it difficult to
// get the same information we see in the Settings > System > About.
// We need that information for QA and customers to tell us which
// Windows version they were using when they found a bug, or testing.
// Instead, we can read the OS info from the registry.
// Example final buff:
// Windows version: Windows 10 Enterprise Build 19044.1466
bool AboutDialog::WindowsVersion( UniStrBuf &buff ) 
{
	buff.Append( L"Windows version: " ); // TODO Localize.

	HKEY versionKey;

    WCHAR regKeyName[MAX_PATH] = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";

    // We use KEY_WOW64_64KEY to read the 64-bit version of the key.
    // On Windows 11 (and 10?):
    // The 32-bit version has EditionID of "Enterprise" while
    // the 64-bit version has EditionID of "Professional" and the System > About setting shows "Pro".
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, regKeyName, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &versionKey) == ERROR_SUCCESS) {
		DWORD type;
        WCHAR productName[BUFSIZE];
		productName[0] = '\0';
        WCHAR buildNumber[BUFSIZE];
        WCHAR edition[BUFSIZE];
        DWORD size = BUFSIZE;
		RegQueryValueExW( versionKey, L"CurrentBuildNumber", NULL, &type, ( LPBYTE )buildNumber, &size );
        int ibuildnum = _wtoi(buildNumber);
        if (ibuildnum >= 22000) // Windows 11 (or greater).
		{
			// Windows 11 productName in registry says it's Windows 10.
			StringCchCatW(productName, _countof(productName), L"Windows 11 "); // TODO Localize? Or is "Windows" always "Windows".
            size = BUFSIZE;
		    RegQueryValueExW( versionKey, L"EditionID", NULL, &type, ( LPBYTE )edition, &size );
			StringCchCatW(productName, _countof(productName), edition);  // Enterprise, etc.
		}
		else
		{
            size = BUFSIZE;
			RegQueryValueExW( versionKey, L"ProductName", NULL, &type, ( LPBYTE )productName, &size );
		}
		buff.Append(productName);
		buff.Append(L" Build ");
		buff.Append(buildNumber);
        buff.Append(L".");
        DWORD dwUpdateBuildRevision = 0;
		size = sizeof(DWORD);
		RegQueryValueExW( versionKey, L"UBR", NULL, &type, reinterpret_cast<LPBYTE>( &dwUpdateBuildRevision ), &size );
        WCHAR updateBuildRevision[BUFSIZE];
        _itow_s(dwUpdateBuildRevision, updateBuildRevision, BUFSIZE, 10);
		buff.Append(updateBuildRevision);
		RegCloseKey( versionKey );
	}
	else {
		buff.Append( L"Unknown"); // TODO Localize.
	}

    return true; 
}

INT_PTR CALLBACK AboutDialog::DialogWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{

	switch(uMsg) {

	case WM_INITDIALOG:	{	
		AboutDialog *dialog = ( AboutDialog * )lParam;
		dialog->SaveDialogObject( hWnd, dialog );

		UniStrBuf version;
		UniStrBuf pluginVersion;
		dialog->WindowsVersion( version );

		SetDlgItemText( hWnd, IDC_PLUGIN_NAME_TEXT, dialog->PluginName() );
		pluginVersion.Set( _T("Version: ") );
		pluginVersion.Append( dialog->PluginVersion() );
		//pluginVersion.Append( _T(" (Beta)") );
		SetDlgItemText( hWnd, IDC_PLUGIN_VERSION_TEXT, pluginVersion.Text() );
		SetDlgItemText( hWnd, IDC_APP_VERSION_TEXT, dialog->AppVersion() );
		SetDlgItemText( hWnd, IDC_WIN_VERSION_TEXT, version.Text() );
		Centre( hWnd );
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case IDOK:
			EndDialog( hWnd, TRUE );
			return true;
		case IDCANCEL:
			EndDialog( hWnd, FALSE );
			return true;
		case IDCOPY:
			int cchMax = 2500;
			WCHAR aDlgLine[500];
			WCHAR allDlgLines[2500];

			GetDlgItemText( hWnd, IDC_PLUGIN_NAME_TEXT, aDlgLine, cchMax );
			_tcscpy_s( allDlgLines, cchMax, aDlgLine );
			_tcscat_s( allDlgLines, cchMax, _T("\n") );
			GetDlgItemText( hWnd, IDC_PLUGIN_VERSION_TEXT, aDlgLine, cchMax );
			_tcscat_s( allDlgLines, cchMax, aDlgLine );
			_tcscat_s( allDlgLines, cchMax, _T("\n") );
			GetDlgItemText( hWnd, IDC_APP_VERSION_TEXT, aDlgLine, cchMax );
			_tcscat_s( allDlgLines, cchMax, aDlgLine );
			_tcscat_s( allDlgLines, cchMax, _T("\n") );
			GetDlgItemText( hWnd, IDC_WIN_VERSION_TEXT, aDlgLine, cchMax );
			_tcscat_s( allDlgLines, cchMax, aDlgLine );
			_tcscat_s( allDlgLines, cchMax, _T("\n") );
			GetDlgItemText( hWnd, IDC_COPYRIGHT_TEXT, aDlgLine, cchMax );
			_tcscat_s( allDlgLines, cchMax, aDlgLine );

			if ( OpenClipboard( hWnd ))
			{
				EmptyClipboard();

				HGLOBAL hClipboardData;
				size_t size = (wcslen(allDlgLines) + 1) * sizeof(WCHAR);
				hClipboardData = GlobalAlloc( NULL, size );
				WCHAR* pchData = (WCHAR*)GlobalLock( hClipboardData );
				memcpy( pchData, LPCTSTR(allDlgLines), size );
				SetClipboardData( CF_UNICODETEXT, hClipboardData );
				GlobalUnlock( hClipboardData );

				CloseClipboard();
			}
			return true;
		}
	}

	return false;
}


