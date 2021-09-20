#pragma once

#include "Material.h"
#include "PinTable.h"
#include "misc.h"

#include <string>
#include <vector>

class VPinball
{
public:
	static bool m_firstRun;

	VPinball();
	~VPinball();

	bool LoadFile(const bool updateEditor);
	void LoadFileName(const std::string& szFileName, const bool updateEditor);

	void DoPlay(const bool _cameraMode);

	void MainMsgLoop();

	bool m_povEdit;

	std::vector<PinTable*> m_vtable;

	Material m_dummyMaterial;

	bool m_primaryDisplay;

private:
};