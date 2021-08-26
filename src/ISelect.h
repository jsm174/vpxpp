#pragma once

#include "ILoadable.h"

enum ItemTypeEnum
{
	eItemSurface,
	eItemFlipper,
	eItemTimer,
	eItemPlunger,
	eItemTextbox,
	eItemBumper,
	eItemTrigger,
	eItemLight,
	eItemKicker,
	eItemDecal,
	eItemGate,
	eItemSpinner,
	eItemRamp,
	eItemTable,
	eItemLightCenter,
	eItemDragPoint,
	eItemCollection,
	eItemDispReel,
	eItemLightSeq,
	eItemPrimitive,
	eItemFlasher,
	eItemRubber,
	eItemHitTarget,
	//eItemLightSeqCenter,
	eItemTypeCount,
	eItemInvalid = 0xffffffff // Force enum to be 32 bits
};

class PinTable;

class ISelect : public ILoadable
{
public:
	ISelect();

	//virtual PinTable *GetPTable() = 0;
	//virtual const PinTable *GetPTable() const = 0;

	void GetTypeNameForType(const ItemTypeEnum type, wchar_t* const buf) const;
};