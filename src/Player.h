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
	static unsigned int material_flips;
	static unsigned int stats_drawn_static_triangles;

	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	virtual void PreCreate();

	virtual void RenderImgGui();
	virtual void Render();

	void UpdateBasicShaderMatrix(const Matrix3D& objectTrafo = Matrix3D(1.0f));

	void FlipVideoBuffers(const bool vsync);

	bool m_cameraMode;
	bool m_hasWindow;

	PinTable* m_ptable;
	Pin3D m_pin3d;
	uint32_t m_time_msec;

	std::vector<AnimObject*> m_vanimate;

	int m_minphyslooptime;

	float m_globalEmissionScale;

	int m_VSync;
	int m_FXAA;
	int m_sharpen;
	bool m_AA;
	bool m_disableAO;

	bool m_disableDWM;

	int m_LastKnownGoodCounter;

	bool m_ss_refl;
	bool m_pf_refl;

	int m_stereo3D;

	int m_BWrendering;

	int m_width;
	int m_height;
	int m_refreshrate;
	bool m_fullScreen;

	int m_overall_frames;

	bool m_isRenderingStatic;

	bool m_reflectionForBalls;

	unsigned int m_showFPS;

private:
	virtual void OnInitialUpdate();

	HRESULT Init();

	void InitFPS();

	void RenderDynamics();

	void InitShader();

	void InitStatic();
	void UpdatePhysics();

	bool RenderStaticOnly() const;
	void RenderStaticMirror(const bool onlyBalls);
	void RenderDynamicMirror(const bool onlyBalls);
	void RenderMirrorOverlay();
	void InitBallShader();

	void SetClipPlanePlayfield(const bool clip_orientation);

	std::vector<HitObject*> m_vho;

	std::vector<IHitable*> m_vhitables;
	std::vector<IHitable*> m_vHitNonTrans;
	std::vector<IHitable*> m_vHitTrans;

	uint32_t m_lastfpstime;
	uint32_t m_cframes;
	float m_fps;
	float m_fpsAvg;
	uint32_t m_fpsCount;
	uint64_t m_lastTime_usec;
	uint32_t m_lastFrameDuration;
	uint32_t m_max;
	uint32_t m_max_total;
	uint64_t m_count;
	uint64_t m_total;
	int m_lastMaxChangeTime;
	uint64_t m_phys_total;
	uint64_t m_phys_total_iterations;
	uint32_t m_phys_max_iterations;
	uint32_t m_phys_period;
	uint32_t m_phys_max;
	uint32_t m_phys_max_total;

	uint32_t m_script_period;
	uint64_t m_script_total;
	uint32_t m_script_max;
	uint32_t m_script_max_total;
};

static const float xyLDBNbnot[STATIC_PRERENDER_ITERATIONS * 2] = {
	0.00000f, 0.00000f,
	0.13926f, 0.00971f,
	0.99043f, 0.01365f,
	0.02105f, 0.06487f,
	0.11562f, 0.13580f,
	0.24960f, 0.14369f,
	0.31265f, 0.05699f,
	0.43087f, 0.12004f,
	0.56484f, 0.11216f,
	0.71459f, 0.07670f,
	0.85645f, 0.10822f,
	0.96678f, 0.14763f,
	0.06045f, 0.26978f,
	0.19443f, 0.26190f,
	0.36782f, 0.22250f,
	0.50968f, 0.23826f,
	0.66730f, 0.19097f,
	0.78552f, 0.19491f,
	0.86433f, 0.28161f,
	0.02893f, 0.37224f,
	0.15503f, 0.39588f,
	0.28900f, 0.34071f,
	0.41510f, 0.34860f,
	0.52544f, 0.36436f,
	0.64366f, 0.30131f,
	0.75399f, 0.34465f,
	0.88797f, 0.41558f,
	0.97466f, 0.30525f,
	0.07621f, 0.50622f,
	0.21019f, 0.49834f,
	0.33629f, 0.45105f,
	0.46239f, 0.49046f,
	0.62789f, 0.44317f,
	0.76975f, 0.47075f,
	0.91161f, 0.54168f,
	0.12350f, 0.62443f,
	0.28112f, 0.58503f,
	0.39146f, 0.60867f,
	0.54908f, 0.56139f,
	0.60425f, 0.65596f,
	0.69094f, 0.55745f,
	0.82492f, 0.61261f,
	0.09001f, 0.75841f,
	0.20822f, 0.71113f,
	0.32644f, 0.72689f,
	0.49194f, 0.68748f,
	0.62592f, 0.78994f,
	0.72838f, 0.70719f,
	0.83871f, 0.76235f,
	0.94905f, 0.69142f,
	0.00331f, 0.87663f,
	0.09789f, 0.90816f,
	0.21610f, 0.83723f,
	0.38949f, 0.82146f,
	0.50771f, 0.81358f,
	0.57864f, 0.90027f,
	0.74414f, 0.85693f,
	0.85448f, 0.89633f,
	0.97269f, 0.81752f,
	0.27915f, 0.92392f,
	0.35008f, 0.99485f,
	0.45254f, 0.96332f,
	0.69685f, 0.96726f,
	0.91753f, 0.97515f};
