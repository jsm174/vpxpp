#pragma once

#include "misc.h"
#include "pole.h"
#include "ILoadable.h"

#define FID(A) (int)((unsigned int)(#A[0])|((unsigned int)(#A[1])<<8)|((unsigned int)(#A[2])<<16)|((unsigned int)(#A[3])<<24))

class ILoadable;

class BiffReader 
{
public:
	BiffReader(POLE::Stream* pStream, ILoadable* pLoadable, void* pData, const int version);
	virtual ~BiffReader();

	HRESULT ReadBytes(void* pValue, const unsigned long count);
	
	HRESULT GetIntNoHash(int& value);
	HRESULT GetInt(void* pValue);
	HRESULT GetInt(int& value);
	HRESULT GetString(std::string& szvalue);
	HRESULT GetWideString(wchar_t* wzvalue, int maxlen);
	HRESULT GetWideString(std::wstring& wzvalue);
	HRESULT GetFloat(float& value);
	HRESULT GetBool(bool& value);
	HRESULT GetStruct(void* pValue, const int size);

	HRESULT Load();

	POLE::Stream* m_pStream;
	void* m_pData;
	int m_version;

private:
	int m_bytesInRecordRemaining;
	ILoadable* m_pLoadable; 
};