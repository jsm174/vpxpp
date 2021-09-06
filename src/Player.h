#pragma once

#include "PinTable.h"

class Player
{
public:
	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	bool m_cameraMode;
	PinTable* m_ptable;

	void Create();

private:
};