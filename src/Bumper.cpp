#include "Bumper.h"

Bumper::Bumper()
{
	// TODO: m_pbumperhitcircle = NULL;
	m_baseVertexBuffer = NULL;
	m_baseIndexBuffer = NULL;
	m_ringVertexBuffer = NULL;
	m_ringIndexBuffer = NULL;
	m_capVertexBuffer = NULL;
	m_capIndexBuffer = NULL;
	m_socketIndexBuffer = NULL;
	m_socketVertexBuffer = NULL;
	m_ringAnimate = false;
	// TODO: m_propVisual = NULL;
	m_d.m_ringDropOffset = 0.0f;
	m_ringDown = false;
	m_updateSkirt = false;
	m_doSkirtAnimation = false;
	m_enableSkirtAnimation = true;
	m_skirtCounter = 0.0f;
}

HRESULT Bumper::InitLoad(POLE::Stream* pStream, PinTable* pTable, int* pId, int version)
{
	return S_OK;
}

HRESULT Bumper::InitVBA(bool fNew, int id, wchar_t * const wzName) 
{ 
	wchar_t wzUniqueName[128]; 
	if (fNew && !wzName) 
	{ 
		{ 
			//GetPTable()->GetUniqueName(eItemBumper, wzUniqueName, 128); 
			//WideStrNCopy(wzUniqueName, (wchar_t *)m_wzName, sizeof(m_wzName)/sizeof(m_wzName[0]));/*lstrcpyW((WCHAR *)m_wzName, wzUniqueName);*/ 
		} 
	} 
	//InitScript(); 
	return S_OK; 
}