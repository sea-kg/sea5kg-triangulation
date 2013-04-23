//---------------------------------------------------------------------------

#pragma hdrstop

#include "_tr_logger.h"
namespace triangulation {

logger::logger(TMemo *pMemo)
 : m_pMemo(pMemo)
{

};
//---------------------------------------------------------------------------

void logger::info(UnicodeString msg)
{
	m_pMemo->Lines->Add("INFO: " + msg);
}

}
//---------------------------------------------------------------------------
#pragma package(smart_init)
