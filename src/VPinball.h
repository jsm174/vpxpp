#pragma once

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

	bool m_povEdit;

	std::vector<PinTable*> m_vtable;

private:
};