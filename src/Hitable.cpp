#include "Hitable.h"

void Hitable::PreRenderStatic(RenderDevice* pd3dDevice)
{
}

bool Hitable::IsTransparent() const
{
	return false;
}

float Hitable::GetDepth(const Vertex3Ds& viewDir) const
{
	return 0.0f;
}
unsigned long long Hitable::GetMaterialID() const
{
	return 0;
}

unsigned long long Hitable::GetImageID() const
{
	return 0;
}

bool Hitable::RenderToLightBuffer() const
{
	return false;
}

bool Hitable::IsDMD() const
{
	return false;
}