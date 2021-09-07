#include "Texture.h"
#include "LZWReader.h"
#include "PinTable.h"
#include "RegUtil.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_NO_STDIO
#define STBI_NO_FAILURE_STRINGS
#include "stb_image.h"

Texture::Texture()
{
	m_pdsBuffer = NULL;
	// TODO: m_hbmGDIVersion = NULL;
	m_ppb = NULL;
	m_alphaTestValue = 1.0f;
}

Texture::~Texture()
{
}

HRESULT Texture::SaveToStream(POLE::Stream* pStream, PinTable* pPinTable)
{
}

HRESULT Texture::LoadFromStream(POLE::Stream* pStream, int version, PinTable* pPinTable)
{
	BiffReader biffReader(pStream, this, pPinTable, version);
	biffReader.Load();

	return ((m_pdsBuffer != NULL) ? S_OK : E_FAIL);
}

bool Texture::LoadToken(const int id, BiffReader* const pBiffReader)
{
	switch (id)
	{
	case FID(NAME):
		pBiffReader->GetString(m_szName);

		break;
	case FID(PATH):
		pBiffReader->GetString(m_szPath);
		break;
	case FID(WDTH):
		pBiffReader->GetInt(m_width);
		break;
	case FID(HGHT):
		pBiffReader->GetInt(m_height);
		break;
	case FID(ALTV):
		pBiffReader->GetFloat(m_alphaTestValue);
		break;
	case FID(BITS):
	{
		if (m_pdsBuffer)
		{
			FreeStuff();
		}

		m_pdsBuffer = new BaseTexture(m_width, m_height, RGBA, true);

		SetSizeFrom(m_pdsBuffer);

		LZWReader lzwreader(pBiffReader->m_pStream, (int*)m_pdsBuffer->data(), m_width * 4, m_height, m_pdsBuffer->pitch());
		lzwreader.Decoder();

		const int lpitch = m_pdsBuffer->pitch();

		BYTE* const __restrict pch = (BYTE*)m_pdsBuffer->data();
		bool allAlphaZero = true;
		for (int i = 0; i < m_height; i++)
		{
			unsigned int o = i * lpitch + 3;
			for (int l = 0; l < m_width; l++, o += 4)
			{
				if (pch[o] != 0)
				{
					allAlphaZero = false;
					goto endAlphaCheck;
				}
			}
		}
	endAlphaCheck:

		if (allAlphaZero)
		{
			m_pdsBuffer->m_has_alpha = false;
			for (int i = 0; i < m_height; i++)
			{
				unsigned int o = i * lpitch + 3;
				for (int l = 0; l < m_width; l++, o += 4)
				{
					pch[o] = 0xff;
				}
			}
		}

		break;
	}
	case FID(JPEG):
	{
		m_ppb = new PinBinary();
		m_ppb->LoadFromStream(pBiffReader->m_pStream, pBiffReader->m_version);
		return LoadFromMemory((BYTE*)m_ppb->m_pData, m_ppb->m_cdata);
	}
	case FID(LINK):
	{
		int linkid;
		pBiffReader->GetInt(linkid);
		PinTable* const pPinTable = (PinTable*)pBiffReader->m_pData;
		m_ppb = pPinTable->GetImageLinkBinary(linkid);
		return LoadFromMemory((BYTE*)m_ppb->m_pData, m_ppb->m_cdata);
	}
	}
	return true;
}

void Texture::FreeStuff()
{
	delete m_pdsBuffer;
	m_pdsBuffer = NULL;

	// if (m_hbmGDIVersion)
	// {
	// 	if (m_hbmGDIVersion != g_pvp->m_hbmInPlayMode)
	// 	{
	// 		DeleteObject(m_hbmGDIVersion);
	// 	}
	// 	m_hbmGDIVersion = NULL;
	// }

	if (m_ppb)
	{
		delete m_ppb;
		m_ppb = NULL;
	}
}

