REM.
REM. This script to automatically generates a version header file
REM. used by the .rc/.cpp files at build time. Must be run before
REM. building libp4gt.lib or build will fail.
REM.
REM. source file:  ..\..\p4\Version
REM. target file: ..\libp4gt\version.h
REM.
::
:: prepare the Command Processor
::
SETLOCAL
::
:: set file paths
::
SET SourceFile="..\\..\\p4\\Version"
SET TargetFile="..\\libp4gt\\version.h"
::
:: extract release info
::
TYPE %SourceFile% | FINDSTR /B RELEASE >%temp%\~p4gt_version.tmp 
FOR /F "tokens=1-5" %%A in (%temp%\~p4gt_version.tmp) DO (
SET P4_YEAR=%%C
SET P4_RELEASE=%%D
SET P4_SPECIAL=%%E
)
::
:: extract and format changelist info
::
TYPE %SourceFile% | FINDSTR /B PATCHLEVEL >%temp%\~p4gt_version.tmp 
FOR /F "usebackq tokens=1-3" %%A in (%temp%\~p4gt_version.tmp) DO (
SET P4_CHANGELEVEL=%%C
)
::
:: extract date info
::
TYPE %SourceFile% | FINDSTR /B SUPPDATE >%temp%\~p4gt_version.tmp 
FOR /F "tokens=1-5" %%A in (%temp%\~p4gt_version.tmp) DO (
SET P4_SUPPYEAR=%%C
SET P4_SUPPMONTH=%%D
SET P4_SUPPDAY=%%E
)
::
:: last minute formatting and construct %TargetFile% header file
::
SET P4_INT_MAJOR=%P4_CHANGELEVEL:~0,3%
SET P4_INT_MINOR=%P4_CHANGELEVEL:~3%
::
ECHO #define P4_FILE_VERSION %P4_YEAR%.%P4_RELEASE%.%P4_INT_MAJOR%.%P4_INT_MINOR% > %TargetFile%
ECHO #define P4_PRODUCT_VERSION "%P4_YEAR%.%P4_RELEASE%.%P4_INT_MAJOR%.%P4_INT_MINOR%" >> %TargetFile%
ECHO #define P4_COPYRIGHT "Copyright (C) 2004-2026 Perforce Software, Inc." >> %TargetFile%
ECHO #define P4_INT_MAJOR %P4_INT_MAJOR% >> %TargetFile%
ECHO #define P4_INT_MINOR %P4_INT_MINOR% >> %TargetFile%
ECHO #define P4_INT_HBUILD 0 >> %TargetFile%
ECHO #define P4_INT_LBUILD 0 >> %TargetFile%
ECHO #define P4_INT_YEAR %P4_YEAR% >> %TargetFile%
ECHO #define P4_INT_RELEASE %P4_RELEASE% >> %TargetFile%
::
:: -----------------------------------------------------------------------------------------------------------
::
:: !!!! PICK ONE OF THESE P4GT VERSION STRINGS TO USE !!!!
::
ECHO #define P4GT_VERSION "%P4_YEAR%.%P4_RELEASE%.%P4_INT_MAJOR%.%P4_INT_MINOR%.%P4_SPECIAL%" >> %TargetFile%

