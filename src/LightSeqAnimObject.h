#pragma once

#include "AnimObject.h"

class LightSeq;

class LightSeqAnimObject : public AnimObject
{
public:
	virtual void Animate();

	LightSeq* m_pLightSeq;
};