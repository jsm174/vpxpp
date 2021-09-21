#include "IHitable.h"

void IHitable::PreRenderStatic(RenderDevice* pd3dDevice)
{
}

bool IHitable::IsTransparent() const
{
	return false;
}

float IHitable::GetDepth(const Vertex3Ds& viewDir) const
{
	return 0.0f;
}
unsigned long long IHitable::GetMaterialID() const
{
	return 0;
}

unsigned long long IHitable::GetImageID() const
{
	return 0;
}

bool IHitable::RenderToLightBuffer() const
{
	return false;
}

bool IHitable::IsDMD() const
{
	return false;
}