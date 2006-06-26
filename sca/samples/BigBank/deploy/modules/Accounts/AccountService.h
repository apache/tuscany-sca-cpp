/**
 * AccountService.h
 *
 */


#include <commonj/sdo/SDO.h>


namespace com
{
    namespace bigbank
    {
        namespace account
        {

    /*
     *  AccountService C interface
     */

            class AccountService {
 		 
            public:

                //The account report data object has one many-valued
                // property "accountSummaries".
                // This had properties
                // String accountNumber
                // String accountType
                // Float balance.

                 virtual commonj::sdo::DataObjectPtr /*AccountReport*/
                     getAccountReport(DataObjectPtr cid) = 0;
         
        
            };
    
        }   
    }
} // namespaces 
    