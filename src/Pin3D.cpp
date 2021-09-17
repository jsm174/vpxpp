#include "Pin3D.h"
#include "Material.h"
#include "Player.h"
#include "Texture.h"
#include "extern.h"

Pin3D::Pin3D()
{
}

Pin3D::~Pin3D()
{
}

HRESULT Pin3D::InitPin3D(const bool fullScreen, const int width, const int height, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl)
{
	return S_OK;
}

void Pin3D::InitLayout(const bool FSS_mode, const float xpixoff, const float ypixoff)
{
/* 	const float rotation = ANGTORAD(g_pplayer->m_ptable->m_BG_rotation[g_pplayer->m_ptable->m_BG_current_set]);
	float inclination = ANGTORAD(g_pplayer->m_ptable->m_BG_inclination[g_pplayer->m_ptable->m_BG_current_set]);
	const float FOV = (g_pplayer->m_ptable->m_BG_FOV[g_pplayer->m_ptable->m_BG_current_set] < 1.0f) ? 1.0f : g_pplayer->m_ptable->m_BG_FOV[g_pplayer->m_ptable->m_BG_current_set];

	std::vector<Vertex3Ds> vvertex3D;
	for (size_t i = 0; i < g_pplayer->m_ptable->m_vedit.size(); ++i)
	{
		g_pplayer->m_ptable->m_vedit[i]->GetBoundingVertices(vvertex3D);
	}

	m_proj.m_rcviewport.left = 0;
	m_proj.m_rcviewport.top = 0;
	m_proj.m_rcviewport.right = m_viewPort.Width;
	m_proj.m_rcviewport.bottom = m_viewPort.Height;

	const float aspect = ((float)m_viewPort.Width) / ((float)m_viewPort.Height); //(float)(4.0/3.0);

	// next 4 def values for layout portrait(game vert) in landscape(screen horz)
	// for FSS, force an offset to camy which drops the table down 1/3 of the way.
	// some values to camy have been commented out because I found the default value
	// better and just modify the camz and keep the table design inclination
	// within 50-60 deg and 40-50 FOV in editor.
	// these values were tested against all known video modes upto 1920x1080
	// in landscape and portrait on the display
	const float camx = m_cam.x;
	const float camy = m_cam.y + (FSS_mode ? 500.0f : 0.f);
	float camz = m_cam.z;
	const float inc = m_inc + (FSS_mode ? 0.2f : 0.f);

	if (FSS_mode)
	{
		const int width = GetSystemMetrics(SM_CXSCREEN);
		const int height = GetSystemMetrics(SM_CYSCREEN);

		if ((m_viewPort.Width > m_viewPort.Height) && (height < width))
		{
			if (aspect > 1.6f)
			{
				camz -= 1170.0f;
			}
			else if (aspect > 1.5f)
			{
				camz -= 1070.0f;
			}
			else if (aspect > 1.4f)
			{
				camz -= 900.0f;
			}
			else if (aspect > 1.3f)
			{
				camz -= 820.0f;
			}
			else
			{
				camz -= 800.0f;
			}
		}
		else
		{
			if (height > width)
			{
				if (aspect > 0.6f)
				{
					camz += 10.0f;
				}
				else if (aspect > 0.5f)
				{
					camz += 300.0f;
				}
				else
				{
					camz += 300.0f;
				}
			}
			else
			{
			}
		}
	}

	inclination += inc;

	m_proj.FitCameraToVertices(vvertex3D, aspect, rotation, inclination, FOV, g_pplayer->m_ptable->m_BG_xlatez[g_pplayer->m_ptable->m_BG_current_set], g_pplayer->m_ptable->m_BG_layback[g_pplayer->m_ptable->m_BG_current_set]);

	m_proj.m_matWorld.SetIdentity();

	m_proj.m_matView.RotateXMatrix((float)M_PI);
	m_proj.ScaleView(g_pplayer->m_ptable->m_BG_scalex[g_pplayer->m_ptable->m_BG_current_set], g_pplayer->m_ptable->m_BG_scaley[g_pplayer->m_ptable->m_BG_current_set], 1.0f);

	m_proj.TranslateView(g_pplayer->m_ptable->m_BG_xlatex[g_pplayer->m_ptable->m_BG_current_set] - m_proj.m_vertexcamera.x + camx, g_pplayer->m_ptable->m_BG_xlatey[g_pplayer->m_ptable->m_BG_current_set] - m_proj.m_vertexcamera.y + camy, -m_proj.m_vertexcamera.z + camz);
	
	if (g_pplayer->m_cameraMode && (g_pplayer->m_ptable->m_BG_current_set == 0 || g_pplayer->m_ptable->m_BG_current_set == 2))
	{
		m_proj.RotateView(inclination, 0, rotation);
	}
	else
	{
		m_proj.RotateView(0, 0, rotation);
		m_proj.RotateView(inclination, 0, 0);
	}

	m_proj.MultiplyView(ComputeLaybackTransform(g_pplayer->m_ptable->m_BG_layback[g_pplayer->m_ptable->m_BG_current_set]));

	m_proj.ComputeNearFarPlane(vvertex3D);

	if (fabsf(inclination) < 0.0075f)
	{
		m_proj.m_rzfar += 10.f;
	}
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, ANGTORAD(FOV), aspect, m_proj.m_rznear, m_proj.m_rzfar);
	memcpy(m_proj.m_matProj.m, proj.m, sizeof(float) * 4 * 4);

	if (xpixoff != 0.f || ypixoff != 0.f)
	{
		Matrix3D projTrans;
		projTrans.SetTranslation((float)((double)xpixoff / (double)m_viewPort.Width), (float)((double)ypixoff / (double)m_viewPort.Height), 0.f);
		projTrans.Multiply(m_proj.m_matProj, m_proj.m_matProj);
	}

	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_PROJECTION, &m_proj.m_matProj);
	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_VIEW, &m_proj.m_matView);
	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_WORLD, &m_proj.m_matWorld);

	m_proj.CacheTransform();

	InitLights(); */
}

void Pin3D::RenderPlayfieldGraphics(const bool depth_only)
{
	const Material* const mat = g_pplayer->m_ptable->GetMaterial(g_pplayer->m_ptable->m_playfieldMaterial);
	Texture* const pin = (depth_only && !mat->m_bOpacityActive) ? NULL : g_pplayer->m_ptable->GetImage(g_pplayer->m_ptable->m_image);

	if (depth_only)
	{
		if (pin)
		{
		}
		else
		{
		}
	}
}
