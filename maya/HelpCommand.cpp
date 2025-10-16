/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "HelpCommand.h"

MStatus HelpCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowHelp();
    return MS::kSuccess;
}

void* HelpCommand::creator() {
    return new HelpCommand;
}