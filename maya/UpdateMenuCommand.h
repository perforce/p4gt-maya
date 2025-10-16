/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 */

#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

/*
 * Class to handle the Perforce menu
 */
class UpdateMenuCommand : public MPxCommand
{

public:

	/*
	 * This gets called then menu is about to be shown
	 */
    MStatus        doIt( const MArgList& args );

	/*
	 * Return an instance of this class
	 */
    static void*   creator();

private:

	/*
	 * Set state of menu item to be enabled or disabled.
	 *
	 * name - The name of the menu item.
	 * enabled - New state of menu item.
	 */
	void			SetMenuState( const char *name, BOOL enabled );

	/*
	 * Set check of menu item.
	 *
	 * name - The name of the menu item.
	 * checked - Whether or not menu item is checked.
	 */
	void			SetMenuChecked( const char *name, BOOL checked );

	/*
	 * Set label of menu item.
	 *
	 * name - The name of the menu item.
	 * label - New label menu item.
	 */
	void			SetMenuLabel( const char *name, TCHAR *label );
};