/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MGlobal.h>

#include "P4GTMaya.h"
#include "UpdateMenuCommand.h"
#include "MenuState.h"
#include "Translate.h"

void UpdateMenuCommand::SetMenuLabel( const char *name, TCHAR *label )
{
		MString strLabel;
		MString cmd;
		strLabel.setWChar(label);

		cmd.format("menuItem -edit -label \"^1s\" ^2s;", strLabel, name);
		MGlobal::executeCommand(cmd);
}

void UpdateMenuCommand::SetMenuState( const char *name, BOOL enabled )
{
	MString cmd( "menuItem -edit -en " );
	if ( enabled ) {
		cmd += "true ";
	}
	else {
		cmd += "false ";
	}
	cmd += name;
	MGlobal::executeCommand( cmd );
}

void UpdateMenuCommand::SetMenuChecked( const char *name, BOOL checked )
{
	MString cmd( "menuItem -edit -checkBox " );
	if ( checked ) {
		cmd += "on ";
	}
	else {
		cmd += "off ";
	}
	cmd += name;
	MGlobal::executeCommand( cmd );
}

MStatus UpdateMenuCommand::doIt( const MArgList& args )
{
	MenuState *state = perforceOps->GetMenuState();

	SetMenuLabel( "PerforceLockMenuItem", state->LockUnlockLabel() );
	SetMenuLabel( "PerforceUndoMenuItem", state->UndoLabel() );
	SetMenuLabel( "PerforceOnlineMenuItem", state->EnableDisableLabel() );

	SetMenuState( "PerforceAddMenuItem", state->AddEnabled() );
    SetMenuState( "PerforceCheckOutMenuItem", state->CheckOutEnabled() );
	SetMenuState( "PerforceCheckInMenuItem", state->CheckInEnabled() );
	SetMenuState( "PerforceLockMenuItem", state->LockUnlockEnabled() );
	SetMenuState( "PerforceUndoMenuItem", state->UndoEnabled() );
	SetMenuState( "PerforceStatusMenuItem", state->DocumentStatusEnabled() );
	SetMenuState( "PerforceHistoryMenuItem", state->DocumentHistoryEnabled() );
	SetMenuState( "PerforceResultsMenuItem", state->ResultsEnabled() );
	SetMenuState( "PerforceOpenedMenuItem", state->CheckedOutEnabled() );
	SetMenuState( "PerforceInfoMenuItem", state->InfoEnabled() );
	SetMenuState( "PerforceOpenMenuItem", state->OpenEnabled() );


	delete state;

    return MS::kSuccess;
}

void* UpdateMenuCommand::creator()
{
    return new UpdateMenuCommand;
}