#include "Pin3D.h"
#include "Material.h"
#include "Player.h"
#include "RegUtil.h"
#include "Texture.h"
#include "extern.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/string.h>

Pin3D::Pin3D()
{
	m_pd3dPrimaryDevice = NULL;
}

Pin3D::~Pin3D()
{
	// TODO: m_pd3dPrimaryDevice->SetZBuffer(NULL);
	// TODO: m_pd3dPrimaryDevice->FreeShader();
}

HRESULT Pin3D::InitPrimary(const bool fullScreen, const int colordepth, int& refreshrate, const int VSync,
						   const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen,
						   const bool useAO, const bool ss_refl)
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

	m_pd3dPrimaryDevice = new RenderDevice(
		/*g_pplayer->GetHwnd(),*/ m_viewPort.Width, m_viewPort.Height, fullScreen, colordepth, VSync, useAA, stereo3D,
		FXAA, sharpen, ss_refl, /*g_pplayer->m_useNvidiaApi,*/ g_pplayer->m_disableDWM, g_pplayer->m_BWrendering);

	/* 	try
	{
		m_pd3dPrimaryDevice->CreateDevice(refreshrate, adapter);
	}
	catch (...)
	{
		return E_FAIL;
	}

	if (!m_pd3dPrimaryDevice->LoadShaders())
	{
		return E_FAIL;
	}

	const bool forceAniso = LoadValueBoolWithDefault("Player", "ForceAnisotropicFiltering", true);
	m_pd3dPrimaryDevice->ForceAnisotropicFiltering(forceAniso);

	const bool compressTextures = LoadValueBoolWithDefault("Player", "CompressTextures", false);
	m_pd3dPrimaryDevice->CompressTextures(compressTextures);

	m_pd3dPrimaryDevice->SetViewport(&m_viewPort);

	m_pd3dPrimaryDevice->GetBackBufferTexture()->GetSurfaceLevel(0, &m_pddsBackBuffer);

	m_pddsStatic = m_pd3dPrimaryDevice->DuplicateRenderTarget(m_pddsBackBuffer);
	if (!m_pddsStatic)
	{
		return E_FAIL;
	}

	m_pddsZBuffer = m_pd3dPrimaryDevice->AttachZBufferTo(m_pddsBackBuffer);
	m_pddsStaticZ = m_pd3dPrimaryDevice->AttachZBufferTo(m_pddsStatic);
	if (!m_pddsZBuffer || !m_pddsStaticZ)
	{
		return E_FAIL;
	}

	if (m_pd3dPrimaryDevice->DepthBufferReadBackAvailable() && (stereo3D || useAO || ss_refl))
	{
		m_pdds3DZBuffer = !m_pd3dPrimaryDevice->m_useNvidiaApi ? (D3DTexture*)m_pd3dPrimaryDevice->AttachZBufferTo(m_pddsBackBuffer) : m_pd3dPrimaryDevice->DuplicateDepthTexture((RenderTarget*)m_pddsZBuffer);

		if (!m_pdds3DZBuffer)
		{
			ShowError("Unable to create depth texture!\r\nTry to (un)set \"Alternative Depth Buffer processing\" in the video options!\r\nOr disable Ambient Occlusion, 3D stereo and/or ScreenSpace Reflections!");
			return E_FAIL;
		}
	}

	if (m_pd3dPrimaryDevice->DepthBufferReadBackAvailable() && useAO)
	{
		const HRESULT hr1 = m_pd3dPrimaryDevice->GetCoreDevice()->CreateTexture(m_viewPort.Width, m_viewPort.Height, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT)colorFormat::GREY8, (D3DPOOL)memoryPool::DEFAULT, &m_pddsAOBackTmpBuffer, NULL);
		const HRESULT hr2 = m_pd3dPrimaryDevice->GetCoreDevice()->CreateTexture(m_viewPort.Width, m_viewPort.Height, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT)colorFormat::GREY8, (D3DPOOL)memoryPool::DEFAULT, &m_pddsAOBackBuffer, NULL);
		if (FAILED(hr1) || FAILED(hr2) || !m_pddsAOBackBuffer || !m_pddsAOBackTmpBuffer)
		{
			ShowError("Unable to create AO buffers!\r\nPlease disable Ambient Occlusion.\r\nOr try to (un)set \"Alternative Depth Buffer processing\" in the video options!");
			return E_FAIL;
		}
	} */

	return S_OK;
}

