#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;


namespace com
{
    namespace bigbank
    {
        namespace stockquote
        {

    /*
     *  AccountsService C interface
     */

			class StockQuoteExternalService {
				public:
				virtual const char* GetQuote(const char* symbol) = 0;

			};

		}
	} 		 
} // namespace;
