#pragma once

#include "AnimObject.h"

class DispReel;

class DispReelAnimObject : public AnimObject
{
public:
	virtual void Animate();

	DispReel* m_pDispReel;
};