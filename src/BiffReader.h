#pragma once

#include "ILoadable.h"
#include "misc.h"
#include "pole.h"

#include "vector.h"

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
	HRESULT GetString(char* const szvalue, const size_t szvalue_maxlength);
	HRESULT GetString(std::string& szvalue);
	HRESULT GetWideString(wchar_t* wzvalue, int maxlen);
	HRESULT GetWideString(std::wstring& wzvalue);
	HRESULT GetFloat(float& value);
	HRESULT GetBool(bool& value);
	HRESULT GetStruct(void* pValue, const int size);
	HRESULT GetVector2(Vertex2D& vec);
	HRESULT GetVector3(Vertex3Ds& vec);
	HRESULT GetVector3Padded(Vertex3Ds& vec);

	HRESULT Load();

	POLE::Stream* m_pStream;
	void* m_pData;
	int m_version;

private:
	int m_bytesInRecordRemaining;
	ILoadable* m_pLoadable;
};