/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MFileIO.h>

#include "P4GTMaya.h"
#include "CallbackManager.h"

CallbackManager::CallbackManager()
{
	MStatus status;
	beforeNewId = MSceneMessage::addCallback(MSceneMessage::kBeforeNew, promptCloseDocument, NULL, &status);
	beforeOpenId = MSceneMessage::addCallback(MSceneMessage::kBeforeOpen, promptCloseDocument, NULL, &status);
	mayaExitingId = MSceneMessage::addCallback(MSceneMessage::kMayaExiting, promptCloseDocument, NULL, &status);
	afterOpenId = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, promptOpenDocument, NULL, &status);
}

CallbackManager::~CallbackManager()
{
	MMessage::removeCallback(beforeNewId);
	MMessage::removeCallback(beforeOpenId);
	MMessage::removeCallback(mayaExitingId);
	MMessage::removeCallback(afterOpenId);
}

void CallbackManager::promptCloseDocument( void *data )
{
	// Don't do it if Maya is running in batch mode i.e with no UI
	if ( perforceOps->GetHost()->GetWin() != NULL ) {
		perforceOps->FileCloseEvent();
	}
}

void CallbackManager::promptOpenDocument( void *data )
{
	// Don't do it if Maya is running in batch mode i.e with no UI
	if ( perforceOps->GetHost()->GetWin() != NULL ) {
		MStringArray names;
		MFileIO::getReferences( names );
		UniStrBuf buff;
		if ( names.length() > 0 ) {
			StrBufArray files;
			for ( unsigned i=0; i<names.length(); i++ ) {
				Translate::ToWinChar( names[i].asChar(), &buff );
				files.Put( buff.Text() );
			}
			perforceOps->SyncReferences( &files );
		}
		perforceOps->FileOpenEvent();
	}
}
