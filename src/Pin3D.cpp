#include "Pin3D.h"
#include "Material.h"
#include "Texture.h"
#include "Player.h"
#include "extern.h"

Pin3D::Pin3D()
{
}

Pin3D::~Pin3D()
{
}

void Pin3D::RenderPlayfieldGraphics(const bool depth_only)
{
	const Material* const mat = g_pplayer->m_ptable->GetMaterial(g_pplayer->m_ptable->m_playfieldMaterial);
	Texture* const pin = (depth_only && !mat->m_bOpacityActive) ? NULL : g_pplayer->m_ptable->GetImage(g_pplayer->m_ptable->m_image);
}
