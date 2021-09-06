#pragma once

#include "VPinball.h"
#include <string>

class VPApp
{
public:
	VPApp();
	~VPApp();

	bool InitInstance(std::string szTableFilename);
	void InitVPX();

	virtual int Run();

private:
	bool m_run;
	bool m_play;
	bool m_file;
	bool m_loadFileResult;
	std::string m_szTableFilename;
	VPinball m_vpinball;
};