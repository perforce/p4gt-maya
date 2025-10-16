/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "CheckInCommand.h"

MStatus CheckInCommand::doIt( const MArgList& args ) 
{
	perforceOps->Submit();
    return MS::kSuccess;
}

void* CheckInCommand::creator() {
    return new CheckInCommand;
}