/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

/*
 * Class to handle "Opened Documents" menu command
 */
class OpenedCommand : public MPxCommand
{

public:

	/*
	 * Carry out action
	 */
    MStatus        doIt( const MArgList& args );

	/*
	 * Return an instance of this class
	 */
    static void*   creator();
};