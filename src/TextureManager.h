#pragma once

#include "BaseTexture.h"

#include <map>

class RenderDevice;

class TextureManager
{
public:
   TextureManager(RenderDevice& rd);
   ~TextureManager();

   //TODO: D3DTexture* LoadTexture(BaseTexture* memtex, const bool linearRGB);
   void SetDirty(BaseTexture* memtex);
   void UnloadTexture(BaseTexture* memtex);
   void UnloadAll();

private:
   struct TexInfo
   {
      // TODO: D3DTexture* d3dtex;
      int texWidth;
      int texHeight;
      bool dirty;
   };

   RenderDevice& m_rd;
   std::map<BaseTexture*, TexInfo> m_map;
   typedef std::map<BaseTexture*, TexInfo>::iterator Iter;
};