// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "ReachableTiles.h"


// Sets default values for this component's properties
UReachableTiles::UReachableTiles()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReachableTiles::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UReachableTiles::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UReachableTiles::SetLocationsForMeshes_Implementation()
{
	LocationsForMeshes.Empty();	
	ReachableTiles.Empty();
		
	Grid->GetReachableCoords(SourceCoord, ReachableTiles, CurrentMovementPoints);

	for (FGridCoord ReachableTile : ReachableTiles)
	{
		FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(ReachableTile);
		
		if (TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Blocked"))) 
			|| TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Obstacle")))
			)
		{
			continue;
		}
		
		if (bConsiderFly || !TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.FlyingOnly"))))
		{
			LocationsForMeshes.Add(TileStaticData->WorldLocation);
		}
	}
	
	return !LocationsForMeshes.IsEmpty();
}

TArray<FGridCoord> UReachableTiles::GetReachableTiles()
{
	return ReachableTiles;
}

void UReachableTiles::SetCurrentMovementPoints(int32 Points)
{
	CurrentMovementPoints = Points;
}

int32 UReachableTiles::GetCurrentMovementPoints()
{
	return CurrentMovementPoints;
}

void UReachableTiles::SetConsiderFly(bool bConsider)
{
	bConsiderFly = bConsider;
}

