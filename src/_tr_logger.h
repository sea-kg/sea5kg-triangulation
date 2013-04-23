//---------------------------------------------------------------------------

#ifndef _tr_loggerH
#define _tr_loggerH

#include<vcl.h>

namespace triangulation {
class logger
{
	public:
		logger(TMemo *pMemo);
		void info(UnicodeString msg);
	private:
		TMemo *m_pMemo;
};
}
//---------------------------------------------------------------------------
#endif
