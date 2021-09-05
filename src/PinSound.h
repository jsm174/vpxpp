#pragma once

#include "misc.h"
#include <string>

class PinSound
{
public:
	PinSound();
	~PinSound();

	HRESULT ReInitialize();

	bool IsWav() const;
	bool IsWav2() const;

	std::string m_szName;
	std::string m_szPath;

	SoundOutTypes m_outputTarget;
	int m_balance;
	int m_fade;
	int m_volume;

	char* m_pData;
	int m_cdata;

	WAVEFORMATEX m_wfx;

private:
};