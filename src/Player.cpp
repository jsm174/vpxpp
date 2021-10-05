#include "Player.h"
#include "DispReel.h"
#include "Inlines.h"
#include "LightSeq.h"
#include "RegUtil.h"

#include "imgui.h"
#include <iostream>

unsigned int Player::material_flips = 0;
unsigned int Player::stats_drawn_static_triangles = 0;

Vertex3Ds g_viewDir;

static bool CompareHitableDepth(IHitable* h1, IHitable* h2)
{
	return h1->GetDepth(g_viewDir) >= h2->GetDepth(g_viewDir);
}

static bool CompareHitableDepthReverse(IHitable* h1, IHitable* h2)
{
	return h1->GetDepth(g_viewDir) < h2->GetDepth(g_viewDir);
}

static bool CompareHitableMaterial(IHitable* h1, IHitable* h2)
{
	return h1->GetMaterialID() < h2->GetMaterialID();
}

static bool CompareHitableImage(IHitable* h1, IHitable* h2)
{
	return h1->GetImageID() < h2->GetImageID();
}

static bool CompareHitableDepthInverse(IHitable* h1, IHitable* h2)
{
	return h1->GetDepth(g_viewDir) <= h2->GetDepth(g_viewDir);
}

inline void gaussianDistribution(float& u1, float& u2, const float sigma = 3.f, const float mu = 0.f)
{
	const float root4 = sqrtf(sqrtf(1.f - u1));
	const float tmp0 = sigma * sqrtf(9.f - 9.f * root4);

	const float tmp1 = (float)(2.0 * M_PI) * u2;
	u1 = tmp0 * cosf(tmp1) + mu;
	u2 = tmp0 * sinf(tmp1) + mu;
}

Player::Player(const bool cameraMode, PinTable* const pPinTable)
{
	m_isRenderingStatic = false;

	m_cameraMode = cameraMode;
	m_ptable = pPinTable;

	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_VSync = pRegUtil->LoadValueIntWithDefault("Player", "AdaptiveVSync", 0);
	m_FXAA = pRegUtil->LoadValueIntWithDefault("Player", "FXAA", Standard_FXAA);
	m_sharpen = pRegUtil->LoadValueIntWithDefault("Player", "Sharpen", 0);
	m_reflectionForBalls = pRegUtil->LoadValueBoolWithDefault("Player", "BallReflection", true);
	m_AA = pRegUtil->LoadValueBoolWithDefault("Player", "USEAA", false);
	m_disableAO = pRegUtil->LoadValueBoolWithDefault("Player", "DisableAO", false);
	m_ss_refl = pRegUtil->LoadValueBoolWithDefault("Player", "SSRefl", false);
	m_pf_refl = pRegUtil->LoadValueBoolWithDefault("Player", "PFRefl", true);
	m_stereo3D = pRegUtil->LoadValueIntWithDefault("Player", "Stereo3D", 0);

	m_disableDWM = pRegUtil->LoadValueBoolWithDefault("Player", "DisableDWM", false);
	m_BWrendering = pRegUtil->LoadValueIntWithDefault("Player", "BWRendering", 0);

	m_minphyslooptime = min(pRegUtil->LoadValueIntWithDefault("Player", "MinPhysLoopTime", 0), 1000);

	m_showFPS = 0;
	m_LastKnownGoodCounter = 0;

	m_overall_frames = 0;
}

Player::~Player()
{
}

void Player::PreCreate()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_fullScreen = pRegUtil->LoadValueBoolWithDefault("Player", "FullScreen", false);

	m_width = pRegUtil->LoadValueIntWithDefault("Player", "Width", m_fullScreen ? -1 : DEFAULT_PLAYER_WIDTH);
	m_height = pRegUtil->LoadValueIntWithDefault("Player", "Height", m_width * 9 / 16);

	if (m_fullScreen)
	{
		m_refreshrate = pRegUtil->LoadValueIntWithDefault("Player", "RefreshRate", 0);
	}
	else
	{
		m_refreshrate = 0;
	}
}

void Player::OnInitialUpdate()
{
	Init();
}

