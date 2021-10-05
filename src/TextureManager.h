#pragma once

#include "BaseTexture.h"

#include <bgfx/bgfx.h>

#include <map>

class RenderDevice;

class TextureManager
{
public:
	TextureManager(RenderDevice& rd);
	~TextureManager();
	bgfx::TextureHandle LoadTexture(BaseTexture* memtex, const bool linearRGB);
	void SetDirty(BaseTexture* memtex);
	void UnloadTexture(BaseTexture* memtex);
	void UnloadAll();

private:
	struct TexInfo
	{
		bgfx::TextureHandle d3dtex;
		int texWidth;
		int texHeight;
		bool dirty;
	};

	RenderDevice& m_rd;
	std::map<BaseTexture*, TexInfo> m_map;
	typedef std::map<BaseTexture*, TexInfo>::iterator Iter;
};