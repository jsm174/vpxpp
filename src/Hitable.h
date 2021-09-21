#pragma once

#include "RenderDevice.h"
#include "HitObject.h"
#include "Vertex3Ds.h"

#include <vector>

class Hitable
{
public:
	// TODO: virtual void GetHitShapes(std::vector<HitObject*>& pvho) = 0;
	// TODO: virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho) = 0;
	// TODO: virtual void GetTimers(std::vector<HitTimer*>& pvht) = 0;
	// TODO: virtual EventProxyBase* GetEventProxyBase() = 0;
	// TODO: virtual void EndPlay() = 0;
	virtual void PreRenderStatic(RenderDevice* pd3dDevice);
	// TODO: virtual void RenderStatic() = 0;
	// TODO: virtual void RenderDynamic() = 0;
	virtual void RenderSetup() = 0;
	// TODO: virtual ItemTypeEnum HitableGetItemType() const = 0;

	virtual bool IsTransparent() const;
	virtual float GetDepth(const Vertex3Ds& viewDir) const;
	virtual unsigned long long GetMaterialID() const;
	virtual unsigned long long GetImageID() const;
	virtual bool RenderToLightBuffer() const;
	virtual bool IsDMD() const;
};