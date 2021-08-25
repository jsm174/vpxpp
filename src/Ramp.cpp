#include "Ramp.h"

Ramp::Ramp()
{
	//m_menuid = IDR_SURFACEMENU;
	m_d.m_collidable = true;
	m_d.m_visible = true;
	m_dynamicVertexBuffer = NULL;
	m_dynamicIndexBuffer = NULL;
	m_dynamicVertexBuffer2 = NULL;
	m_dynamicVertexBufferRegenerate = true;
	m_d.m_depthBias = 0.0f;
	m_d.m_wireDiameter = 6.0f;
	m_d.m_wireDistanceX = 38.0f;
	m_d.m_wireDistanceY = 88.0f;
	// TODO: m_propPosition = NULL;
	// TODO: m_propPhysics = NULL;
	m_d.m_hitEvent = false;
	m_d.m_overwritePhysics = true;
	m_rgheightInit = NULL;
}