// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "GridPathfindingComponent.h"

#include "GridMathLibrary.h"


// Sets default values for this component's properties
UGridPathfindingComponent::UGridPathfindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGridPathfindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGridPathfindingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGridPathfindingComponent::GetNeighborsCoords(const FGridCoord Coord, TArray<FGridCoord>& NeighborsCoords,
	const TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent)
{
	if (!RuntimeStateComponent) { return false; }
	
	if (!RuntimeStateComponent->HasTile(Coord)) { return false; }
	
	NeighborsCoords.Empty();
	
	if (TilePathfindMap.Contains(Coord))
	{
		NeighborsCoords = TilePathfindMap.Find(Coord)->Neighbors;
		return true;
	}
	
	TArray<FGridCoord> Results;
	
	if (!UGridMathLibrary::GetHexNeighborTiles(Coord,Results))
	{
		return false;
	}

	for (FGridCoord Result : Results)
	{
		if (!RuntimeStateComponent->HasTile(Result)) { continue; }
		
		NeighborsCoords.AddUnique(Result);
	}
	
	FTilePathfindData TilePathfindData;
	TilePathfindData.Neighbors = NeighborsCoords;
	TilePathfindData.bFlyOnly = RuntimeStateComponent->GetTileFlyOnly(Coord);
	RuntimeStateComponent->GetTileCost(Coord,TilePathfindData.Cost);
	
	//Adding for not asking again.
	TilePathfindMap.Add(Coord, TilePathfindData);
	
	return !NeighborsCoords.IsEmpty();
}