HRESULT Pin3D::InitPin3D(const bool fullScreen, const int width, const int height, const int colordepth,
						 int& refreshrate, const int VSync, const bool useAA, const bool stereo3D,
						 const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl)
{
	m_viewPort.X = 0;
	m_viewPort.Y = 0;
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinZ = 0.0f;
	m_viewPort.MaxZ = 1.0f;

	HRESULT hr =
		InitPrimary(fullScreen, colordepth, refreshrate, VSync, useAA, stereo3D, FXAA, sharpen, useAO, ss_refl);

	if (hr != S_OK)
	{
		return E_FAIL;
	}

	/* 	TODO: m_pd3dSecondaryDevice = m_pd3dPrimaryDevice;

		m_pinballEnvTexture.CreateFromResource(IDB_BALL);
		m_aoDitherTexture.CreateFromResource(IDB_AO_DITHER);

		m_envTexture = g_pplayer->m_ptable->GetImage(g_pplayer->m_ptable->m_envImage);
		m_builtinEnvTexture.CreateFromResource(IDB_ENV);

		Texture* const envTex = m_envTexture ? m_envTexture : &m_builtinEnvTexture;

		const unsigned int envTexHeight = min(envTex->m_pdsBuffer->height(), 256) / 8;
		const unsigned int envTexWidth = envTexHeight * 2;

		m_envRadianceTexture = new BaseTexture(envTexWidth, envTexHeight, envTex->m_pdsBuffer->m_format, false);

		EnvmapPrecalc(envTex->m_pdsBuffer->data(), envTex->m_pdsBuffer->width(), envTex->m_pdsBuffer->height(),
					  m_envRadianceTexture->data(), envTexWidth, envTexHeight, envTex->IsHDR());

		m_pd3dPrimaryDevice->m_texMan.SetDirty(m_envRadianceTexture);
	 */

	InitPrimaryRenderState();

	// TODOSetPrimaryRenderTarget(m_pddsStatic, m_pddsStaticZ);

	return S_OK;
}

