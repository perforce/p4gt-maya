:: setenv2022.bat
:: Setup environment including for Visual Studio 2022.
:: Sets environment variables used for building P4GT including Visual Studio 2022.
:: Add a section for your system.
:: VSINSTDIR: Visual Studio installation directory root. Parent of VC folder.
:: OSPLAT: OS Platform. X64 or X86.
:: SRC: Root of the workspace.
@echo off

if NOT "%COMPUTERNAME%" == "DESKTOP-G4OOK7Q" goto bail
@set VSINSTDIR="C:\Program Files\Microsoft Visual Studio\2022\Professional"
@set OSPLAT=X64
@set SRC=C:\Users\perforce\p4maya
@set T4TEMP=C:\Users\perforce\AppData\Local\Temp

:settings
@call %VSINSTDIR%\VC\Auxiliary\Build\vcvarsall.bat x64 10.0.22621.0 > nul 2>&1

@set JAM=%SRC%\jam\bin.ntx64

@set PATH=%JAM%;%PATH%

@rem Remove MS trailing slashes.
@set VSINSTALLDIR=%VSINSTDIR%
@set VCINSTALLDIR=%VSINSTDIR%\VC

@set VSINSTDIR=
REM @set MSVCNT=%VCINSTALLDIR%
@set MSVSVER=17
@set ZIP=7z a -r
@set BUILD_WITH_SUB_DIRECTORIES=true
@set LDAPSTUB=no
@set X3=no
@set UDT=no
@set SCP=yes
@set ASPARA=yes

@set SSL=yes
@set SSLVER=3

@set SSLINCDIR=%SRC%\openssl-3\include
@set SSLLIBDIR=%SRC%\openssl-3\lib

@goto finish

:bail
echo No settings for this machine

:finish
