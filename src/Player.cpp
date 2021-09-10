#include "Player.h"

Player::Player(const bool cameraMode, PinTable* const pPinTable)
{
	m_cameraMode = cameraMode;
	m_ptable = pPinTable;
}

Player::~Player()
{
}

void Player::Render()
{

	BgfxWindow::Render();
}