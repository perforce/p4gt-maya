:: setenv2017.bat
:: Setup environment including for Visual Studio 2017.
:: Sets environment variables used for building P4GT including Visual Studio 2017.
:: Add a section for your system.
:: VSINSTDIR: Visual Studio installation directory root. Parent of VC folder.
:: OSPLAT: OS Platform. X64 or X86.
:: SRC: Root of the workspace.
:: T4TEMP: Temporary for . . . ?
@echo off

if NOT "%COMPUTERNAME%" == "INSTDEV1" goto bail
@set VSINSTDIR="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional"
@set OSPLAT=X64
@set SRC=C:\Users\kmackey\perforce\kmackey_p4gt
@set T4TEMP=c:\Temp

:settings
:: Note that this VS 2017 path, file, and parameter is different than for VS 2015.
:: We must use the SDK version since the newer SDKs are using DDK symbols.
:: Meaning WINNT_ type symbols are being changed to NTDDI_ type symbols.
:: This has been on MS's list for some time. See this article:
:: https://devblogs.microsoft.com/oldnewthing/20081205-00/?p=19943
@call %VSINSTDIR%\VC\Auxiliary\Build\vcvarsall.bat x64 10.0.17763.0 > nul 2>&1

@set P4CONFIG=p4config

@set JAM=%SRC%\depot\main\jam\bin.ntx64

@set PATH=%JAM%;%PATH%

@rem Remove MS trailing slashes.
@set VSINSTALLDIR=%VSINSTDIR%
@set VCINSTALLDIR=%VSINSTDIR%\VC

@set VSINSTDIR=
REM @set MSVCNT=%VCINSTALLDIR%
@set MSVSVER=15
@set ZIP=7z a -r
@set BUILD_WITH_SUB_DIRECTORIES=true
@set LDAPSTUB=no
@set X3=no
@set UDT=no
@set SCP=yes
@set ASPARA=yes

@set TP=%SRC%\3rd_party

@set SSL=yes
@set SSLVER=1.1

@set SSLINCDIR=%TP%\cpp_libraries\openssl\1.1.1-latest\artifacts\inc.ntx64
@set SSLLIBDIR=%TP%\cpp_libraries\openssl\1.1.1-latest\artifacts\lib.ntx64\vs15

@set UDTINCDIR=%TP%\cpp_libraries\udt\4.11\artifacts\inc.ntx64
@set UDTLIBDIR=%TP%\cpp_libraries\udt\4.11\artifacts\lib.ntx64

@set TP=

@goto finish


:bail
echo No settings for this machine

:finish


