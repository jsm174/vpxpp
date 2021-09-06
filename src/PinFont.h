#pragma once

#include "PinBinary.h"

class PinFont : public PinBinary
{
public:
	void Register();
	void UnRegister();

private:
	std::string m_szTempFile;
};