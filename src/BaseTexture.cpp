#include "BaseTexture.h"
#include "RegUtil.h"

#include "Inlines.h"

#include "def.h"

BaseTexture* BaseTexture::CreateFromFile(const std::string& szfile)
{
	if (szfile.empty())
	{
		return NULL;
	}

	FREE_IMAGE_FORMAT fif;

	fif = FreeImage_GetFileType(szfile.c_str(), 0);

	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(szfile.c_str());
	}

	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		FIBITMAP* const dib = FreeImage_Load(fif, szfile.c_str(), 0);
		if (!dib)
		{
			return NULL;
		}

		BaseTexture* const mySurface = BaseTexture::CreateFromFreeImage(dib);

		return mySurface;
	}

	return NULL;
}

BaseTexture* BaseTexture::CreateFromFreeImage(FIBITMAP* dib)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	int maxTexDim = pRegUtil->LoadValueIntWithDefault("Player", "MaxTexDimension", 0);
	if (maxTexDim <= 0)
	{
		maxTexDim = 65536;
	}

	const int pictureWidth = FreeImage_GetWidth(dib);
	const int pictureHeight = FreeImage_GetHeight(dib);

	FIBITMAP* dibResized = dib;
	FIBITMAP* dibConv = dib;
	BaseTexture* tex = NULL;

	bool success = false;

	while (!success)
	{
		if (maxTexDim <= 0)
		{
			FreeImage_Unload(dib);
			return NULL;
		}

		if ((pictureHeight > maxTexDim) || (pictureWidth > maxTexDim))
		{
			int newWidth = max(min(pictureWidth, maxTexDim), MIN_TEXTURE_SIZE);
			int newHeight = max(min(pictureHeight, maxTexDim), MIN_TEXTURE_SIZE);

			if (pictureWidth - newWidth > pictureHeight - newHeight)
			{
				newHeight = min(pictureHeight * newWidth / pictureWidth, maxTexDim);
			}
			else
			{
				newWidth = min(pictureWidth * newHeight / pictureHeight, maxTexDim);
			}

			dibResized = FreeImage_Rescale(dib, newWidth, newHeight, FILTER_BILINEAR);
		}
		else if (pictureWidth < MIN_TEXTURE_SIZE || pictureHeight < MIN_TEXTURE_SIZE)
		{
			const int newWidth = max(pictureWidth, MIN_TEXTURE_SIZE);
			const int newHeight = max(pictureHeight, MIN_TEXTURE_SIZE);
			dibResized = FreeImage_Rescale(dib, newWidth, newHeight, FILTER_BOX);
		}

		if (!dibResized)
		{
			maxTexDim /= 2;

			while ((maxTexDim > pictureHeight) && (maxTexDim > pictureWidth))
			{
				maxTexDim /= 2;
			}

			continue;
		}

		const FREE_IMAGE_TYPE img_type = FreeImage_GetImageType(dibResized);
		const bool rgbf = (img_type == FIT_FLOAT) || (img_type == FIT_DOUBLE) || (img_type == FIT_RGBF) || (img_type == FIT_RGBAF);
		const bool has_alpha = !!FreeImage_IsTransparent(dibResized);

		if (((img_type == FIT_BITMAP) && (FreeImage_GetBPP(dibResized) == 32)) || (img_type == FIT_RGBF))
		{
			dibConv = dibResized;
		}
		else
		{
			dibConv = rgbf ? FreeImage_ConvertToRGBF(dibResized) : FreeImage_ConvertTo32Bits(dibResized);

			if (dibResized != dib)
			{
				FreeImage_Unload(dibResized);
			}

			if (!dibConv)
			{
				maxTexDim /= 2;

				while ((maxTexDim > pictureHeight) && (maxTexDim > pictureWidth))
				{
					maxTexDim /= 2;
				}

				continue;
			}
		}

		try
		{
			tex = new BaseTexture(FreeImage_GetWidth(dibConv), FreeImage_GetHeight(dibConv), rgbf ? RGB_FP : RGBA, rgbf ? false : has_alpha);

			success = true;
		}

		catch (...)
		{
			if (tex)
			{
				delete tex;
			}

			if (dibConv != dibResized)
			{
				FreeImage_Unload(dibConv);
			}
			else if (dibResized != dib)
			{
				FreeImage_Unload(dibResized);
			}

			maxTexDim /= 2;
			while ((maxTexDim > pictureHeight) && (maxTexDim > pictureWidth))
			{
				maxTexDim /= 2;
			}
		}
	}

	tex->m_realWidth = pictureWidth;
	tex->m_realHeight = pictureHeight;

	const BYTE* const __restrict psrc = FreeImage_GetBits(dibConv);
	BYTE* const __restrict pdst = tex->data();
	const int pitchdst = tex->pitch();
	const int pitchsrc = FreeImage_GetPitch(dibConv);
	const int height = tex->height();
	const int pitch = MIN(pitchsrc, pitchdst);

	for (int y = 0; y < height; ++y)
	{
		memcpy(pdst + (height - y - 1) * pitchdst, psrc + y * pitchsrc, pitch);
	}

	if (dibConv != dibResized)
	{
		FreeImage_Unload(dibConv);
	}
	else if (dibResized != dib)
	{
		FreeImage_Unload(dibResized);
	}

	FreeImage_Unload(dib);

	return tex;
}

BaseTexture* BaseTexture::CreateFromData(const void* data, const size_t size)
{
	FIMEMORY* const dataHandle = FreeImage_OpenMemory((BYTE*)data, (DWORD)size);
	if (!dataHandle)
	{
		return NULL;
	}
	const FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(dataHandle, (int)size);

	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
	{
		FIBITMAP* const dib = FreeImage_LoadFromMemory(fif, dataHandle, 0);
		FreeImage_CloseMemory(dataHandle);
		if (!dib)
		{
			return NULL;
		}

		BaseTexture* const mySurface = BaseTexture::CreateFromFreeImage(dib);

		return mySurface;
	}

	FreeImage_CloseMemory(dataHandle);
	return NULL;
}

BaseTexture::BaseTexture()
{
	m_width = 0;
	m_height = 0;
	m_realWidth = 0;
	m_realHeight = 0;
	m_format = RGBA;
	m_has_alpha = false;
}

BaseTexture::BaseTexture(const int w, const int h, const TextureFormat format, const bool has_alpha)
{
	m_width = w;
	m_height = h;

	m_data.resize((format == RGBA ? 4 : 3 * 4) * (w * h));

	m_realWidth = w;
	m_realHeight = h;
	m_format = format;
	m_has_alpha = has_alpha;
}

int BaseTexture::width() const
{
	return m_width;
}

int BaseTexture::height() const
{
	return m_height;
}

int BaseTexture::pitch() const
{
	return (m_format == RGBA ? 4 : 3 * 4) * m_width;
}

BYTE* BaseTexture::data()
{
	return m_data.data();
}
