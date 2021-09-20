#include "TextureManager.h"

TextureManager::TextureManager(RenderDevice& rd) : m_rd(rd)
{
}

TextureManager::~TextureManager()
{
	UnloadAll();
}

// TODO: D3DTexture* TextureManager::LoadTexture(BaseTexture* memtex, const bool linearRGB)
// {
// 	const Iter it = m_map.find(memtex);
// 	if (it == m_map.end())
// 	{
// 		TexInfo texinfo;
// 		texinfo.d3dtex = m_rd.UploadTexture(memtex, &texinfo.texWidth, &texinfo.texHeight, linearRGB);
// 		if (!texinfo.d3dtex)
// 		{
// 			return NULL;
// 		}
// 		texinfo.dirty = false;
// 		m_map[memtex] = texinfo;
// 		return texinfo.d3dtex;
// 	}
// 	else
// 	{
// 		if (it->second.dirty)
// 		{
// 			m_rd.UpdateTexture(it->second.d3dtex, memtex, linearRGB);
// 			it->second.dirty = false;
// 		}
// 		return it->second.d3dtex;
// 	}
// }

void TextureManager::SetDirty(BaseTexture* memtex)
{
	const Iter it = m_map.find(memtex);
	if (it != m_map.end())
	{
		it->second.dirty = true;
	}
}

void TextureManager::UnloadTexture(BaseTexture* memtex)
{
	const Iter it = m_map.find(memtex);
	if (it != m_map.end())
	{
		// TODO: SAFE_RELEASE(it->second.d3dtex);
		m_map.erase(it);
	}
}

void TextureManager::UnloadAll()
{
	for (Iter it = m_map.begin(); it != m_map.end(); ++it)
	{
		// TODO: SAFE_RELEASE(it->second.d3dtex);
	}

	m_map.clear();
}