void Player::InitFPS()
{
	m_lastfpstime = m_time_msec;
	m_cframes = 0;
	m_fps = 0.0f;
	m_fpsAvg = 0.0f;
	m_fpsCount = 0;
	m_total = 0;
	m_count = 0;
	m_max = 0;
	m_max_total = 0;
	m_lastMaxChangeTime = 0;
	m_lastTime_usec = 0;

	m_phys_total = 0;
	m_phys_max = 0;
	m_phys_max_total = 0;
	m_phys_max_iterations = 0;
	m_phys_total_iterations = 0;

	m_script_total = 0;
	m_script_max = 0;
	m_script_max_total = 0;
}

HRESULT Player::Init()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_globalEmissionScale = m_ptable->m_globalEmissionScale;

	const int vsync = (m_ptable->m_TableAdaptiveVSync == -1) ? m_VSync : m_ptable->m_TableAdaptiveVSync;
	const bool useAA = (m_AA && (m_ptable->m_useAA == -1)) || (m_ptable->m_useAA == 1);
	const unsigned int FXAA = (m_ptable->m_useFXAA == -1) ? m_FXAA : m_ptable->m_useFXAA;
	const bool ss_refl = (m_ss_refl && (m_ptable->m_useSSR == -1)) || (m_ptable->m_useSSR == 1);

	const int colordepth = pRegUtil->LoadValueIntWithDefault("Player", "ColorDepth", 32);

	const HRESULT hr = m_pin3d.InitPin3D(m_fullScreen,
										 m_width,
										 m_height,
										 colordepth,
										 m_refreshrate,
										 vsync,
										 useAA,
										 !!m_stereo3D,
										 FXAA,
										 !!m_sharpen,
										 !m_disableAO,
										 ss_refl);

	if (hr != S_OK)
	{
		std::cout << "InitPin3D Error" << std::endl;

		return hr;
	}

	m_pin3d.InitLayout(m_ptable->m_BG_enable_FSS); // DELETEME: Line 1233

	InitFPS();
	m_showFPS = 0;

	for (size_t i = 0; i < m_ptable->m_vedit.size(); i++) // DELETEME: Line 1304
	{
		IEditable* const pe = m_ptable->m_vedit[i];
		IHitable* const ph = pe->GetIHitable();

		if (ph)
		{
			const size_t currentsize = m_vho.size();
			ph->GetHitShapes(m_vho);
			const size_t newsize = m_vho.size();
			for (size_t hitloop = currentsize; hitloop < newsize; hitloop++)
			{
				// TODO: m_vho[hitloop]->m_pfedebug = pe->GetIFireEvents();
			}

			// TODO: ph->GetTimers(m_vht);

			m_vhitables.push_back(ph);

			if (pe->GetItemType() == eItemDispReel)
			{
				DispReel* const dispReel = (DispReel*)pe;
				m_vanimate.push_back(&dispReel->m_dispreelanim);
			}
			else if (pe->GetItemType() == eItemLightSeq)
			{
				LightSeq* const lightseq = (LightSeq*)pe;
				m_vanimate.push_back(&lightseq->m_lightseqanim);
			}
		}
	}

	g_viewDir = Vertex3Ds(0, 0, -1.0f);

	InitShader();

	for (int i = 0; i < m_ptable->m_vcollection.size(); i++)
	{
		/* TODO:	Collection* const pcol = m_ptable->m_vcollection.ElementAt(i);
		for (int t = 0; t < pcol->m_visel.size(); t++)
		{
			ISelect* const pisel = pcol->m_visel.ElementAt(t);
			if (pisel != nullptr && pisel->GetItemType() == eItemPrimitive)
			{
				Primitive* const prim = (Primitive*)pisel;
				prim->CreateRenderGroup(pcol);
				break;
			}
		} */
	}

	InitStatic(); // DELETEME: Line 1412

	for (size_t i = 0; i < m_ptable->m_vedit.size(); ++i)
	{
		IEditable* const pe = m_ptable->m_vedit[i];
		IHitable* const ph = pe->GetIHitable();
		if (ph)
		{
			// sort into proper categories
			if (ph->IsTransparent())
			{
				m_vHitTrans.push_back(ph);
			}
			else
			{
				m_vHitNonTrans.push_back(ph);
			}
		}
	}

	material_flips = 0;
	unsigned long long m;
	if (!m_vHitNonTrans.empty())
	{
		std::stable_sort(m_vHitNonTrans.begin(), m_vHitNonTrans.end(), CompareHitableDepthReverse);
		std::stable_sort(m_vHitNonTrans.begin(), m_vHitNonTrans.end(), CompareHitableImage);

		std::stable_sort(m_vHitNonTrans.begin(), m_vHitNonTrans.end(), CompareHitableMaterial);

		m = m_vHitNonTrans[0]->GetMaterialID();

		for (size_t i = 1; i < m_vHitNonTrans.size(); ++i)
			if (m_vHitNonTrans[i]->GetMaterialID() != m)
			{
				material_flips++;
				m = m_vHitNonTrans[i]->GetMaterialID();
			}
	}

	if (!m_vHitTrans.empty())
	{
		std::stable_sort(m_vHitTrans.begin(), m_vHitTrans.end(), CompareHitableImage);
		std::stable_sort(m_vHitTrans.begin(), m_vHitTrans.end(), CompareHitableMaterial);
		std::stable_sort(m_vHitTrans.begin(), m_vHitTrans.end(), CompareHitableDepth);

		m = m_vHitTrans[0]->GetMaterialID();

		for (size_t i = 1; i < m_vHitTrans.size(); ++i)
			if (m_vHitTrans[i]->GetMaterialID() != m)
			{
				material_flips++;
				m = m_vHitTrans[i]->GetMaterialID();
			}
	}

	// TODO: m_pin3d.SetPrimaryRenderTarget(m_pin3d.m_pddsBackBuffer, m_pin3d.m_pddsZBuffer);

	return hr;
}

