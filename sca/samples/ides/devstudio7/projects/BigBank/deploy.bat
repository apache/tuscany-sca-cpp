set BBDIR=..\..\..\..\BigBank

if not exist %BBDIR%\deploy mkdir                          %BBDIR%\deploy
if not exist %BBDIR%\deploy\packages mkdir                 %BBDIR%\deploy\packages
if not exist %BBDIR%\deploy\packages\Accounts mkdir        %BBDIR%\deploy\packages\Accounts
if not exist %BBDIR%\deploy\configuration mkdir            %BBDIR%\deploy\configuration
if not exist %BBDIR%\deploy\configuration\Accounts mkdir   %BBDIR%\deploy\configuration\Accounts
if not exist %BBDIR%\deploy\bin mkdir                      %BBDIR%\deploy\bin

copy %BBDIR%\Accounts\Debug\Accounts.dll                   %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\Debug\Accounts.pdb                   %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountServiceImpl.h                 %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountService.h                     %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountDataServiceImpl.h             %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountDataService.h                 %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteService.h                  %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteServiceImpl.h              %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteWebService.h               %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\Accounts.composite                   %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountServiceImpl.componentType     %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountDataServiceImpl.componentType %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteServiceImpl.componentType  %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\AccountService.wsdl                  %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteService.wsdl               %BBDIR%\deploy\packages\Accounts
copy %BBDIR%\Accounts\StockQuoteTypes.xsd                  %BBDIR%\deploy\packages\Accounts

copy %BBDIR%\BigBankSubsystem\BigBankSubsystem.composite   %BBDIR%\deploy\configuration\Accounts

copy AccountClient\Debug\AccountClient.exe                 %BBDIR%\deploy\bin
copy AccountClient\Debug\AccountClient.pdb                 %BBDIR%\deploy\bin
copy %BBDIR%\AccountClient\runclient.cmd                   %BBDIR%\deploy\bin

copy WSAccountClient\Debug\WSAccountClient.exe             %BBDIR%\deploy\bin
copy WSAccountClient\Debug\WSAccountClient.pdb             %BBDIR%\deploy\bin
copy %BBDIR%\WSAccountClient\runwsclient.cmd               %BBDIR%\deploy\bin


