#include "DispReel.h"

DispReel::DispReel()
{

}

HRESULT DispReel::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

HRESULT DispReel::InitVBA(bool fNew, int id, wchar_t * const wzName) 
{ 
	wchar_t wzUniqueName[128]; 
	if (fNew && !wzName) 
	{ 
		{ 
			//GetPTable()->GetUniqueName(eItemDispReel, wzUniqueName, 128); 
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/ 
		} 
	} 
	//InitScript(); 
	return S_OK; 
}