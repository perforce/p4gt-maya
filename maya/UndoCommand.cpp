/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "P4GTMaya.h"
#include "UndoCommand.h"

MStatus UndoCommand::doIt( const MArgList& args ) 
{
	perforceOps->Revert();
    return MS::kSuccess;
}

void* UndoCommand::creator() {
    return new UndoCommand;
}