void Player::InitShader()
{
	UpdateBasicShaderMatrix();

	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetBool("hdrEnvTextures", (m_pin3d.m_envTexture ? m_pin3d.m_envTexture : &m_pin3d.m_builtinEnvTexture)->IsHDR());
	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetTexture("Texture1", m_pin3d.m_envTexture ? m_pin3d.m_envTexture : &m_pin3d.m_builtinEnvTexture, false);

	/*// TODO:  m_pin3d.m_pd3dPrimaryDevice->m_texMan.LoadTexture(m_pin3d.m_envRadianceTexture, false));
	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetTexture("Texture2", m_pin3d.m_envTexture ? m_pin3d.m_envTexture : &m_pin3d.m_builtinEnvTexture, false);
*/
	const vec4 st(m_ptable->m_envEmissionScale * m_globalEmissionScale, m_pin3d.m_envTexture ? (float)m_pin3d.m_envTexture->m_height : (float)m_pin3d.m_builtinEnvTexture.m_height, 0.f, 0.f);
	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetVector("fenvEmissionScale_TexWidth", &st);

	InitBallShader();
}

void Player::InitBallShader()
{
}

void Player::InitStatic()
{
	for (size_t i = 0; i < m_vhitables.size(); ++i)
	{
		IHitable* const pHitable = m_vhitables[i];
		pHitable->RenderSetup();
	}

	m_pin3d.InitPlayfieldGraphics();

	// TODO: D3DSURFACE_DESC descStatic;
	// TODO: m_pin3d.m_pddsStatic->GetDesc(&descStatic);
	// TODO: RECT rectStatic;
	// TODO: rectStatic.left = 0;
	// TODO: rectStatic.right = descStatic.Width;
	// TODO: rectStatic.top = 0;
	// TODO: ectStatic.bottom = descStatic.Height;

	// TODO: float* __restrict const pdestStatic = new float[descStatic.Width * descStatic.Height * 3]; // RGB float32
	// TODO: memset(pdestStatic, 0, descStatic.Width * descStatic.Height * 3 * sizeof(float));

	// TODO: RenderTarget* offscreenSurface;
	// TODO: CHECKD3D(m_pin3d.m_pd3dPrimaryDevice->GetCoreDevice()->CreateOffscreenPlainSurface(descStatic.Width, descStatic.Height, // TODO: descStatic.Format, (D3DPOOL)memoryPool::SYSTEM, &offscreenSurface, NULL));

	if (!m_cameraMode)
	{
		m_isRenderingStatic = true;
		// TODO: m_pin3d.m_pd3dPrimaryDevice->SetTextureFilter(0, TEXTURE_MODE_TRILINEAR);
		// TODO: m_pin3d.m_pd3dPrimaryDevice->SetTextureFilter(4, TEXTURE_MODE_TRILINEAR);
	}

	for (int iter = m_cameraMode ? 0 : (STATIC_PRERENDER_ITERATIONS - 1); iter >= 0; --iter)
	{
		m_pin3d.m_pd3dPrimaryDevice->m_stats_drawn_triangles = 0;

		float u1 = xyLDBNbnot[iter * 2];
		float u2 = xyLDBNbnot[iter * 2 + 1];

		gaussianDistribution(u1, u2, 0.5f, 0.5f);

		assert(u1 > -1.f && u1 < 2.f);
		assert(u2 > -1.f && u2 < 2.f);

		if (iter == 0)
		{
			assert(u1 == 0.5f && u2 == 0.5f);
		}

		m_pin3d.InitLayout(m_ptable->m_BG_enable_FSS, u1 - 0.5f, u2 - 0.5f);

		m_pin3d.m_pd3dPrimaryDevice->BeginScene();

		// TODO: m_pin3d.SetPrimaryRenderTarget(m_pin3d.m_pddsStatic, m_pin3d.m_pddsStaticZ);

		m_pin3d.DrawBackground();

		for (size_t i = 0; i < m_vhitables.size(); ++i)
		{
			IHitable* const ph = m_vhitables[i];
			ph->PreRenderStatic(m_pin3d.m_pd3dPrimaryDevice);
		}

		SetClipPlanePlayfield(true);

		if (!m_cameraMode)
		{

			const bool drawBallReflection = ((m_reflectionForBalls && (m_ptable->m_useReflectionForBalls == -1)) || (m_ptable->m_useReflectionForBalls == 1));
			if (!(m_ptable->m_reflectElementsOnPlayfield) && drawBallReflection)
			{
				RenderStaticMirror(true);
			}
			else if (m_ptable->m_reflectElementsOnPlayfield)
			{
				RenderStaticMirror(false);
			}

			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_FALSE);
			m_pin3d.RenderPlayfieldGraphics(false);
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);

			if (m_ptable->m_reflectElementsOnPlayfield)
			{
				RenderMirrorOverlay();
			}

			SetClipPlanePlayfield(false);
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, RenderDevice::PLANE0);

			for (size_t i = 0; i < m_ptable->m_vedit.size(); i++)
			{
				if (m_ptable->m_vedit[i]->GetItemType() != eItemDecal)
				{
					IHitable* const ph = m_ptable->m_vedit[i]->GetIHitable();
					if (ph)
					{
						ph->RenderStatic();
						if (((i % 16) == 0) && iter == 0)
						{
							// TODO: m_ptable->m_progressDialog.SetProgress(60 + ((15 * (int)i) / (int)m_ptable->m_vedit.size()));
						}
					}
				}
			}

			for (size_t i = 0; i < m_ptable->m_vedit.size(); i++)
			{
				if (m_ptable->m_vedit[i]->GetItemType() == eItemDecal)
				{
					IHitable* const ph = m_ptable->m_vedit[i]->GetIHitable();
					if (ph)
					{
						ph->RenderStatic();
						if (((i % 16) == 0) && iter == 0)
						{
							// TODO: m_ptable->m_progressDialog.SetProgress(75 + ((15 * (int)i) / (int)m_ptable->m_vedit.size()));
						}
					}
				}
			}

			m_pin3d.DisableAlphaBlend();
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::BLENDOP, RenderDevice::BLENDOP_ADD);
			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

			m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, 0);
			SetClipPlanePlayfield(true);
		}

		m_pin3d.m_pd3dPrimaryDevice->EndScene();

		/* TODO:		if (!m_cameraMode)
		{
			CHECKD3D(m_pin3d.m_pd3dPrimaryDevice->GetCoreDevice()->GetRenderTargetData(m_pin3d.m_pddsStatic, offscreenSurface));

			D3DLOCKED_RECT locked;
			CHECKD3D(offscreenSurface->LockRect(&locked, &rectStatic, D3DLOCK_READONLY));

			const unsigned short* __restrict const psrc = (unsigned short*)locked.pBits;
			for (unsigned int y = 0; y < descStatic.Height; ++y)
			{
				unsigned int ofs0 = y * descStatic.Width * 3;
				unsigned int ofs1 = y * locked.Pitch / 2;
				if (descStatic.Format == (D3DFORMAT)colorFormat::RGBA16F)
				{
					for (unsigned int x = 0; x < descStatic.Width; ++x, ofs0 += 3, ofs1 += 4)
					{
						pdestStatic[ofs0] += half2float(psrc[ofs1]);
						pdestStatic[ofs0 + 1] += half2float(psrc[ofs1 + 1]);
						pdestStatic[ofs0 + 2] += half2float(psrc[ofs1 + 2]);
					}
				}
				else if (descStatic.Format == (D3DFORMAT)colorFormat::RED16F)
				{
					for (unsigned int x = 0; x < descStatic.Width; ++x, ++ofs0, ++ofs1)
					{
						pdestStatic[ofs0] += half2float(psrc[ofs1]);
					}
				}
				else if (descStatic.Format == (D3DFORMAT)colorFormat::RG16F)
				{
					for (unsigned int x = 0; x < descStatic.Width; ++x, ofs0 += 2, ofs1 += 2)
					{
						pdestStatic[ofs0] += half2float(psrc[ofs1]);
						pdestStatic[ofs0 + 1] += half2float(psrc[ofs1 + 1]);
					}
				}
			}

			offscreenSurface->UnlockRect();
		} */
		stats_drawn_static_triangles = m_pin3d.m_pd3dPrimaryDevice->m_stats_drawn_triangles;
	}
}

