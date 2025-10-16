/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "OnlineCommand.h"
#include "P4GTMaya.h"

MStatus OnlineCommand::doIt( const MArgList& args ) 
{
	perforceOps->ToggleOnline();
    return MS::kSuccess;
}

void* OnlineCommand::creator() {
    return new OnlineCommand;
}
