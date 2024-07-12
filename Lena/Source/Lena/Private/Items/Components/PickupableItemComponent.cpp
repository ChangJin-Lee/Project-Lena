#include "Items/Components/PickupableItemComponent.h"

UPickupableItemComponent::UPickupableItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ItemID = TEXT("");
	ItemName = TEXT("");
	Quantity = 1;
}
