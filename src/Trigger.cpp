#include "Trigger.h"

Trigger::Trigger()
{
}

HRESULT Trigger::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

HRESULT Trigger::InitVBA(bool fNew, int id, wchar_t* const wzName)
{
	return S_OK;
}