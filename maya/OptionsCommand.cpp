/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "OptionsCommand.h"

MStatus OptionsCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowOptions();	
	return MS::kSuccess;
}

void* OptionsCommand::creator() {
    return new OptionsCommand;
}