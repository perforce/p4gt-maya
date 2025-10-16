/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "PerforceCommand.h"
#include "debug.h"

PerforceCommand::PerforceCommand( TCHAR* cmd ) : command( cmd ) 
{
}

PerforceCommand::PerforceCommand( TCHAR* cmd, TCHAR *arg1 ) : command( cmd )
{
	Append( arg1 );
}

PerforceCommand::PerforceCommand( TCHAR* cmd, TCHAR* arg1, TCHAR* arg2 ) : command( cmd ) 
{
	Append( arg1 );
	Append( arg2 );
}

PerforceCommand::PerforceCommand( TCHAR* cmd, TCHAR *arg1, TCHAR *arg2, TCHAR *arg3 ) : command( cmd )
{
	Append( arg1 );
	Append( arg2 );
	Append( arg3 );
}

PerforceCommand::PerforceCommand( TCHAR* cmd, TCHAR *arg1, TCHAR *arg2, TCHAR *arg3, TCHAR *arg4 ) : command( cmd ) 
{
	Append( arg1 );
	Append( arg2 );
	Append( arg3 );
	Append( arg4 );
}

PerforceCommand::~PerforceCommand() 
{
	for ( int i=0; i<buffs.Count(); i++ ) {
		delete (StrBuf *)buffs.Get( i );
	}
}

void PerforceCommand::SetInputVar( char *var, TCHAR *val ) 
{
	StrBuf buff;
	Translate::FromWinChar( val, &buff );
	input.SetVar( var, buff );
}

StrDict* PerforceCommand::GetInput() 
{
	return &input;
}

char* PerforceCommand::GetCommand() 
{
	static StrBuf buff;
	Translate::FromWinChar( command, &buff );
	return buff.Text();
}

char** PerforceCommand::GetArgs() 
{
	return (char **)args.ElemTab();
}

int PerforceCommand::GetNumArgs() 
{
	return args.Count();
}

void PerforceCommand::Append( TCHAR* arg ) 
{
	StrBuf *buff = new StrBuf();
	Translate::FromWinChar( arg, buff );
	args.Put( buff->Text() );
	buffs.Put( buff );
}