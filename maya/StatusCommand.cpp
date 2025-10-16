/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "StatusCommand.h"

MStatus StatusCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowStatus();
    return MS::kSuccess;
}

void* StatusCommand::creator() {
    return new StatusCommand;
}