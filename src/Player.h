#pragma once

#include "misc.h"

#include "AnimObject.h"
#include "BgfxWindow.h"
#include "IHitable.h"
#include "Pin3D.h"
#include "PinTable.h"

class Player : public BgfxWindow
{

public:
	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	virtual void Render();

	bool m_cameraMode;
	bool m_hasWindow;

	PinTable* m_ptable;
	Pin3D m_pin3d;
	uint32_t m_time_msec;

	std::vector<AnimObject*> m_vanimate;

	int m_VSync;
	int m_FXAA;
	int m_sharpen;
	bool m_AA;
	bool m_disableAO;

	bool m_disableDWM;

	bool m_ss_refl;

	int m_stereo3D;

	int m_BWrendering;

	int m_width;
	int m_height;
	int m_refreshrate;
	bool m_fullScreen;

private:
	virtual void OnInitialUpdate();

	HRESULT Init();

	void RenderDynamics();
	void InitStatic();
	void RenderStaticMirror(const bool onlyBalls);

	std::vector<HitObject*> m_vho;
	std::vector<IHitable*> m_vhitables;
};