bool Texture::LoadFromMemory(BYTE* const data, const DWORD size)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	if (m_pdsBuffer)
	{
		FreeStuff();
	}

	const int maxTexDim = pRegUtil->LoadValueIntWithDefault("Player", "MaxTexDimension", 0);
	if (maxTexDim <= 0)
	{
		int x;
		int y;
		int channels_in_file;
		unsigned char* const __restrict stbi_data = stbi_load_from_memory(data, size, &x, &y, &channels_in_file, 4);

		if (stbi_data)
		{
			BaseTexture* tex = NULL;
			try
			{
				tex = new BaseTexture(x, y, RGBA, channels_in_file == 4);
			}

			catch (...)
			{
				if (tex)
				{
					delete tex;
				}

				goto freeimage_fallback;
			}

			DWORD* const __restrict pdst = (DWORD*)tex->data();
			DWORD* const __restrict psrc = (DWORD*)stbi_data;
			assert(tex->pitch() == x * 4);
			unsigned int o = 0;
			for (int yo = 0; yo < y; ++yo)
			{
				for (int xo = 0; xo < x; ++xo, ++o)
				{
					const DWORD src = psrc[o];
					// TODO: pdst[o] = (src & 0xFF00FF00u) | _rotl(src & 0x00FF00FFu, 16);
					pdst[o] = (src & 0xFF00FF00u) | std::rotl(src & 0x00FF00FFu, 16);
				}
			}

			stbi_image_free(stbi_data);

			tex->m_realWidth = x;
			tex->m_realHeight = y;

			m_pdsBuffer = tex;
			SetSizeFrom(m_pdsBuffer);

			return true;
		}
	}

freeimage_fallback:

	FIMEMORY* const hmem = FreeImage_OpenMemory(data, size);
	if (!hmem)
	{
		return false;
	}
	const FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
	FIBITMAP* const dib = FreeImage_LoadFromMemory(fif, hmem, 0);

	FreeImage_CloseMemory(hmem);
	if (!dib)
	{
		return false;
	}

	m_pdsBuffer = BaseTexture::CreateFromFreeImage(dib);
	SetSizeFrom(m_pdsBuffer);

	return true;
}

void Texture::SetSizeFrom(const BaseTexture* const tex)
{
	m_width = tex->width();
	m_height = tex->height();
	m_realWidth = tex->m_realWidth;
	m_realHeight = tex->m_realHeight;
}

bool Texture::IsHDR() const
{
	if (m_pdsBuffer == NULL)
	{
		return false;
	}

	return (m_pdsBuffer->m_format == RGB_FP);
}

void Texture::DumpBaseTexture()
{
	size_t i = m_szPath.rfind('\\', m_szPath.length());

	if (i == std::string::npos)
	{
		return;
	}

	std::string filename = "texture_";
	filename.append(m_szPath.substr(i + 1, m_szPath.length() - i));

	FIBITMAP* dib;

	if (m_pdsBuffer->m_format == RGBA)
	{
		dib = FreeImage_ConvertFromRawBits(
			(BYTE*)&m_pdsBuffer->m_data[0],
			m_pdsBuffer->m_realWidth,
			m_pdsBuffer->m_realHeight,
			m_pdsBuffer->pitch(),
			32,
			FI_RGBA_RED_MASK,
			FI_RGBA_BLUE_MASK,
			FI_RGBA_GREEN_MASK,
			true);
	}
	else
	{
		dib = FreeImage_ConvertFromRawBitsEx(
			true,
			(BYTE*)&m_pdsBuffer->m_data[0],
			FIT_RGBF,
			m_pdsBuffer->m_realWidth,
			m_pdsBuffer->m_realHeight,
			m_pdsBuffer->pitch(),
			96,
			0,
			0,
			0,
			true);
	}

	if (IsHDR())
	{
		FreeImage_Save(FIF_HDR, dib, filename.c_str());
	}
	else if (filename.find(".bmp") != std::string::npos)
	{
		FreeImage_Save(FIF_BMP, dib, filename.c_str());
	}
	else if (filename.find(".jpg") != std::string::npos)
	{
		FIBITMAP* tempDib = FreeImage_ConvertTo24Bits(dib);
		FreeImage_Save(FIF_JPEG, tempDib, filename.c_str());
		FreeImage_Unload(tempDib);
	}
	else if (filename.find(".png") != std::string::npos)
	{
		FreeImage_Save(FIF_PNG, dib, filename.c_str());
	}

	FreeImage_Unload(dib);
}