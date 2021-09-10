#pragma once

#include "Pin3D.h"
#include "PinTable.h"

#include "BgfxWindow.h"

class Player : public BgfxWindow
{

public:
	Player(const bool cameraMode, PinTable* const ptable);
	virtual ~Player();

	bool m_cameraMode;
	bool m_hasWindow;

	PinTable* m_ptable;
	Pin3D m_pin3d;

	virtual void Render();
};