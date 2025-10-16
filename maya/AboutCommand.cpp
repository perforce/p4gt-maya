/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "AboutCommand.h"

MStatus AboutCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowAbout();
    return MS::kSuccess;
}

void* AboutCommand::creator() 
{
    return new AboutCommand;
}