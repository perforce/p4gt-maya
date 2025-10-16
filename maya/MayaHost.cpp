/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MCommandResult.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>
#include <maya/MFileIO.h>

#include "P4GTMaya.h"
#include "Translate.h"
#include "..\libp4gt\version.h"

#define UNSAVED_TITLE	_T("untitled")

TCHAR *MayaHost::GetName()
{
	return _T("P4GT-Maya");
}

TCHAR *MayaHost::GetDesc()
{
	return _T("P4GT - Maya.");
}

TCHAR *MayaHost::GetVersion()
{
	return _T(P4GT_VERSION);
}

TCHAR *MayaHost::GetHostVersion()
{
	MString version("Maya version: ");
	version += MGlobal::mayaVersion();
	static UniStrBuf buff;
	Translate::ToWinChar( version.asChar(), &buff );
	return buff.Text();
}

int MayaHost::GetNumDocs()
{
	return 1;
}

// Use Maya project directory to find a possible p4config file.
// If the project name ends in untitled, we do not attempt to
// find a p4config file.
//
bool MayaHost::GetCurrDocPath( UniStrBuf &buff )
{
	MString path = MFileIO::currentFile();
#ifdef LANG_ja
	// We will always use unicode in the Japanese plugin.  We
	// need to search for the Japanese word "untitled".  Since
	// it is impossible to guarantee that a Japanese string
	// will not be corrupted in this source, we search for the
	// two unicode hex values which represent the word untitled.
	//
	wchar_t *s = wcsrchr( (wchar_t *)path.asWChar(), L'/' );
	if ( s == NULL )
		s = wcsrchr( (wchar_t *)path.asWChar(), L'\\' );
	if ( s != NULL ) {
		s++;
		if ( s[0] == 0x7121 && s[1] == 0x984c && s[2] == 0 ) {
			return false;
		}
	}
#else
	// Check if path ends in "untitled"
	if ( path.length() >= _tcslen( UNSAVED_TITLE ) ) {
		TCHAR *s = (TCHAR *)path.asWChar() + path.length() - _tcslen( UNSAVED_TITLE );
		if ( _tcscmp( s, UNSAVED_TITLE ) == 0 ) {
			return false;
		}
	}
#endif
	Translate::ToWinChar( path.asChar(), &buff );
	currDocPath = buff;
	return true;
}


bool MayaHost::IsCurrDocDirty()
{
	MCommandResult result;
	MString cmd("file -q -mf");
	MGlobal::executeCommand(cmd, result);
	int intResult;
	result.getResult(intResult);
	return intResult ? true : false;
}


void MayaHost::SaveCurrDoc()
{
	MFileIO::save();
}

void MayaHost::LoadDoc( StrPtr *path )
{
	UniStrBuf mayaPath;
	mayaPath.Set( (TCHAR *)path->Text() );

	// Maya paths only understand forward slashes
	for (TCHAR *curr = mayaPath.Text(); *curr != '\0'; curr++) {
		if (*curr == '\\') {
			*curr = '/';
		}
	}

//	StrBuf buff;
//	Translate::FromWinChar( mayaPath.Text(), &buff );
//	MFileIO::open( buff.Text(), NULL, true );
	MFileIO::open( mayaPath.Text(), NULL, true );
}

void MayaHost::ReloadCurrDoc()
{
	// to prevent a hang in the FileOpen handler when reloading
	// currentFile after a sync we first open a dummy file
	MString untitled("untitled");
	MFileIO::open( untitled, NULL, true );

	MFileIO::newFile(true);
	MFileIO::open( currDocPath.Text(), NULL, true );
}

bool MayaHost::HasMultiDocs()
{
	return false;
}