void Player::UpdatePhysics()
{
}

bool Player::RenderStaticOnly() const
{
	const unsigned int modes = (m_showFPS % 9);
	return (modes == 6);
}

void Player::RenderImgGui()
{
	ImGui::Begin("Statistics");

	const float fpsAvg = (m_fpsCount == 0) ? 0.0f : m_fpsAvg / m_fpsCount;

	ImGui::Text("FPS: %.1f (%.1f avg)  Display %s Objects(%uk/%uk Triangles)", m_fps + 0.01f, fpsAvg + 0.01f, RenderStaticOnly() ? "only static" : "all", (m_pin3d.m_pd3dPrimaryDevice->m_stats_drawn_triangles + 999) / 1000, (stats_drawn_static_triangles + m_pin3d.m_pd3dPrimaryDevice->m_stats_drawn_triangles + 999) / 1000);

	ImGui::Text("Draw calls: %u (%u Locks)", m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumDrawCalls(), m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumLockCalls());
	ImGui::Text("State changes: %u", m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumStateChanges());
	ImGui::Text("Texture changes: %u (%u Uploads)", m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumTextureChanges(), m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumTextureUploads());
	ImGui::Text("Shader/Parameter changes: %u / %u (%u Material ID changes)", m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumTechniqueChanges(), m_pin3d.m_pd3dPrimaryDevice->Perf_GetNumParameterChanges(), material_flips);
	ImGui::Text("Objects: %u Transparent, %u Solid", (unsigned int)m_vHitTrans.size(), (unsigned int)m_vHitNonTrans.size());

	ImGui::End();
}

