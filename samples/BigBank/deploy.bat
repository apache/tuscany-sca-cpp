if not exist deploy mkdir deploy
if not exist deploy\packages mkdir deploy\packages
if not exist deploy\packages\Accounts mkdir deploy\packages\Accounts
if not exist deploy\configuration mkdir deploy\configuration
if not exist deploy\configuration\Accounts mkdir deploy\configuration\Accounts
if not exist deploy\bin mkdir deploy\bin

copy Accounts\Debug\Accounts.dll deploy\packages\Accounts
copy Accounts\Debug\Accounts.pdb deploy\packages\Accounts
copy Accounts\AccountServiceImpl.h deploy\packages\Accounts
copy Accounts\AccountService.h deploy\packages\Accounts
copy Accounts\AccountDataServiceImpl.h deploy\packages\Accounts
copy Accounts\AccountDataService.h deploy\packages\Accounts
copy Accounts\StockQuoteService.h deploy\packages\Accounts
copy Accounts\StockQuoteServiceImpl.h deploy\packages\Accounts
copy Accounts\StockQuoteWebService.h deploy\packages\Accounts
copy Accounts\Accounts.composite deploy\packages\Accounts
copy Accounts\AccountServiceImpl.componentType deploy\packages\Accounts
copy Accounts\AccountDataServiceImpl.componentType deploy\packages\Accounts
copy Accounts\StockQuoteServiceImpl.componentType deploy\packages\Accounts
copy Accounts\AccountService.wsdl deploy\packages\Accounts
copy Accounts\StockQuoteService.wsdl deploy\packages\Accounts
copy Accounts\StockQuoteTypes.xsd deploy\packages\Accounts
copy Accounts\Tuscany-model.config deploy\packages\Accounts

copy BigBankSubsystem\BigBankSubsystem.composite deploy\configuration\Accounts

copy AccountClient\Debug\AccountClient.exe deploy\bin
copy AccountClient\Debug\AccountClient.pdb deploy\bin
copy AccountClient\runclient.cmd deploy\bin

copy WSAccountClient\Debug\WSAccountClient.exe deploy\bin
copy WSAccountClient\Debug\WSAccountClient.pdb deploy\bin
copy WSAccountClient\runwsclient.cmd deploy\bin


