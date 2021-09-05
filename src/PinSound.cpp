#include "PinSound.h"

PinSound::PinSound()
{
}

PinSound::~PinSound()
{
}

HRESULT PinSound::ReInitialize()
{
	return S_OK;
}

bool PinSound::IsWav() const
{
	const size_t pos = m_szPath.find_last_of('.');

	if (pos == std::string::npos)
	{
		return true;
	}

	return (strcasecmp(m_szPath.substr(pos + 1).c_str(), "wav") == 0);
}

bool PinSound::IsWav2() const
{
	return IsWav();
}
