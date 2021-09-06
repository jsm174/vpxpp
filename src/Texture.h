#pragma once

#include "BaseTexture.h"
#include "BiffReader.h"
#include "ILoadable.h"
#include "PinBinary.h"

#include "misc.h"
#include "pole.h"

class PinTable;

class Texture : public ILoadable
{
public:
	Texture();
	virtual ~Texture();

	HRESULT SaveToStream(POLE::Stream* pStream, PinTable* pPinTable);
	HRESULT LoadFromStream(POLE::Stream* pStream, int version, PinTable* pPinTable);

	virtual bool LoadToken(const int id, BiffReader* const pBiffReader);

	void FreeStuff();

	void SetSizeFrom(const BaseTexture* const tex);

	int m_width;
	int m_height;
	int m_realWidth;
	int m_realHeight;
	float m_alphaTestValue;
	BaseTexture* m_pdsBuffer;

	// TODO: HBITMAP m_hbmGDIVersion;
	PinBinary* m_ppb;

	std::string m_szName;
	std::string m_szPath;

private:
	bool LoadFromMemory(BYTE* const data, const DWORD size);
};