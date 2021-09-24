#pragma once

#include "AnimObject.h"
#include "misc.h"

class SlingshotAnimObject : public AnimObject
{
public:
	virtual void Animate();

	uint32_t m_TimeReset;
	bool m_animations;
	bool m_iframe;
};