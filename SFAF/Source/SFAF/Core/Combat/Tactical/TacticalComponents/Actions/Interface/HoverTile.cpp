// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "HoverTile.h"


// Sets default values for this component's properties
UHoverTile::UHoverTile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHoverTile::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHoverTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHoverTile::Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord)
{
	Super::Execute_Implementation(InSourceCoord, bHasHit, InTargetCoord);
	
	SetHoveredTile(InSourceCoord);
	//Return true if changed the value, false if not. It will consider the before and after.
	return bChange;
}

void UHoverTile::SetHoveredTile(FGridCoord NewHoveredTile)
{
	if (HoveredTile != NewHoveredTile)
	{
		HoveredTile = NewHoveredTile;
		HoveredTileData = Grid->GetTileStaticData(NewHoveredTile);
		bChange = true;
	}
	else
	{
		bChange = false;
	}
}

FGridTileStaticData* UHoverTile::GetHoveredTileData() const
{
	return HoveredTileData;
}

