/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#ifndef INC_CALLBACK_MANAGER
#define INC_CALLBACK_MANAGER

#include <maya/MSceneMessage.h>

/*
 * Class to handle callbacks when documents are opened/closed.
 */
class CallbackManager
{

public:
	
	/*
	 * Constructor. Register callbacks here.
	 */
				CallbackManager();

	/*
	 * Destructor. Deregister callbacks here.
	 */
				~CallbackManager();

private:

	/*
	 * This gets called when a document is opened.
	 *
	 * data - userdata.
	 */
	static void promptOpenDocument( void *data );

	/*
	 * This gets called when a document is closed.
	 *
	 * data - userdata.
	 */
	static void promptCloseDocument( void *data );

	MCallbackId beforeNewId;
	MCallbackId beforeOpenId;
	MCallbackId mayaExitingId;
	MCallbackId afterOpenId;
};

#endif