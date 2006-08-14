if not exist deploy mkdir deploy
if not exist deploy\composites mkdir deploy\composites
if not exist deploy\composites\Accounts mkdir deploy\composites\Accounts
if not exist deploy\subsystems mkdir deploy\subsystems
if not exist deploy\subsystems\Accounts mkdir deploy\subsystems\Accounts
if not exist deploy\bin mkdir deploy\bin

copy Accounts\Debug\Accounts.dll deploy\composites\Accounts
copy Accounts\Debug\Accounts.pdb deploy\composites\Accounts
copy Accounts\AccountServiceImpl.h deploy\composites\Accounts
copy Accounts\AccountService.h deploy\composites\Accounts
copy Accounts\AccountDataServiceImpl.h deploy\composites\Accounts
copy Accounts\AccountDataService.h deploy\composites\Accounts
copy Accounts\StockQuoteService.h deploy\composites\Accounts
copy Accounts\StockQuoteServiceImpl.h deploy\composites\Accounts
copy Accounts\StockQuoteWebService.h deploy\composites\Accounts
copy Accounts\Accounts.composite deploy\composites\Accounts
copy Accounts\AccountServiceImpl.componentType deploy\composites\Accounts
copy Accounts\AccountDataServiceImpl.componentType deploy\composites\Accounts
copy Accounts\StockQuoteServiceImpl.componentType deploy\composites\Accounts
copy Accounts\AccountService.wsdl deploy\composites\Accounts
copy Accounts\StockQuoteService.wsdl deploy\composites\Accounts
copy Accounts\StockQuoteTypes.xsd deploy\composites\Accounts
copy Accounts\Tuscany-model.config deploy\composites\Accounts

copy BigBankSubsystem\BigBankSubsystem.composite deploy\subsystems\Accounts

copy AccountClient\Debug\AccountClient.exe deploy\bin
copy AccountClient\Debug\AccountClient.pdb deploy\bin
copy AccountClient\runclient.cmd deploy\bin

copy WSAccountClient\Debug\WSAccountClient.exe deploy\bin
copy WSAccountClient\Debug\WSAccountClient.pdb deploy\bin
copy WSAccountClient\runwsclient.cmd deploy\bin


