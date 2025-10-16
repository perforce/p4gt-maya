/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

/*
 * Class to handle "Check In" menu command
 */
class CheckInCommand : public MPxCommand
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