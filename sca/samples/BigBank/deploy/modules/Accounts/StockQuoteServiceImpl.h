
#include "StockQuoteService.h"


namespace com
{
    namespace bigbank
    {
        namespace stockquote
        {

    /*
     *  AccountsService C interface
     */

            class StockQuoteServiceImpl : public StockQuoteService {
	    public:
		virtual float getQuote(const char* symbol);

	};
	

}
} 		 
} // namespace;
