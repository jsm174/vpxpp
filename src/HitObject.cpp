#include "HitObject.h"

void HitObject::FireHitEvent(Ball* const pball)
{
	/* TODO: if (m_obj && m_fe && m_enabled)
	{
		const float dist_ls = (pball->m_lastEventPos - pball->m_d.m_pos).LengthSquared();
		pball->m_lastEventPos = pball->m_d.m_pos;

		const float normalDist = (m_ObjType == eHitTarget) ? 0.0f
														   : 0.25f;

		if (dist_ls > normalDist)
		{
			((IFireEvents*)m_obj)->FireGroupEvent(DISPID_HitEvents_Hit);
		}
	} */
}