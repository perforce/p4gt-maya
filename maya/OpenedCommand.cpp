/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "OpenedCommand.h"

MStatus OpenedCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowOpenFiles();
    return MS::kSuccess;
}

void* OpenedCommand::creator() {
    return new OpenedCommand;
}
