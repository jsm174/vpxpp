#include "Pin3D.h"
#include "Material.h"
#include "Player.h"
#include "RegUtil.h"
#include "Texture.h"
#include "extern.h"

#include "Inlines.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/string.h>

inline D3DCOLOR COLORREF_to_D3DCOLOR(const COLORREF c)
{
	const COLORREF r = (c & 0x000000ff);
	const COLORREF g = (c & 0x0000ff00) >> 8;
	const COLORREF b = (c & 0x00ff0000) >> 16;

	return b | (g << 8) | (r << 16) | 0xff000000;
}

Pin3D::Pin3D()
{
	m_pd3dPrimaryDevice = NULL;
}

Pin3D::~Pin3D()
{
	// TODO: m_pd3dPrimaryDevice->SetZBuffer(NULL);
	// TODO: m_pd3dPrimaryDevice->FreeShader();

	m_pinballEnvTexture.FreeStuff();

	m_builtinEnvTexture.FreeStuff();

	m_aoDitherTexture.FreeStuff();
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
	}*/

	if (!m_pd3dPrimaryDevice->LoadShaders())
	{
		return E_FAIL;
	}

	const bool forceAniso = pRegUtil->LoadValueBoolWithDefault("Player", "ForceAnisotropicFiltering", true);
	m_pd3dPrimaryDevice->ForceAnisotropicFiltering(forceAniso);

	const bool compressTextures = pRegUtil->LoadValueBoolWithDefault("Player", "CompressTextures", false);
	m_pd3dPrimaryDevice->CompressTextures(compressTextures);

	m_pd3dPrimaryDevice->SetViewport(&m_viewPort);

	/* TODO: m_pd3dPrimaryDevice->GetBackBufferTexture()->GetSurfaceLevel(0, &m_pddsBackBuffer);

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

	// TODO: m_pd3dSecondaryDevice = m_pd3dPrimaryDevice;

	m_pinballEnvTexture.CreateFromResource("ball.bmp");
	m_aoDitherTexture.CreateFromResource("ao_dither.bmp");

	m_envTexture = g_pplayer->m_ptable->GetImage(g_pplayer->m_ptable->m_envImage);

	m_builtinEnvTexture.CreateFromResource("env_map.bmp");

	Texture* const envTex = m_envTexture ? m_envTexture : &m_builtinEnvTexture;

	const unsigned int envTexHeight = min(envTex->m_pdsBuffer->height(), 256) / 8;
	const unsigned int envTexWidth = envTexHeight * 2;

	m_envRadianceTexture = new BaseTexture(envTexWidth, envTexHeight, envTex->m_pdsBuffer->m_format, false);

	EnvmapPrecalc(envTex->m_pdsBuffer->data(),
				  envTex->m_pdsBuffer->width(),
				  envTex->m_pdsBuffer->height(),
				  m_envRadianceTexture->data(),
				  envTexWidth,
				  envTexHeight,
				  envTex->IsHDR());

	m_pd3dPrimaryDevice->m_texMan.SetDirty(m_envRadianceTexture);

	InitPrimaryRenderState();

	// TODO: SetPrimaryRenderTarget(m_pddsStatic, m_pddsStaticZ);

	return S_OK;
}

void Pin3D::EnvmapPrecalc(const void* __restrict envmap, const DWORD env_xres, const DWORD env_yres, void* const __restrict rad_envmap, const DWORD rad_env_xres, const DWORD rad_env_yres, const bool isHDR)
{
	if (isHDR && (env_xres > 64))
	{
		const float scale_factor = (float)env_xres * (float)(1.0 / 64.);
		const int xs = (int)(scale_factor * 0.5f + 0.5f);
		const void* const __restrict envmap2 = malloc(env_xres * env_yres * (isHDR ? 12 : 4));
		const void* const __restrict envmap3 = malloc(env_xres * env_yres * (isHDR ? 12 : 4));
		const float sigma = (scale_factor - 1.f) * 0.25f;
		float* const __restrict weights = (float*)malloc((xs * 2 + 1) * 4);
		for (int x = 0; x < (xs * 2 + 1); ++x)
		{
			weights[x] = (1.f / sqrtf((float)(2. * M_PI) * sigma * sigma)) * expf(-(float)((x - xs) * (x - xs)) / (2.f * sigma * sigma));
		}

		for (int y = 0; y < (int)env_yres; ++y)
		{
			for (int x = 0; x < (int)env_xres; ++x)
			{
				float sum[3] = {0.f, 0.f, 0.f};
				float sum_w = 0.f;
				const int yoffs = y * (env_xres * 3);
				for (int xt2 = 0; xt2 <= xs * 2; ++xt2)
				{
					int xt = xt2 + (x - xs);
					if (xt < 0)
					{
						xt += env_xres;
					}
					else if (xt >= (int)env_xres)
					{
						xt -= env_xres;
					}
					const float w = weights[xt2];
					const unsigned int offs = xt * 3 + yoffs;
					sum[0] += ((float*)envmap)[offs] * w;
					sum[1] += ((float*)envmap)[offs + 1] * w;
					sum[2] += ((float*)envmap)[offs + 2] * w;
					sum_w += w;
				}

				const unsigned int offs = (x + y * env_xres) * 3;
				const float inv_sum = 1.0f / sum_w;
				((float*)envmap2)[offs] = sum[0] * inv_sum;
				((float*)envmap2)[offs + 1] = sum[1] * inv_sum;
				((float*)envmap2)[offs + 2] = sum[2] * inv_sum;
			}
		}

		for (int y = 0; y < (int)env_yres; ++y)
		{
			for (int x = 0; x < (int)env_xres; ++x)
			{
				float sum[3] = {0.f, 0.f, 0.f};
				float sum_w = 0.f;
				const int yt_end = min(y + xs, (int)env_yres - 1) - (y - xs);
				int offs = x * 3 + max(y - xs, 0) * (env_xres * 3);
				for (int yt = max(y - xs, 0) - (y - xs); yt <= yt_end; ++yt, offs += env_xres * 3)
				{
					const float w = weights[yt];
					sum[0] += ((float*)envmap2)[offs] * w;
					sum[1] += ((float*)envmap2)[offs + 1] * w;
					sum[2] += ((float*)envmap2)[offs + 2] * w;
					sum_w += w;
				}

				offs = (x + y * env_xres) * 3;
				const float inv_sum = 1.0f / sum_w;
				((float*)envmap3)[offs] = sum[0] * inv_sum;
				((float*)envmap3)[offs + 1] = sum[1] * inv_sum;
				((float*)envmap3)[offs + 2] = sum[2] * inv_sum;
			}
		}

		envmap = envmap3;
		free((void*)envmap2);
		free(weights);
	}

	for (unsigned int y = 0; y < rad_env_yres; ++y)
	{
		for (unsigned int x = 0; x < rad_env_xres; ++x)
		{
			const float phi = (float)x / (float)rad_env_xres * (float)(2.0 * M_PI) + (float)M_PI;
			const float theta = (float)y / (float)rad_env_yres * (float)M_PI;
			const Vertex3Ds n(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta));

			float sum[3];
			sum[0] = sum[1] = sum[2] = 0.0f;

			const unsigned int num_samples = 4096;
			for (unsigned int s = 0; s < num_samples; ++s)
			{
				const Vertex3Ds l = rotate_to_vector_upper(cos_hemisphere_sample((float)s * (float)(1.0 / num_samples), radical_inverse(s)), n); // QMC hammersley point set

				const float u = atan2_approx_div2PI(l.y, l.x) + 0.5f;
				const float v = acos_approx_divPI(l.z);

				float r, g, b;
				if (isHDR)
				{
					unsigned int offs = ((int)(u * (float)env_xres) + (int)(v * (float)env_yres) * env_xres) * 3;
					if (offs >= env_yres * env_xres * 3)
						offs = 0;
					r = ((float*)envmap)[offs];
					g = ((float*)envmap)[offs + 1];
					b = ((float*)envmap)[offs + 2];
				}
				else
				{
					unsigned int offs = (int)(u * (float)env_xres) + (int)(v * (float)env_yres) * env_xres;
					if (offs >= env_yres * env_xres)
						offs = 0;
					const DWORD rgb = ((DWORD*)envmap)[offs];
					r = invGammaApprox((float)(rgb & 255) * (float)(1.0 / 255.0));
					g = invGammaApprox((float)(rgb & 65280) * (float)(1.0 / 65280.0));
					b = invGammaApprox((float)(rgb & 16711680) * (float)(1.0 / 16711680.0));
				}

				sum[0] += r;
				sum[1] += g;
				sum[2] += b;
			}

			sum[0] *= (float)(1.0 / num_samples);
			sum[1] *= (float)(1.0 / num_samples);
			sum[2] *= (float)(1.0 / num_samples);

			if (isHDR)
			{
				const unsigned int offs = (y * rad_env_xres + x) * 3;
				((float*)rad_envmap)[offs] = sum[0];
				((float*)rad_envmap)[offs + 1] = sum[1];
				((float*)rad_envmap)[offs + 2] = sum[2];
			}
			else
			{
				sum[0] = gammaApprox(sum[0]);
				sum[1] = gammaApprox(sum[1]);
				sum[2] = gammaApprox(sum[2]);
				((DWORD*)rad_envmap)[y * rad_env_xres + x] = ((int)(sum[0] * 255.0f)) | (((int)(sum[1] * 255.0f)) << 8) | (((int)(sum[2] * 255.0f)) << 16);
			}
		}
	}

	if (isHDR && (env_xres > 64))
	{
		free((void*)envmap);
	}
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

	m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_PROJECTION, &m_proj.m_matProj);
	m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &m_proj.m_matView);
	m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_WORLD, &m_proj.m_matWorld);

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
	DisableAlphaBlend();

	/* TODO: 
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
	// TODO: SetPrimaryTextureFilter(0, TEXTURE_MODE_TRILINEAR);

	PinTable* const ptable = g_pplayer->m_ptable;
	Texture* const pin = ptable->GetDecalsEnabled()
							 ? ptable->GetImage(ptable->m_BG_image[ptable->m_BG_current_set])
							 : NULL;
	if (pin)
	{
		m_pd3dPrimaryDevice->Clear(0, NULL, ClearType::ZBUFFER, 0, 1.0f, 0L);

		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_FALSE);
		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZENABLE, RenderDevice::RS_FALSE);

		if (g_pplayer->m_ptable->m_tblMirrorEnabled ^ g_pplayer->m_ptable->m_reflectionEnabled)
		{
			m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
		}

		DisableAlphaBlend();

		// TODO: g_pplayer->Spritedraw(0.f, 0.f, 1.f, 1.f, 0xFFFFFFFF, pin, ptable->m_ImageBackdropNightDay ? sqrtf(g_pplayer->m_globalEmissionScale) : 1.0f, true);

		if (g_pplayer->m_ptable->m_tblMirrorEnabled ^ g_pplayer->m_ptable->m_reflectionEnabled)
		{
			m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);
		}

		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZENABLE, RenderDevice::RS_TRUE);
		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
	}
	else
	{
		const D3DCOLOR d3dcolor = COLORREF_to_D3DCOLOR(ptable->m_colorbackdrop);
		m_pd3dPrimaryDevice->Clear(0, NULL, ClearType::TARGET | ClearType::ZBUFFER, d3dcolor, 1.0f, 0L);
	}
}

void Pin3D::EnableAlphaBlend(const bool additiveBlending, const bool set_dest_blend, const bool set_blend_op) const
{
	m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ALPHABLENDENABLE, RenderDevice::RS_TRUE);
	m_pd3dPrimaryDevice->SetRenderState(RenderDevice::SRCBLEND, RenderDevice::SRC_ALPHA);

	if (set_dest_blend)
	{
		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::DESTBLEND, additiveBlending ? RenderDevice::ONE : RenderDevice::INVSRC_ALPHA);
	}
	if (set_blend_op)
	{
		m_pd3dPrimaryDevice->SetRenderState(RenderDevice::BLENDOP, RenderDevice::BLENDOP_ADD);
	}
}

void Pin3D::DisableAlphaBlend() const
{
	m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ALPHABLENDENABLE, RenderDevice::RS_FALSE);
}

void Pin3D::Flip(const bool vsync)
{
	m_pd3dPrimaryDevice->Flip(vsync);
}
