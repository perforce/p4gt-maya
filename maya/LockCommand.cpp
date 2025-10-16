/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "LockCommand.h"

MStatus LockCommand::doIt( const MArgList& args ) 
{
	perforceOps->ToggleLock();
    return MS::kSuccess;
}

void* LockCommand::creator() {
    return new LockCommand;
}