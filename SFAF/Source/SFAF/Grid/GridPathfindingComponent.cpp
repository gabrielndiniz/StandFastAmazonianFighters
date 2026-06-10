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

    CostSoFar.Add(Coord, 0);
    OpenList.Emplace(0, Coord);

    for (FGridCoord TileCoord : RuntimeStateComponent->GetTilesByState(EGridTileStateType::Selected))
    {
        RuntimeStateComponent->RemoveTileState(TileCoord, EGridTileStateType::Selected);
    }
    RuntimeStateComponent->AddTileState(Coord, EGridTileStateType::Selected);

    auto GetNeighborsFunc = [&](const FGridCoord& CurrentCoord, TArray<FGridCoord>& OutNeighbors)
    {
        GetNeighborsCoords(CurrentCoord, OutNeighbors, RuntimeStateComponent, bIsFlying);
    };

    UGridMathLibrary::ComputeReachableCoordsWhileLoop(
        Coord,
        Points,
        OpenList,
        CostSoFar,
        TilesPaths,
        TilePathfindMap,
        GetNeighborsFunc
    );


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
    CachedReachableCoords = ReachableCoords;
    
    return !ReachableCoords.IsEmpty();
}

bool UGridPathfindingComponent::GetPathCoords(const FGridCoord Source, const FGridCoord Target,
    TArray<FGridCoord>& PathCoords,
    TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent)
{
    if (!RuntimeStateComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetPathCoords: RuntimeStateComponent is null"));
        return false;
    }
    if (!RuntimeStateComponent->HasTile(Source))
    {
        UE_LOG(LogTemp, Warning, TEXT("GetPathCoords: Source (%d,%d) has no tile"), Source.X, Source.Y);
        return false;
    }
    if (!RuntimeStateComponent->HasTile(Target))
    {
        UE_LOG(LogTemp, Warning, TEXT("GetPathCoords: Target (%d,%d) has no tile"), Target.X, Target.Y);
        return false;
    }
    if (!CachedReachableCoords.Contains(Target))
    {
        UE_LOG(LogTemp, Verbose, TEXT("GetPathCoords: Target (%d,%d) not in CachedReachableCoords (%d tiles)"), Target.X, Target.Y, CachedReachableCoords.Num());
        return false;
    }

   
    struct FPathSearchNode
    {
        int32 TotalCost;
        int32 HopCount;
        int32 LinearityPenalty;
        FGridCoord Coord;

        bool operator<(const FPathSearchNode& Other) const
        {
            if (TotalCost != Other.TotalCost) return TotalCost < Other.TotalCost;
            if (HopCount != Other.HopCount) return HopCount < Other.HopCount;
            return LinearityPenalty < Other.LinearityPenalty;
        }
    };

    TMap<FGridCoord, int32> GScore;
    TMap<FGridCoord, int32> Hops;
    TMap<FGridCoord, FGridCoord> CameFrom;
    TArray<FPathSearchNode> OpenList;

    auto Push = [&](const FPathSearchNode& Node)
    {
        int32 InsertIndex = OpenList.Num();
        for (int32 i = 0; i < OpenList.Num(); ++i)
        {
            if (Node < OpenList[i])
            {
                InsertIndex = i;
                break;
            }
        }
        OpenList.Insert(Node, InsertIndex);
    };

    auto Pop = [&]() -> FPathSearchNode
    {
        FPathSearchNode Node = OpenList[0];
        OpenList.RemoveAt(0);
        return Node;
    };

    GScore.Add(Source, 0);
    Hops.Add(Source, 0);
    Push({UGridMathLibrary::GetHexDistance(Source, Target), 0, 0, Source});

    while (OpenList.Num() > 0)
    {
        FPathSearchNode Current = Pop();
        const FGridCoord CurrentCoord = Current.Coord;

       
        if (CurrentCoord == Target)
        {
            PathCoords.Empty();
            FGridCoord PathTile = Target;
            while (PathTile != Source)
            {
                PathCoords.Insert(PathTile, 0);
                PathTile = CameFrom[PathTile];
            }
            PathCoords.Insert(Source, 0);

            return true;
        }

        TArray<FGridCoord> NeighborsCoords;
        GetNeighborsCoords(CurrentCoord, NeighborsCoords, RuntimeStateComponent, bLastIsFlying);

        if (NeighborsCoords.IsEmpty())
        {
            UE_LOG(LogTemp, Verbose, TEXT("GetPathCoords: No neighbors for (%d,%d)"), CurrentCoord.X, CurrentCoord.Y);
        }

        for (const FGridCoord& Neighbor : NeighborsCoords)
        {
            if (Neighbor == Source)
            {
                continue;
            }
            if (!CachedReachableCoords.Contains(Neighbor) && Neighbor != Target)
            {
                continue;
            }

            int32 StepCost = 1;
            if (TilePathfindMap.Contains(Neighbor))
            {
                StepCost = TilePathfindMap[Neighbor].Cost;
            }

            const int32 NewG = GScore[CurrentCoord] + StepCost;
            const int32* ExistingG = GScore.Find(Neighbor);

            if (!ExistingG || NewG < *ExistingG)
            {
                CameFrom.Add(Neighbor, CurrentCoord);
                GScore.Add(Neighbor, NewG);
                Hops.Add(Neighbor, Hops[CurrentCoord] + 1);

                const int32 Heuristic = UGridMathLibrary::GetHexDistance(Neighbor, Target);
                const int32 Linearity = UGridMathLibrary::GetLinearityPenalty(Source, Target, Neighbor);

                
                Push({
                    NewG + Heuristic,
                    Hops[Neighbor],
                    Linearity,
                    Neighbor
                });
            }
            else
            {
                UE_LOG(LogTemp, VeryVerbose, TEXT("GetPathCoords: Skipping (%d,%d) - not better path (NewG=%d >= Existing=%d)"),
                    Neighbor.X, Neighbor.Y, NewG, *ExistingG);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GetPathCoords: OpenList exhausted - no path from (%d,%d) to (%d,%d)"),
        Source.X, Source.Y, Target.X, Target.Y);
    return false;
}


