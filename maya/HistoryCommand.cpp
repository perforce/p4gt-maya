/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "HistoryCommand.h"

MStatus HistoryCommand::doIt( const MArgList& args ) 
{

	perforceOps->ShowHistory();
    return MS::kSuccess;
}

void* HistoryCommand::creator() {
    return new HistoryCommand;
}