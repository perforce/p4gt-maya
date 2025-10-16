/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "LastResultsCommand.h"

MStatus LastResultsCommand::doIt( const MArgList& args ) 
{
	perforceOps->ShowLastResults();
    return MS::kSuccess;
}

void* LastResultsCommand::creator() 
{
    return new LastResultsCommand;
}