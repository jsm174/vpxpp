#pragma once

#include <string>

class BaseProperty
{
public:
	std::string m_szImage;
	std::string m_szMaterial;
	std::string m_szPhysicsMaterial;
	float m_elasticity;
	float m_friction;
	float m_scatter;
	float m_threshold;
	bool m_collidable;
	bool m_hitEvent;
	bool m_overwritePhysics;
	bool m_reflectionEnabled;
	bool m_visible;
};