void Player::FlipVideoBuffers(const bool vsync)
{
	m_pin3d.Flip(vsync);

	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->SetTexture("Texture0", (D3DTexture*)NULL);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->SetRenderTarget(m_pin3d.m_pddsBackBuffer);

	//m_lastFlipTime = usec();
}

void Player::Render()
{
	m_LastKnownGoodCounter++;

	m_pin3d.m_pd3dPrimaryDevice->m_stats_drawn_triangles = 0;

	// TODO: m_pin3d.m_pd3dPrimaryDevice->CopySurface(m_pin3d.m_pddsBackBuffer, m_pin3d.m_pddsStatic);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->CopySurface(m_pin3d.m_pddsZBuffer, m_pin3d.m_pddsStaticZ); // cannot be called inside BeginScene -> EndScene cycle

	if (m_minphyslooptime == 0)
	{
		UpdatePhysics();
	}

	m_overall_frames++;

	for (size_t l = 0; l < m_vanimate.size(); ++l)
	{
		m_vanimate[l]->Animate();
	}

	if (!RenderStaticOnly())
	{
		m_pin3d.m_pd3dPrimaryDevice->BeginScene();
		RenderDynamics();
		m_pin3d.m_pd3dPrimaryDevice->EndScene();
	}

	bool vsync = false;

	FlipVideoBuffers(vsync);
}

