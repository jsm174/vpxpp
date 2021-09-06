#pragma once

#include "BiffReader.h"
#include "ILoadable.h"

#include "pole.h"

class PinBinary : public ILoadable
{
public:
	PinBinary();
	virtual ~PinBinary();

	// TODO: HRESULT SaveToStream(POLE::Stream* pStream);
	HRESULT LoadFromStream(POLE::Stream* pStream, int version);

	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	std::string m_szName;
	std::string m_szPath;

	char* m_pData;
	int m_cdata;

private:
};