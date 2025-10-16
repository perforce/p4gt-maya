/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "CheckOutCommand.h"

MStatus CheckOutCommand::doIt( const MArgList& args ) 
{
	perforceOps->Edit();
    return MS::kSuccess;
}

void* CheckOutCommand::creator() {
    return new CheckOutCommand;
}