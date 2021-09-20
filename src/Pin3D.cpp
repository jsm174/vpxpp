#include "Pin3D.h"
#include "Material.h"
#include "Player.h"
#include "RegUtil.h"
#include "Texture.h"
#include "extern.h"

Pin3D::Pin3D()
{
	m_pd3dPrimaryDevice = NULL;
}

Pin3D::~Pin3D()
{
	// TODO: m_pd3dPrimaryDevice->SetZBuffer(NULL);
	// TODO: m_pd3dPrimaryDevice->FreeShader();
}

HRESULT Pin3D::InitPrimary(const bool fullScreen, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl)
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	const int display = g_pvp->m_primaryDisplay ? 0 : pRegUtil->LoadValueIntWithDefault("Player", "Display", 0);

	// TODO: std::vector<DisplayConfig> displays;
	// getDisplayList(displays);

	// int adapter = 0;

	// for (std::vector<DisplayConfig>::iterator dispConf = displays.begin(); dispConf != displays.end(); ++dispConf)
	// {
	// 	if (display == dispConf->display)
	// 	{
	// 		adapter = dispConf->adapter;
	// 	}
	// }

	m_pd3dPrimaryDevice = new RenderDevice(/*g_pplayer->GetHwnd(),*/ m_viewPort.Width, m_viewPort.Height, fullScreen, colordepth, VSync, useAA, stereo3D, FXAA, sharpen, ss_refl, /*g_pplayer->m_useNvidiaApi,*/ g_pplayer->m_disableDWM, g_pplayer->m_BWrendering);

	return S_OK;
}

HRESULT Pin3D::InitPin3D(const bool fullScreen, const int width, const int height, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl)
{
	m_viewPort.X = 0;
	m_viewPort.Y = 0;
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinZ = 0.0f;
	m_viewPort.MaxZ = 1.0f;

	HRESULT hr = InitPrimary(fullScreen, colordepth, refreshrate, VSync, useAA, stereo3D, FXAA, sharpen, useAO, ss_refl);

	if (hr != S_OK)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Pin3D::InitLayout(const bool FSS_mode, const float xpixoff, const float ypixoff)
{
	const float rotation = ANGTORAD(g_pplayer->m_ptable->m_BG_rotation[g_pplayer->m_ptable->m_BG_current_set]);
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

	const float camx = m_cam.x;
	const float camy = m_cam.y + (FSS_mode ? 500.0f : 0.f);
	float camz = m_cam.z;
	const float inc = m_inc + (FSS_mode ? 0.2f : 0.f);

	if (FSS_mode)
	{
		const int width = 1024; // TODO: GetSystemMetrics(SM_CXSCREEN);
		const int height = 768; // TODO: GetSystemMetrics(SM_CYSCREEN);

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

	// TODO: m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_PROJECTION, &m_proj.m_matProj);
	// TODO: m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_VIEW, &m_proj.m_matView);
	// TODO: m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_WORLD, &m_proj.m_matWorld);

	m_proj.CacheTransform();

	// TODO: InitLights();
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

// https://github.com/wine-mirror/wine/blob/master/dlls/d3dx9_36/math.c#L479

D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pout, float fovy, float aspect, float zn, float zf)
{
	D3DXMatrixIdentity(pout);
	pout->m[0][0] = 1.0f / (aspect * tanf(fovy / 2.0f));
	pout->m[1][1] = 1.0f / tanf(fovy / 2.0f);
	pout->m[2][2] = zf / (zf - zn);
	pout->m[2][3] = 1.0f;
	pout->m[3][2] = (zf * zn) / (zn - zf);
	pout->m[3][3] = 0.0f;
	return pout;
}

// https://github.com/wine-mirror/wine/blob/master/include/d3dx9math.inl#L1192

D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pout)
{
	if (!pout)
		return NULL;
	(*pout).m[0][1] = 0.0f;
	(*pout).m[0][2] = 0.0f;
	(*pout).m[0][3] = 0.0f;
	(*pout).m[1][0] = 0.0f;
	(*pout).m[1][2] = 0.0f;
	(*pout).m[1][3] = 0.0f;
	(*pout).m[2][0] = 0.0f;
	(*pout).m[2][1] = 0.0f;
	(*pout).m[2][3] = 0.0f;
	(*pout).m[3][0] = 0.0f;
	(*pout).m[3][1] = 0.0f;
	(*pout).m[3][2] = 0.0f;
	(*pout).m[0][0] = 1.0f;
	(*pout).m[1][1] = 1.0f;
	(*pout).m[2][2] = 1.0f;
	(*pout).m[3][3] = 1.0f;
	return pout;
}