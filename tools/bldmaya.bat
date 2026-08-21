REM
REM This batch file simulates the build process used by EC.
REM
REM Run this batch file fro top level p4-gt directory.
REM

REM rel_year is the release year of the *Autodesk Maya* that P4GT-Maya is built for.
set rel_year=2027

jam -a -s BUILD=en -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=en -s SMARTHEAP=no clean

jam -a -s BUILD=en -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=en -s SMARTHEAP=no libp4gt.lib

jam -a -s BUILD=en -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=en -s SMARTHEAP=no P4GT-Maya-%rel_year%_x64.mll


jam -a -s BUILD=ja -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=ja -s SMARTHEAP=no clean

jam -a -s BUILD=ja -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=ja -s SMARTHEAP=no libp4gt_ja.lib

jam -a -s BUILD=ja -s LDAPSTUB=no -s SSL=yes -s SSLVER=3 -s USE_WILDARGS=no -s OSPLAT=X64 -s TYPE=dyn -s LANG=ja -s SMARTHEAP=no P4GT-Maya-%rel_year%_x64_ja.mll

set rel_year=

