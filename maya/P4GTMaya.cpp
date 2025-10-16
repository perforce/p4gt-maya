/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <tchar.h>

#include <maya/MFnPlugin.h>
#include <maya/MSceneMessage.h>
#include <maya/M3dView.h>

#define P4MAYA_MAIN
#include "P4GTMaya.h"
#include "..\libp4gt\version.h"

#include "AddCommand.h"
#include "CheckInCommand.h"
#include "CheckOutCommand.h"
#include "LockCommand.h"
#include "UndoCommand.h"
#include "StatusCommand.h"
#include "HistoryCommand.h"
#include "OpenedCommand.h"
#include "OptionsCommand.h"
#include "InfoCommand.h"
#include "AboutCommand.h"
#include "HelpCommand.h"
#include "UpdateMenuCommand.h"
#include "OpenCommand.h"
#include "LastResultsCommand.h"
#include "OnlineCommand.h"

/*
 *	Description:
 *		this method is called when the plug-in is loaded into Maya.  It
 *		registers all of the services that this plug-in provides with
 *		Maya.
 *
 *	Arguments:
 *		obj - a handle to the plug-in object (use MFnPlugin to access it)
 */
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	StrBuf buff;
	Translate::FromWinChar( _T(P4GT_VERSION), &buff );
	MFnPlugin plugin( obj, "Perforce Software Inc.", buff.Text(), "Any");

	// Save Dll instance handle
	hInstance = MhInstPlugin;

	host = new MayaHost( MhInstPlugin, M3dView::applicationShell() );
	perforceOps = new PluginOperations( host );

	// Register menu commands
	plugin.registerCommand( "p4UpdateMenu", UpdateMenuCommand::creator );
	plugin.registerCommand( "p4Add", AddCommand::creator );
	plugin.registerCommand( "p4Edit", CheckOutCommand::creator );
	plugin.registerCommand( "p4Submit", CheckInCommand::creator );
	plugin.registerCommand( "p4Lock", LockCommand::creator );
	plugin.registerCommand( "p4Revert", UndoCommand::creator );
	plugin.registerCommand( "p4Opened", OpenedCommand::creator );
	plugin.registerCommand( "p4Stat", StatusCommand::creator );
	plugin.registerCommand( "p4History", HistoryCommand::creator );
	plugin.registerCommand( "p4Open", OpenCommand::creator );
	plugin.registerCommand( "p4Options", OptionsCommand::creator );
	plugin.registerCommand( "p4Info", InfoCommand::creator );
	plugin.registerCommand( "p4About", AboutCommand::creator );
	plugin.registerCommand( "p4Help", HelpCommand::creator );
	plugin.registerCommand( "p4Results", LastResultsCommand::creator );
	plugin.registerCommand( "p4Online", OnlineCommand::creator );

	// Register callbacks
	callbacks = new CallbackManager();

	// Set the mel procs to be run when the plugin is loaded / unloaded
	//
	// Status is good even if the perforce .mel file does not exist.
	//
#ifdef LANG_ja
	status = plugin.registerUI("perforceCreateUI_ja", "perforceDeleteUI_ja");
#else
	status = plugin.registerUI("perforceCreateUI", "perforceDeleteUI");
#endif  // LANG_ja
    if (!status) {
        status.perror("registerUIScripts");
        return status;
    }

	return status;
}

/*
 *	Description:
 *		this method is called when the plug-in is unloaded from Maya. It
 *		deregisters all of the services that it was providing.
 *
 *	Arguments:
 *		obj - a handle to the plug-in object (use MFnPlugin to access it)
 */
MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	delete host;
	delete perforceOps;
	delete callbacks;

	// Deregister menu commands
	plugin.deregisterCommand( "p4UpdateMenu" );
	plugin.deregisterCommand( "p4Add" );
	plugin.deregisterCommand( "p4Edit" );
	plugin.deregisterCommand( "p4Submit" );
	plugin.deregisterCommand( "p4Lock" );
	plugin.deregisterCommand( "p4Revert" );
	plugin.deregisterCommand( "p4Opened" );
	plugin.deregisterCommand( "p4Stat" );
	plugin.deregisterCommand( "p4History" );
	plugin.deregisterCommand( "p4Open" );
	plugin.deregisterCommand( "p4Options" );
	plugin.deregisterCommand( "p4Info" );
	plugin.deregisterCommand( "p4About" );
	plugin.deregisterCommand( "p4Help" );
	plugin.deregisterCommand( "p4Results" );
	plugin.deregisterCommand( "p4Online" );

	return status;
}
