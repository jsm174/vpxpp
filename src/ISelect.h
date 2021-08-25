#pragma once

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