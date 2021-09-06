#include "PinBinary.h"

PinBinary::PinBinary()
{
}

PinBinary::~PinBinary()
{
}

HRESULT PinBinary::LoadFromStream(POLE::Stream* pStream, int version)
{
	BiffReader biffReader(pStream, this, NULL, version);
	biffReader.Load();

	return S_OK;
}

bool PinBinary::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(NAME):
		pBiffReader->GetString(m_szName);
		break;
	case FID(PATH):
		pBiffReader->GetString(m_szPath);
		break;
	case FID(SIZE):
	{
		pBiffReader->GetInt(m_cdata);
		m_pData = new char[m_cdata];
		break;
	}
	case FID(DATA):
		pBiffReader->GetStruct(m_pData, m_cdata);
		break;
	}
	return true;
}
