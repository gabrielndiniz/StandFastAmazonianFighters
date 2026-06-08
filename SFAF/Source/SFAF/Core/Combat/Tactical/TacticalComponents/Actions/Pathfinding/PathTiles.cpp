// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "PathTiles.h"


// Sets default values for this component's properties
UPathTiles::UPathTiles()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathTiles::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPathTiles::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPathTiles::SetLocationsForMeshes_Implementation()
{
	LocationsForMeshes.Empty();	
	PathTiles.Empty();
		
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grid is invalid, returning false"));
		return false;
	}
	
	
	Grid->GetPathCoords(SourceCoord, TargetCoord, PathTiles);
	
	for (FGridCoord PathTile : PathTiles)
	{
	
		FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(PathTile);
		LocationsForMeshes.Add(TileStaticData->WorldLocation);
	}
	UE_LOG(LogTemp, Warning, TEXT("PathTile finish getting coords"));

	return !LocationsForMeshes.IsEmpty();
}

TArray<FGridCoord> UPathTiles::GetPathTiles()
{
	return PathTiles;
}

