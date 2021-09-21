#pragma once

#include "HitObject.h"
#include "RenderDevice.h"
#include "Vertex3Ds.h"

#include <vector>

class IHitable
{
public:
	virtual void GetHitShapes(std::vector<HitObject*>& pvho) = 0;
	virtual void GetHitShapesDebug(std::vector<HitObject*>& pvho) = 0;
	// TODO: virtual void GetTimers(std::vector<HitTimer*>& pvht) = 0;
	// TODO: virtual EventProxyBase* GetEventProxyBase() = 0;
	// TODO: virtual void EndPlay() = 0;
	virtual void PreRenderStatic(RenderDevice* pd3dDevice);
	virtual void RenderStatic() = 0;
	virtual void RenderDynamic() = 0;
	virtual void RenderSetup() = 0;
	virtual ItemTypeEnum HitableGetItemType() const = 0;

	virtual bool IsTransparent() const;
	virtual float GetDepth(const Vertex3Ds& viewDir) const;
	virtual unsigned long long GetMaterialID() const;
	virtual unsigned long long GetImageID() const;
	virtual bool RenderToLightBuffer() const;
	virtual bool IsDMD() const;
};