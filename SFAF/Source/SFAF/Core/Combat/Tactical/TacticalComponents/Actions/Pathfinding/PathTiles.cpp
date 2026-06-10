// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "PathTiles.h"
#include "Grid/GridMathLibrary.h"
#include "Grid/GridRuntimeStateComponent.h"


UPathTiles::UPathTiles()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPathTiles::BeginPlay()
{
	Super::BeginPlay();
}

void UPathTiles::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPathTiles::SetLocationsForMeshes_Implementation()
{
	LocationsForMeshes.Empty();	
	PathTiles.Empty();
	TotalCost = 0;
		
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grid is invalid, returning false"));
		return false;
	}
	
	Grid->GetPathCoords(SourceCoord, TargetCoord, PathTiles);
	
	if (Grid->GridRuntimeStateComponent)
	{
		TotalCost = UGridMathLibrary::GetPathMovementCost(Grid->GridRuntimeStateComponent, PathTiles);
	}
	
	for (FGridCoord PathTile : PathTiles)
	{
		FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(PathTile);
		LocationsForMeshes.Add(TileStaticData->WorldLocation);
	}


	return !LocationsForMeshes.IsEmpty();
}

TArray<FGridCoord> UPathTiles::GetPathTiles()
{
	return PathTiles;
}

int32 UPathTiles::GetTotalCost() const
{
	return TotalCost;
}
