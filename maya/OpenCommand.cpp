/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "OpenCommand.h"

static StrBufArray fileTypes;

/*
 * Setup Maya file types
 */
void InitFileTypes() 
{
	fileTypes.Put( _T("Maya Scene (*.mb;*.ma)") );
	fileTypes.Put( _T("mayaAscii (*.ma)") );
	fileTypes.Put( _T("mayaBinary (*.mb)") );
	fileTypes.Put( _T("mel (*.mel)") );
	fileTypes.Put( _T("OBJ (*.obj)") );
	fileTypes.Put( _T("audio (*.wav;*.aif;*.aiff)") );
	fileTypes.Put( _T("move (*.mov)") );
	fileTypes.Put( _T("EPS (*.eps)") );
	fileTypes.Put( _T("Illustrator (*.ai)") );
	fileTypes.Put( _T("fluidCache (*.mcfi;*.mcfp)") );
	fileTypes.Put( _T("All Files (*.*)") );
}

MStatus OpenCommand::doIt( const MArgList& args ) 
{
	if ( fileTypes.Count() == 0 ) {
		InitFileTypes();
	}
	perforceOps->ShowOpenDepot( &fileTypes );
    return MS::kSuccess;
}

void* OpenCommand::creator() {
    return new OpenCommand;
}