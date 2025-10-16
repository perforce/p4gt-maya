/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "InfoCommand.h"

MStatus InfoCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowInfo();
    return MS::kSuccess;
}

void* InfoCommand::creator() {
    return new InfoCommand;
}