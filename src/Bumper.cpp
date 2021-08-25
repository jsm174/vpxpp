#include "Bumper.h"

Bumper::Bumper()
{
	// TODO: m_pbumperhitcircle = NULL;
	m_baseVertexBuffer = NULL;
	m_baseIndexBuffer = NULL;
	m_ringVertexBuffer = NULL;
	m_ringIndexBuffer = NULL;
	m_capVertexBuffer = NULL;
	m_capIndexBuffer = NULL;
	m_socketIndexBuffer = NULL;
	m_socketVertexBuffer = NULL;
	m_ringAnimate = false;
	// TODO: m_propVisual = NULL;
	m_d.m_ringDropOffset = 0.0f;
	m_ringDown = false;
	m_updateSkirt = false;
	m_doSkirtAnimation = false;
	m_enableSkirtAnimation = true;
	m_skirtCounter = 0.0f;
}