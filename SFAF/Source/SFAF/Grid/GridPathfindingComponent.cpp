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

bool UGridPathfindingComponent::GetNeighborsCoords(
    const FGridCoord Coord,
    TArray<FGridCoord>& NeighborsCoords,
    const TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent,
    const bool bIsFlying)
{
    if (!RuntimeStateComponent) return false;
    if (!RuntimeStateComponent->HasTile(Coord)) return false;

    NeighborsCoords.Empty();

    // Cache hit
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

    for (const FGridCoord& Result : Results)
    {
        if (Result.X == Coord.X && Result.Y == Coord.Y) continue;
        if (!RuntimeStateComponent->HasTile(Result)) continue;

        const FGridTileStaticData* TileStaticData = RuntimeStateComponent->GetStaticTile(Result);
        if (!TileStaticData) continue;

        const FGameplayTagContainer& Tags = TileStaticData->TileTags;

        // Blocked e Obstacle 
        if (Tags.HasTag(FGameplayTag::RequestGameplayTag("Grid.Type.Blocked")) ||
            Tags.HasTag(FGameplayTag::RequestGameplayTag("Grid.Type.Obstacle")))
        {
            continue;
        }

        // FlyingOnly 
        if (Tags.HasTag(FGameplayTag::RequestGameplayTag("Grid.Type.FlyingOnly")) && !bIsFlying)
        {
            continue;
        }

        

        NeighborsCoords.AddUnique(Result);
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


bool UGridPathfindingComponent::GetAllReachableCoords(
    FGridCoord Coord,
    TArray<FGridCoord>& ReachableCoords,
    const int32 Points,
    const bool bIsFlying,
    TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent)
{
    if (!RuntimeStateComponent) return false;
    if (!RuntimeStateComponent->HasTile(Coord)) return false;

    // Invalidate cache if change flying or origin coord
    // Points do not invalidate cache from neighbors
    if (bIsFlying != bLastIsFlying || !(Coord == LastCoord))
    {
        TilePathfindMap.Empty();
        bLastIsFlying = bIsFlying;
        LastCoord = Coord;
    }

    // TilesPaths recalculates always, even changing points
    ReachableCoords.Empty();
    TilesPaths.Empty();

    TMap<FGridCoord, int32> CostSoFar;
    TArray<TPair<int32, FGridCoord>> OpenList;

    auto Enqueue = [&](int32 Cost, FGridCoord C)
    {
        int32 InsertIndex = OpenList.Num();
        for (int32 i = 0; i < OpenList.Num(); ++i)
        {
            if (Cost < OpenList[i].Key)
            {
                InsertIndex = i;
                break;
            }
        }
        OpenList.Insert(TPair<int32, FGridCoord>(Cost, C), InsertIndex);
    };

    CostSoFar.Add(Coord, 0);
    Enqueue(0, Coord);

    for (FGridCoord TileCoord : RuntimeStateComponent->GetTilesByState(EGridTileStateType::Selected))
    {
        RuntimeStateComponent->RemoveTileState(TileCoord, EGridTileStateType::Selected);
    }
    RuntimeStateComponent->AddTileState(Coord, EGridTileStateType::Selected);

    while (OpenList.Num() > 0)
    {
        TPair<int32, FGridCoord> Current = OpenList[0];
        OpenList.RemoveAt(0);

        const int32 CurrentCost = Current.Key;
        const FGridCoord CurrentCoord = Current.Value;

        if (TilesPaths.Contains(CurrentCoord)) continue;

        FPathNode PathNode;
        PathNode.Coord = CurrentCoord;
        PathNode.CostSoFar = CurrentCost;
        TilesPaths.Add(CurrentCoord, PathNode);

        TArray<FGridCoord> NeighborsCoords;
        GetNeighborsCoords(CurrentCoord, NeighborsCoords, RuntimeStateComponent, bIsFlying);

        for (const FGridCoord& Neighbor : NeighborsCoords)
        {
            if (Neighbor == Coord) continue;
            if (TilesPaths.Contains(Neighbor)) continue;

            int32 NeighborEntryCost = 1;
            if (TilePathfindMap.Contains(Neighbor))
            {
                NeighborEntryCost = TilePathfindMap[Neighbor].Cost;
            }

            const int32 NewCost = CurrentCost + NeighborEntryCost;
            if (NewCost > Points) continue;

            const int32* ExistingCost = CostSoFar.Find(Neighbor);
            if (!ExistingCost || NewCost < *ExistingCost)
            {
                CostSoFar.Add(Neighbor, NewCost);
                Enqueue(NewCost, Neighbor);
            }
        }
    }


    for (FGridCoord ReachableCoord : ReachableCoords)
    {
        RuntimeStateComponent->RemoveTileState(ReachableCoord, EGridTileStateType::Neighbor);
        RuntimeStateComponent->RemoveTileState(ReachableCoord, EGridTileStateType::Analyzed);
        RuntimeStateComponent->RemoveTileState(ReachableCoord, EGridTileStateType::InPath);
        RuntimeStateComponent->RemoveTileState(ReachableCoord, EGridTileStateType::Discovered);
        RuntimeStateComponent->AddTileState(ReachableCoord, EGridTileStateType::None);        
    }
    ReachableCoords.Remove(Coord);
    
    TilesPaths.GetKeys(ReachableCoords);
    
    return !ReachableCoords.IsEmpty();
}