void Pin3D::InitLayout(const bool FSS_mode, const float xpixoff, const float ypixoff)
{
	const float rotation = ANGTORAD(g_pplayer->m_ptable->m_BG_rotation[g_pplayer->m_ptable->m_BG_current_set]);
	float inclination = ANGTORAD(g_pplayer->m_ptable->m_BG_inclination[g_pplayer->m_ptable->m_BG_current_set]);
	const float FOV = (g_pplayer->m_ptable->m_BG_FOV[g_pplayer->m_ptable->m_BG_current_set] < 1.0f)
						  ? 1.0f
						  : g_pplayer->m_ptable->m_BG_FOV[g_pplayer->m_ptable->m_BG_current_set];

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

	m_proj.FitCameraToVertices(vvertex3D, aspect, rotation, inclination, FOV,
							   g_pplayer->m_ptable->m_BG_xlatez[g_pplayer->m_ptable->m_BG_current_set],
							   g_pplayer->m_ptable->m_BG_layback[g_pplayer->m_ptable->m_BG_current_set]);

	m_proj.m_matWorld.SetIdentity();

	m_proj.m_matView.RotateXMatrix((float)M_PI);

	m_proj.ScaleView(g_pplayer->m_ptable->m_BG_scalex[g_pplayer->m_ptable->m_BG_current_set],
					 g_pplayer->m_ptable->m_BG_scaley[g_pplayer->m_ptable->m_BG_current_set], 1.0f);

	m_proj.TranslateView(
		g_pplayer->m_ptable->m_BG_xlatex[g_pplayer->m_ptable->m_BG_current_set] - m_proj.m_vertexcamera.x + camx,
		g_pplayer->m_ptable->m_BG_xlatey[g_pplayer->m_ptable->m_BG_current_set] - m_proj.m_vertexcamera.y + camy,
		-m_proj.m_vertexcamera.z + camz);

	if (g_pplayer->m_cameraMode &&
		(g_pplayer->m_ptable->m_BG_current_set == 0 || g_pplayer->m_ptable->m_BG_current_set == 2))
	{
		m_proj.RotateView(inclination, 0, rotation);
	}
	else
	{
		m_proj.RotateView(0, 0, rotation);
		m_proj.RotateView(inclination, 0, 0);
	}

	m_proj.MultiplyView(
		ComputeLaybackTransform(g_pplayer->m_ptable->m_BG_layback[g_pplayer->m_ptable->m_BG_current_set]));

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
		projTrans.SetTranslation((float)((double)xpixoff / (double)m_viewPort.Width),
								 (float)((double)ypixoff / (double)m_viewPort.Height), 0.f);
		projTrans.Multiply(m_proj.m_matProj, m_proj.m_matProj);
	}

	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_PROJECTION, &m_proj.m_matProj);
	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_VIEW, &m_proj.m_matView);
	m_pd3dPrimaryDevice->SetTransform(TRANSFORMSTATE_WORLD, &m_proj.m_matWorld);

	// Set view and projection matrix for view 1.

	float tmpView[16];
	tmpView[0] = m_proj.m_matView.m[0][1];
	tmpView[1] = m_proj.m_matView.m[0][1];
	tmpView[2] = m_proj.m_matView.m[0][2];
	tmpView[3] = m_proj.m_matView.m[0][3];
	tmpView[4] = m_proj.m_matView.m[1][0];
	tmpView[5] = m_proj.m_matView.m[1][1];
	tmpView[6] = m_proj.m_matView.m[1][2];
	tmpView[7] = m_proj.m_matView.m[1][3];
	tmpView[8] = m_proj.m_matView.m[2][0];
	tmpView[9] = m_proj.m_matView.m[2][1];
	tmpView[10] = m_proj.m_matView.m[2][2];
	tmpView[11] = m_proj.m_matView.m[2][3];
	tmpView[12] = m_proj.m_matView.m[3][0];
	tmpView[13] = m_proj.m_matView.m[3][1];
	tmpView[14] = m_proj.m_matView.m[3][2];
	tmpView[15] = m_proj.m_matView.m[3][3];

	float tmpProj[16];
	tmpProj[0] = m_proj.m_matProj.m[0][0];
	tmpProj[1] = m_proj.m_matProj.m[0][1];
	tmpProj[2] = m_proj.m_matProj.m[0][2];
	tmpProj[3] = m_proj.m_matProj.m[0][3];
	tmpProj[4] = m_proj.m_matProj.m[1][0];
	tmpProj[5] = m_proj.m_matProj.m[1][1];
	tmpProj[6] = m_proj.m_matProj.m[1][2];
	tmpProj[7] = m_proj.m_matProj.m[1][3];
	tmpProj[8] = m_proj.m_matProj.m[2][0];
	tmpProj[9] = m_proj.m_matProj.m[2][1];
	tmpProj[10] = m_proj.m_matProj.m[2][2];
	tmpProj[11] = m_proj.m_matProj.m[2][3];
	tmpProj[12] = m_proj.m_matProj.m[3][0];
	tmpProj[13] = m_proj.m_matProj.m[3][1];
	tmpProj[14] = m_proj.m_matProj.m[3][2];
	tmpProj[15] = m_proj.m_matProj.m[3][3];

	bgfx::setViewTransform(0, tmpView, tmpProj);

	m_proj.CacheTransform();

	// TODO: InitLights();
}

void Pin3D::InitPlayfieldGraphics()
{
}

void Pin3D::RenderPlayfieldGraphics(const bool depth_only)
{
	const Material* const mat = g_pplayer->m_ptable->GetMaterial(g_pplayer->m_ptable->m_playfieldMaterial);
	Texture* const pin =
		(depth_only && !mat->m_bOpacityActive) ? NULL : g_pplayer->m_ptable->GetImage(g_pplayer->m_ptable->m_image);

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

void Pin3D::InitRenderState(RenderDevice* const pd3dDevice)
{
	/* TODO: DisableAlphaBlend();

		pd3dDevice->SetRenderState(RenderDevice::LIGHTING, RenderDevice::RS_FALSE);

		pd3dDevice->SetRenderState(RenderDevice::ZENABLE, RenderDevice::RS_TRUE);
		pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
		pd3dDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

		pd3dDevice->SetRenderState(RenderDevice::CLIPPING, RenderDevice::RS_FALSE);
		pd3dDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, 0);

		pd3dDevice->SetTextureAddressMode(0, RenderDevice::TEX_CLAMP);
		pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		SetTextureFilter(pd3dDevice, 0, TEXTURE_MODE_TRILINEAR);

		pd3dDevice->SetTextureAddressMode(4, RenderDevice::TEX_CLAMP);
		SetTextureFilter(pd3dDevice, 4, TEXTURE_MODE_TRILINEAR); */
}

void Pin3D::InitPrimaryRenderState()
{
	InitRenderState(m_pd3dPrimaryDevice);
}

void Pin3D::DrawBackground()
{
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
