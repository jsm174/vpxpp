#include "VPApp.h"
#include "EditableRegistry.h"

#include "Bumper.h"
#include "Decal.h"
#include "DispReel.h"
#include "Flasher.h"
#include "Flipper.h"
#include "Gate.h"
#include "HitTarget.h"
#include "Kicker.h"
#include "Light.h"
#include "LightSeq.h"
#include "PinTable.h"
#include "Plunger.h"
#include "Primitive.h"
#include "Ramp.h"
#include "Rubber.h"
#include "Spinner.h"
#include "Surface.h"
#include "Textbox.h"
#include "Timer.h"
#include "Trigger.h"

#include "extern.h"

std::map<ItemTypeEnum, EditableInfo> EditableRegistry::m_map;

VPApp::VPApp()
{
}

VPApp::~VPApp()
{
}

bool VPApp::InitInstance(std::string szTableFilename)
{
	m_file = false;
	m_play = false;
	m_run = true;
	m_loadFileResult = true;
	m_szTableFilename.clear();
	m_szTableFilename = szTableFilename;

	m_file = true;
	m_play = true;

	InitVPX();

	return true;
}

void VPApp::InitVPX()
{
	EditableRegistry::RegisterEditable<Bumper>();
	EditableRegistry::RegisterEditable<Decal>();
	EditableRegistry::RegisterEditable<DispReel>();
	EditableRegistry::RegisterEditable<Flasher>();
	EditableRegistry::RegisterEditable<Flipper>();
	EditableRegistry::RegisterEditable<Gate>();
	EditableRegistry::RegisterEditable<Kicker>();
	EditableRegistry::RegisterEditable<Light>();
	EditableRegistry::RegisterEditable<LightSeq>();
	EditableRegistry::RegisterEditable<Plunger>();
	EditableRegistry::RegisterEditable<Primitive>();
	EditableRegistry::RegisterEditable<Ramp>();
	EditableRegistry::RegisterEditable<Rubber>();
	EditableRegistry::RegisterEditable<Spinner>();
	EditableRegistry::RegisterEditable<Surface>();
	EditableRegistry::RegisterEditable<Textbox>();
	EditableRegistry::RegisterEditable<Timer>();
	EditableRegistry::RegisterEditable<Trigger>();
	EditableRegistry::RegisterEditable<HitTarget>();

	g_pvp = &m_vpinball;

	if (m_file)
	{
		if (!m_szTableFilename.empty())
		{
			m_vpinball.LoadFileName(m_szTableFilename, !m_play);
		}
	}
}

int VPApp::Run()
{
	if (m_run)
	{
		if (m_play && m_loadFileResult)
		{
			m_vpinball.DoPlay(m_vpinball.m_povEdit);
		}

		m_vpinball.MainMsgLoop();
	}

	return 0;
}