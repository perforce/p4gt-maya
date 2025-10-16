/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "AddCommand.h"

MStatus AddCommand::doIt( const MArgList& args ) 
{
	perforceOps->Add();
    return MS::kSuccess;
}

void* AddCommand::creator() {
    return new AddCommand;
}