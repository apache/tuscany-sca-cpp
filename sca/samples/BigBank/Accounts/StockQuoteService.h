

namespace com
{
    namespace bigbank
    {
        namespace stockquote
        {

    /*
     *  AccountsService C interface
     */

			class StockQuoteService {
				public:
				virtual float getQuote(const char* symbol) = 0;

			};

		}
	} 		 
} // namespace;

using namespace com::bigbank::stockquote;
