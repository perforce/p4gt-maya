/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include "MayaHost.h"
#include "PluginOperations.h"
#include "CallbackManager.h"

#ifdef P4MAYA_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif // P4MAYA_MAIN

/*
 * Plugin DLL handle.
 */
EXTERN HINSTANCE hInstance;

/*
 * Provides Maya specific callbacks to plugin library
 */
EXTERN MayaHost *host;

/*
 * Plugin operations interface
 */
EXTERN PluginOperations *perforceOps;

/* 
 * Interface to callback handler
 */
EXTERN CallbackManager *callbacks;