void Player::RenderStaticMirror(const bool onlyBalls)
{
	// TODO: m_pin3d.m_pd3dPrimaryDevice->SetRenderTarget(m_pin3d.m_pd3dPrimaryDevice->GetMirrorTmpBufferTexture());
	m_pin3d.m_pd3dPrimaryDevice->Clear(0, NULL, ClearType::TARGET, 0, 1.0f, 0L);

	if (!onlyBalls)
	{
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, RenderDevice::PLANE0);

		D3DMATRIX viewMat;
		m_pin3d.m_pd3dPrimaryDevice->GetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);

		viewMat._33 = -viewMat._33;
		const float rotation = fmodf(m_ptable->m_BG_rotation[m_ptable->m_BG_current_set], 360.f);
		if (rotation != 0.0f)
		{
			viewMat._31 = -viewMat._31;
		}
		else
		{
			viewMat._32 = -viewMat._32;
		}
		m_pin3d.m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);

		m_ptable->m_reflectionEnabled = true;
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

		UpdateBasicShaderMatrix();

		for (size_t i = 0; i < m_ptable->m_vedit.size(); i++)
		{
			if (m_ptable->m_vedit[i]->GetItemType() != eItemDecal)
			{
				IHitable* const ph = m_ptable->m_vedit[i]->GetIHitable();
				if (ph)
				{
					ph->RenderStatic();
				}
			}
		}

		m_pin3d.DisableAlphaBlend();
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::BLENDOP, RenderDevice::BLENDOP_ADD);

		m_ptable->m_reflectionEnabled = false;
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

		viewMat._33 = -viewMat._33;
		if (rotation != 0.0f)
		{
			viewMat._31 = -viewMat._31;
		}
		else
		{
			viewMat._32 = -viewMat._32;
		}
		m_pin3d.m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);
		UpdateBasicShaderMatrix();

		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, 0);
	}

	// TODO: m_pin3d.m_pd3dPrimaryDevice->SetRenderTarget(m_pin3d.m_pddsStatic);
}

void Player::RenderDynamicMirror(const bool onlyBalls)
{
	// TODO: m_pin3d.m_pd3dPrimaryDevice->SetRenderTarget(m_pin3d.m_pd3dPrimaryDevice->GetMirrorTmpBufferTexture());
	m_pin3d.m_pd3dPrimaryDevice->Clear(0, NULL, ClearType::TARGET, 0, 1.0f, 0L);

	D3DMATRIX viewMat;
	m_pin3d.m_pd3dPrimaryDevice->GetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);

	viewMat._33 = -viewMat._33;
	const float rotation = fmodf(m_ptable->m_BG_rotation[m_ptable->m_BG_current_set], 360.f);
	if (rotation != 0.0f)
	{
		viewMat._31 = -viewMat._31;
	}
	else
	{
		viewMat._32 = -viewMat._32;
	}
	m_pin3d.m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);

	m_ptable->m_reflectionEnabled = true;
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

	if (!onlyBalls)
	{
		UpdateBasicShaderMatrix();
	}

	// TODO: UpdateBallShaderMatrix();

	if (!onlyBalls)
	{
		std::stable_sort(m_vHitTrans.begin(), m_vHitTrans.end(), CompareHitableDepthInverse);

		for (size_t i = 0; i < m_vHitTrans.size(); ++i)
		{
			m_vHitTrans[i]->RenderDynamic();
		}

		std::stable_sort(m_vHitTrans.begin(), m_vHitTrans.end(), CompareHitableDepth);
	}

	// TODO: DrawBalls();

	if (!onlyBalls)
	{
		for (size_t i = 0; i < m_vHitNonTrans.size(); ++i)
		{
			m_vHitNonTrans[i]->RenderDynamic();
		}
	}

	m_pin3d.DisableAlphaBlend();
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::BLENDOP, RenderDevice::BLENDOP_ADD);

	m_ptable->m_reflectionEnabled = false;
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);

	viewMat._33 = -viewMat._33;
	if (rotation != 0.0f)
	{
		viewMat._31 = -viewMat._31;
	}
	else
	{
		viewMat._32 = -viewMat._32;
	}
	m_pin3d.m_pd3dPrimaryDevice->SetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);

	if (!onlyBalls)
	{
		UpdateBasicShaderMatrix();
	}

	// TODO: UpdateBallShaderMatrix();

	// TODO: m_pin3d.m_pd3dPrimaryDevice->SetRenderTarget(m_pin3d.m_pddsBackBuffer);
}

