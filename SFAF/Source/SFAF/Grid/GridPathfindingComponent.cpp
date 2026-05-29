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
                                                   const TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent, 
                                                   const bool bIsFlying)
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
    
    if (!UGridMathLibrary::GetHexNeighborTiles(Coord, Results))
    {
        return false;
    }

    for (FGridCoord Result : Results)
    {
        if (!RuntimeStateComponent->HasTile(Result)) { continue; }
        
        FGridTileStaticData* TileStaticData = RuntimeStateComponent->GetMutableStaticTile(Result);
        
        if (TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Blocked"))) 
            || TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Obstacle")))
           )
        {
            continue;
        }
        
        if (bIsFlying || !TileStaticData->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.FlyingOnly"))))
        {
            NeighborsCoords.AddUnique(Result);
        }
    }

    if (!NeighborsCoords.IsEmpty())
    {
        FTilePathfindData TilePathfindData;
        TilePathfindData.Neighbors = NeighborsCoords;
        TilePathfindData.bFlyOnly = RuntimeStateComponent->GetTileFlyOnly(Coord);
        RuntimeStateComponent->GetTileCost(Coord, TilePathfindData.Cost);

        TilePathfindMap.Add(Coord, TilePathfindData);
    }

    return !NeighborsCoords.IsEmpty();
}


void UGridPathfindingComponent::ClearPathfindingCache()
{
	TilePathfindMap.Empty();
}

bool UGridPathfindingComponent::GetAllReachableCoords(FGridCoord Coord, TArray<FGridCoord>& ReachableCoords,
	const int32 Points, TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent)
{
	if (!RuntimeStateComponent) { return false; }
	
	if (!RuntimeStateComponent->HasTile(Coord)) { return false; }
	
	ReachableCoords.Empty();
	
	int32 TotalCost = 0;
	
	// Setting selected tile
	for (FGridCoord TileCoord : RuntimeStateComponent->GetTilesByState(EGridTileStateType::Selected))
	{
		RuntimeStateComponent->RemoveTileState(TileCoord, EGridTileStateType::Selected);		
	}
	RuntimeStateComponent->AddTileState(Coord, EGridTileStateType::Selected);
	
	TQueue<FGridCoord> OpenList;
	OpenList.Enqueue(Coord);

	while (!OpenList.IsEmpty())
	{
		FGridCoord CurrentCoord;
		OpenList.Dequeue(CurrentCoord);

		if (TotalCost > Points)
		{
			break; // If cost is enough, break loop
		}

		if (TilesPaths.Contains(CurrentCoord))
		{
			continue; // Ignore if already analyzed
		}

		FPathNode PathNode;
		PathNode.Coord = CurrentCoord;
		int32 CostSoFar = 0;

		if (TilePathfindMap.Contains(CurrentCoord))
		{
			CostSoFar = TilePathfindMap[CurrentCoord].Cost;
		}
		
		TotalCost += CostSoFar;

		TilesPaths.Add(CurrentCoord, PathNode);

		TArray<FGridCoord> NeighborsCoords;
		if (!GetNeighborsCoords(CurrentCoord, NeighborsCoords, RuntimeStateComponent, false))
		{
			continue; // If there is no neighbor, go to next
		}

		for (FGridCoord Neighbor : NeighborsCoords)
		{
			if (TotalCost + TilePathfindMap[Neighbor].Cost > Points)
			{
				continue; // Ignore if overreach points
			}

			if (!TilesPaths.Contains(Neighbor))
			{
				OpenList.Enqueue(Neighbor);
			}
		}
	}

	TilesPaths.GetKeys(ReachableCoords);

	return !ReachableCoords.IsEmpty();
}


