
@echo off

call vcvars32

echo Build of SDO started....

set FROM_DIR=c:\apache\checkout_rc1

set TO_SRC=c:\installsrc

set TO_BIN=c:\installbin

echo Copying SDO source...

if not exist %TO_SRC% mkdir %TO_SRC%
if not exist %TO_SRC%\tuscany_sdo-0.1.incubating-M1 mkdir %TO_SRC%\tuscany_sdo-0.1.incubating-M1 
if not exist %TO_SRC%\tuscany_sca-0.1.incubating-M1\projects mkdir %TO_SRC%\tuscany_sca-0.1.incubating-M1\projects 

echo Copying SDO source...

if not exist %TO_SRC%\tuscany_sdo-0.1.incubating-M1\runtime mkdir %TO_SRC%\tuscany_sdo-0.1.incubating-M1\runtime
 
xcopy/s %FROM_DIR%\sdo\runtime %TO_SRC%\tuscany_sdo-0.1.incubating-M1\runtime 

if not exist %TO_SRC%\tuscany_sdo-0.1.incubating-M1\projects mkdir %TO_SRC%\tuscany_sdo-0.1.incubating-M1\projects
 
xcopy/s %FROM_DIR%\sdo\projects %TO_SRC%\tuscany_sdo-0.1.incubating-M1\projects

copy %FROM_DIR%\sca\INSTALL   %TO_SRC%\tuscany_sca-0.1.incubating-M1
copy %FROM_DIR%\sca\LICENSE   %TO_SRC%\tuscany_sca-0.1.incubating-M1
copy %FROM_DIR%\sca\COPYING   %TO_SRC%\tuscany_sca-0.1.incubating-M1

copy %FROM_DIR%\sdo\INSTALL   %TO_SRC%\tuscany_sdo-0.1.incubating-M1
copy %FROM_DIR%\sdo\LICENSE   %TO_SRC%\tuscany_sdo-0.1.incubating-M1
copy %FROM_DIR%\sdo\COPYING   %TO_SRC%\tuscany_sdo-0.1.incubating-M1

rem install libxml2 to c:\apache\libs
set LIBXML2_HOME=c:\apache\libs\libxml2-2.6.19.win32

rem install iconv to c:\apache\libs
set ICONV_HOME=c:\apache\libs\iconv-1.9.1.win32

rem install axis2c
set AXIS2C_HOME=c:\axis\axis2c_92\axis2c-bin-0.92-win32

echo Building SDO....

set TUSCANY_SDOCPP=%FROM_DIR%\sdo\deploy

cd %FROM_DIR%\sdo\projects\tuscany_sdo

call build

echo Installing SDO....

if not exist %TO_BIN% mkdir %TO_BIN%
if not exist %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1 mkdir %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1 
if not exist %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1\deploy mkdir %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1\deploy 

xcopy/s %FROM_DIR%\sdo\deploy %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1\deploy
copy %FROM_DIR%\sdo\INSTALL   %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1
copy %FROM_DIR%\sdo\LICENSE   %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1
copy %FROM_DIR%\sdo\COPYING   %TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1

set TUSCANY_SDOCPP=%TO_BIN%\tuscany_sdo-bin-0.1.incubating-M1\deploy

echo SDO Build complete.
