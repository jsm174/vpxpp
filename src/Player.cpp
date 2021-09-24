#include "Player.h"
#include "DispReel.h"
#include "LightSeq.h"
#include "RegUtil.h"

#include <iostream>

Player::Player(const bool cameraMode, PinTable* const pPinTable)
{
	m_cameraMode = cameraMode;
	m_ptable = pPinTable;

	RegUtil* pRegUtil = RegUtil::SharedInstance();

	m_VSync = pRegUtil->LoadValueIntWithDefault("Player", "AdaptiveVSync", 0);
	m_FXAA = pRegUtil->LoadValueIntWithDefault("Player", "FXAA", Standard_FXAA);
	m_sharpen = pRegUtil->LoadValueIntWithDefault("Player", "Sharpen", 0);
	m_AA = pRegUtil->LoadValueBoolWithDefault("Player", "USEAA", false);
	m_disableAO = pRegUtil->LoadValueBoolWithDefault("Player", "DisableAO", false);
	m_ss_refl = pRegUtil->LoadValueBoolWithDefault("Player", "SSRefl", false);
	m_stereo3D = pRegUtil->LoadValueIntWithDefault("Player", "Stereo3D", 0);

	m_disableDWM = pRegUtil->LoadValueBoolWithDefault("Player", "DisableDWM", false);
	m_BWrendering = pRegUtil->LoadValueIntWithDefault("Player", "BWRendering", 0);

	// TODO: Below comes from precreate

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

Player::~Player()
{
}

void Player::OnInitialUpdate()
{
	Init();
}

HRESULT Player::Init()
{
	RegUtil* pRegUtil = RegUtil::SharedInstance();

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

	m_pin3d.InitLayout(m_ptable->m_BG_enable_FSS);

	for (size_t i = 0; i < m_ptable->m_vedit.size(); i++)
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

	InitStatic();

	return hr;
}

void Player::InitStatic()
{
	for (size_t i = 0; i < m_vhitables.size(); ++i)
	{
		IHitable* const pHitable = m_vhitables[i];
		pHitable->RenderSetup();
	}

	m_pin3d.InitPlayfieldGraphics();
}

void Player::Render()
{
	RenderDynamics();

	BgfxWindow::Render();
}

void Player::RenderStaticMirror(const bool onlyBalls)
{
}

void Player::RenderDynamics()
{
	m_pin3d.RenderPlayfieldGraphics(true);
}