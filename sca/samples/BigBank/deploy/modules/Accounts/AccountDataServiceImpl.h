

#include "AccountDataService.h"


namespace com
{
    namespace bigbank
    {
        namespace account
        {

    /*
     *  AccountDataService C interface
     */

        class AccountDataServiceImpl : public AccountDataService {

            public:
            // CheckingAccount is a data object containing
            // String "accountNumber"
            // float balance

            virtual DataObjectPtr /* CheckingAccount*/ getCheckingAccount(
                const char *id);

            // SavingsAccount is a data object containing
            // String accountNumber
            // float balance

            virtual DataObjectPtr /* SavingsAccount */ getSavingsAccount(
                const char *id);

            // StockAccount is a data object containing
            // String accountNumber
            // String symbol
            // int quantity

            virtual DataObjectPtr /* StockAccount */ getStockAccount (
                const char* id);

            };
        }
    }
}