void Player::UpdateBasicShaderMatrix(const Matrix3D& objectTrafo)
{
	D3DMATRIX worldMat;
	D3DMATRIX viewMat;
	D3DMATRIX projMat;

	m_pin3d.m_pd3dPrimaryDevice->GetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_WORLD, &worldMat);
	m_pin3d.m_pd3dPrimaryDevice->GetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_VIEW, &viewMat);
	m_pin3d.m_pd3dPrimaryDevice->GetTransform(RenderDevice::TransformStateType::TRANSFORMSTATE_PROJECTION, &projMat);

	D3DXMATRIX matProj(projMat);
	D3DXMATRIX matView(viewMat);
	D3DXMATRIX matWorld(worldMat);
	D3DXMATRIX matObject(objectTrafo);

	if (m_ptable->m_reflectionEnabled)
	{
		matObject._43 -= m_ptable->m_tableheight * 2.0f;
	}

	D3DXMATRIX matWorldView = matObject * matWorld * matView;
	D3DXMATRIX matWorldViewProj = matWorldView * matProj;

	if (m_ptable->m_tblMirrorEnabled)
	{
		const D3DXMATRIX flipx(-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		const D3DXMATRIX flipy(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		const float rotation = fmodf(m_ptable->m_BG_rotation[m_ptable->m_BG_current_set], 360.f);
		matWorldViewProj = matWorldViewProj * (rotation != 0.0f ? flipy : flipx);
	}
	Matrix3D temp;
	memcpy(temp.m, matWorldView.m, 4 * 4 * sizeof(float));
	temp.Invert();
	temp.Transpose();
	D3DXMATRIX matWorldViewInvTrans;
	memcpy(matWorldViewInvTrans.m, temp.m, 4 * 4 * sizeof(float));

	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetMatrix("matWorldViewProj", &matWorldViewProj);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->flasherShader->SetMatrix("matWorldViewProj", &matWorldViewProj);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->lightShader->SetMatrix("matWorldViewProj", &matWorldViewProj);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->DMDShader->SetMatrix("matWorldViewProj", &matWorldViewProj);

	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetMatrix("matWorldView", &matWorldView);
	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetMatrix("matWorldViewInverseTranspose", &matWorldViewInvTrans);
	m_pin3d.m_pd3dPrimaryDevice->basicShader->SetMatrix("matView", &matView);
}

void Player::RenderDynamics()
{
	unsigned int reflection_path = 0;

	if (!m_cameraMode)
	{
		const bool drawBallReflection = ((m_reflectionForBalls && (m_ptable->m_useReflectionForBalls == -1)) || (m_ptable->m_useReflectionForBalls == 1));

		if (!(m_ptable->m_reflectElementsOnPlayfield && m_pf_refl) && drawBallReflection)
		{
			reflection_path = 1;
		}
		else if (m_ptable->m_reflectElementsOnPlayfield && m_pf_refl)
		{
			reflection_path = 2;
		}
	}

	if (reflection_path != 0)
	{
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, RenderDevice::PLANE0);
		RenderDynamicMirror(reflection_path == 1);
		m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CLIPPLANEENABLE, 0);

		RenderMirrorOverlay();
	}

	m_pin3d.RenderPlayfieldGraphics(true);
}

void Player::SetClipPlanePlayfield(const bool clip_orientation)
{
	// TODO: Matrix3D mT = m_pin3d.m_proj.m_matrixTotal;
	// TODO: mT.Invert();
	// TODO: mT.Transpose();
	// TODO: D3DXMATRIX m(mT);
	// TODO: D3DXPLANE clipSpacePlane;
	// TODO: const D3DXPLANE plane(0.0f, 0.0f, clip_orientation ? -1.0f : 1.0f, clip_orientation ? m_ptable->m_tableheight : -m_ptable->m_tableheight);
	// TODO: D3DXPlaneTransform(&clipSpacePlane, &plane, &m);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->GetCoreDevice()->SetClipPlane(0, clipSpacePlane);
}

void Player::RenderMirrorOverlay()
{
	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->SetTexture("Texture0", m_pin3d.m_pd3dPrimaryDevice->GetMirrorTmpBufferTexture());
	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->SetFloat("mirrorFactor", m_ptable->m_playfieldReflectionStrength);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->SetTechnique("fb_mirror");

	m_pin3d.EnableAlphaBlend(false, false);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::DESTBLEND, RenderDevice::DST_ALPHA);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_NONE);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_FALSE);

	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->Begin(0);
	// TODO: m_pin3d.m_pd3dPrimaryDevice->DrawFullscreenTexturedQuad();
	// TODO: m_pin3d.m_pd3dPrimaryDevice->FBShader->End();

	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::ZWRITEENABLE, RenderDevice::RS_TRUE);
	m_pin3d.m_pd3dPrimaryDevice->SetRenderState(RenderDevice::CULLMODE, RenderDevice::CULL_CCW);
	m_pin3d.DisableAlphaBlend();
}