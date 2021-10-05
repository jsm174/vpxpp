#pragma once

#include "misc.h"

#include "FreeImage.h"

#include <string>
#include <vector>

class BaseTexture
{
public:
	enum Format
	{
		RGBA,
		RGB_FP
	};

	// TODO: static BaseTexture* CreateFromHBitmap(const HBITMAP hbm);
	static BaseTexture* CreateFromFile(const std::string& filename);
	static BaseTexture* CreateFromFreeImage(FIBITMAP* dib);
	static BaseTexture* CreateFromData(const void* data, const size_t size);

	BaseTexture();
	BaseTexture(const int w, const int h, const Format format, const bool has_alpha);

	int width() const;
	int height() const;
	int pitch() const;
	BYTE* data();

	std::vector<BYTE> m_data;
	int m_realWidth;
	int m_realHeight;
	Format m_format;
	bool m_has_alpha;

private:
	int m_width;
	int m_height;
};