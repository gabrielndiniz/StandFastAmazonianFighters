// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "NeighborTile.h"

#include "Grid/GridMathLibrary.h"


// Sets default values for this component's properties
UNeighborTile::UNeighborTile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNeighborTile::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNeighborTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UNeighborTile::SetLocationsForMeshes_Implementation()
{
	LocationsForMeshes.Empty();	
	NeighborsCoords.Empty();
	
	//TODO: Consider if unit selected does not have fly
	bool bFly = true;	
	
	Grid->GetNeighbors(SourceCoord, NeighborsCoords);

	for (FGridCoord NeighborCoord : NeighborsCoords)
	{
		LocationsForMeshes.Add(Grid->GetTileStaticData(NeighborCoord)->WorldLocation);
	}
	
	return !LocationsForMeshes.IsEmpty();
}

TArray<FGridCoord> UNeighborTile::GetNeighbors()
{
	return NeighborsCoords;
}

