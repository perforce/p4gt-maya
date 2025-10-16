:: Build P4 API.
:: Run from P4 folder.
::jam -s OSPLAT=X64 -s SSL=yes -s LDAPSTUB=no -s TYPE=dyn -s SMARTHEAP=no -s USE_EXTENSIONS=no -s API=x

:: New settings based on email from Brett Taylor 2021/12/17 to Kevin Mackey (and copying Joel Kovisto).
:: TYPE=dyn was added to match above.
if "%MSVSVER%" == "15" goto next1
set C++FLAGS=-DHAS_CPP11
:next1
set TYPE=dyn
set SSL=yes
set SSLSTUB=no
set SSLVER=1.1
set USE_EXTENSIONS=1
set WARNINGS_AS_ERRORS=1
set WIN32_WINNT=0x0601
set USE_WILDARGS=no
set MALLOC_OVERRIDE=no
set USE_MIMALLOC=no
set USE_SMARTHEAP=no

jam -a -s LDAPSTUB=no -s API=x
:: Delete variables.
set TYPE=
set USE_SMARTHEAP=
set USE_MIMALLOC=
set MALLOC_OVERRIDE=

set C++FLAGS=
set SSL=yes
set SSLSTUB=
set SSLVER=
set USE_EXTENSIONS=
set WARNINGS_AS_ERRORS=
set WIN32_WINNT=
set USE_WILDARGS=