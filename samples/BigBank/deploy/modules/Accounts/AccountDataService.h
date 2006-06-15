


#include "commonj/sdo/SDO.h"

namespace com
{
    namespace bigbank
    {
        namespace account
        {

    /*
     *  AccountDataService C interface
     */

    class AccountDataService {

    public:

        // CheckingAccount is a data object containing
        // String "accountNumber"
        // float balance

        virtual commonj::sdo::DataObjectPtr /* CheckingAccount*/ getCheckingAccount(
            const char *id) = 0;

        // SavingsAccount is a data object containing
        // String accountNumber
        // float balance

        virtual commonj::sdo::DataObjectPtr /* SavingsAccount */ getSavingsAccount(
            const char *id) = 0;

        // StockAccount is a data object containing
        // String accountNumber
        // String symbol
        // int quantity

        virtual commonj::sdo::DataObjectPtr /* StockAccount */ getStockAccount (
            const char* id) = 0;

         
    };

}
} 		 
} // namespace;
