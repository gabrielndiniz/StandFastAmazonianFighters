// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "Grid/GridRuntimeStateComponent.h"

UGridRuntimeStateComponent::UGridRuntimeStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGridRuntimeStateComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UGridRuntimeStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// ---------------------------------------------------------------------------
// Static Tile API
// ---------------------------------------------------------------------------

void UGridRuntimeStateComponent::RegisterTile(const FIntPoint& Coord, const FGridTileStaticData& Data)
{
    StaticTiles.Add(Coord, Data);
}

void UGridRuntimeStateComponent::RemoveTile(const FIntPoint& Coord)
{
    StaticTiles.Remove(Coord);
}

const FGridTileStaticData* UGridRuntimeStateComponent::GetStaticTile(const FIntPoint& Coord) const
{
    return StaticTiles.Find(Coord);
}

FGridTileStaticData* UGridRuntimeStateComponent::GetMutableStaticTile(const FIntPoint& Coord)
{
    return StaticTiles.Find(Coord);
}

bool UGridRuntimeStateComponent::BP_GetStaticTile(const FIntPoint& Coord, FGridTileStaticData& OutTileData) const
{
    const FGridTileStaticData* FoundTile =
        StaticTiles.Find(Coord);

    if (!FoundTile)
    {
        return false;
    }

    OutTileData = *FoundTile;

    return true;
}

bool UGridRuntimeStateComponent::HasTile(const FIntPoint& Coord) const
{
    return StaticTiles.Contains(Coord);
}

void UGridRuntimeStateComponent::SetTileOccupancy(const FIntPoint& Coord, const FGridTileOccupancy& Occupancy)
{
    if (FGridTileStaticData* Tile = StaticTiles.Find(Coord))
    {
        Tile->Occupancy = Occupancy;
    }
}

FGridTileOccupancy UGridRuntimeStateComponent::GetTileOccupancy(const FIntPoint& Coord) const
{
    if (const FGridTileStaticData* Tile = StaticTiles.Find(Coord))
    {
        return Tile->Occupancy;
    }
    return FGridTileOccupancy();
}

void UGridRuntimeStateComponent::ClearAllTiles()
{
    StaticTiles.Empty();
    ClearAllStates();
}

TArray<FIntPoint> UGridRuntimeStateComponent::GetTilesWithStaticData() const
{
    TArray<FIntPoint> Result;

    StaticTiles.GetKeys(Result);

    return Result;
}

int32 UGridRuntimeStateComponent::GetTilePathCost(bool bConsiderFlying, FGameplayTagContainer DataTags)
{
    int32 Cost = 1;

    if (!bConsiderFlying)
    {
        if (DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Cost.Double")))
        {
            Cost = 2;
        }
        else if (DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Cost.Triple")) || 
                 DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Type.Blocked")))
        {
            Cost = 3;
        }
    }
    return Cost;
}

int32 UGridRuntimeStateComponent::GetNearestTileFromTargetPosition(const TArray<FIntPoint>& Positions, FIntPoint Target,
                                                                   bool bConsiderFlying) const
{
    if (!HasTile(Target))
    {
        return INDEX_NONE;
    }

    if (Positions.IsEmpty())
    {
        return INDEX_NONE;
    }

    const FGridTileStaticData* TargetTile = GetStaticTile(Target);

    if (!TargetTile)
    {
        return INDEX_NONE;
    }

    const FVector& TargetLocation = TargetTile->WorldLocation;

    float BestDistance = TNumericLimits<float>::Max();

    int32 BestIndex = INDEX_NONE;

    for (int32 Index = 0; Index < Positions.Num(); ++Index)
    {
        const FIntPoint& TileCoord = Positions[Index];

        const FGridTileStaticData* TileData =
            GetStaticTile(TileCoord);

        if (!TileData)
        {
            continue;
        }
        
        FGameplayTagContainer DataTags = TileData->TileTags;

        int32 CostMultiplier = GetTilePathCost(bConsiderFlying, DataTags);

        const float Distance =
            FVector::Distance(TargetLocation, TileData->WorldLocation) * CostMultiplier;

        if (Distance < BestDistance)
        {
            BestDistance = Distance;
            BestIndex = Index;
        }
    }

    return BestIndex;
}

FGameplayTagContainer UGridRuntimeStateComponent::GetTileTags(FIntPoint Target) const
{
    return GetStaticTile(Target)->TileTags;
}

// ---------------------------------------------------------------------------
// Runtime State API
// ---------------------------------------------------------------------------

bool UGridRuntimeStateComponent::AddTileState(const FIntPoint& Coord, EGridTileStateType StateType)
{
    if (!HasTile(Coord)) return false;

    FGridTileRuntimeState& Current = TileStates.FindOrAdd(Coord);

    switch (StateType)
    {
    case EGridTileStateType::Hovered:    Current.bHovered     = true; break;
    case EGridTileStateType::Selected:   Current.bSelected    = true; break;
    case EGridTileStateType::InPath:     Current.bInPath      = true; break;
    case EGridTileStateType::Neighbor:   Current.bIsNeighbor  = true; break;
    case EGridTileStateType::Discovered: Current.bIsDiscovered = true; break;
    case EGridTileStateType::Analyzed:   Current.bIsAnalyzed  = true; break;
    default: return false;
    }

    TileStateCache.FindOrAdd(StateType).Add(Coord);
    return true;
}

bool UGridRuntimeStateComponent::RemoveTileState(const FIntPoint& Coord, EGridTileStateType StateType)
{
    if (!HasTile(Coord)) return false;

    FGridTileRuntimeState* Current = TileStates.Find(Coord);
    if (!Current) return false;

    switch (StateType)
    {
    case EGridTileStateType::Hovered:    Current->bHovered     = false; break;
    case EGridTileStateType::Selected:   Current->bSelected    = false; break;
    case EGridTileStateType::InPath:     Current->bInPath      = false; break;
    case EGridTileStateType::Neighbor:   Current->bIsNeighbor  = false; break;
    case EGridTileStateType::Discovered: Current->bIsDiscovered = false; break;
    case EGridTileStateType::Analyzed:   Current->bIsAnalyzed  = false; break;
    default: return false;
    }

    if (TSet<FIntPoint>* Cache = TileStateCache.Find(StateType))
    {
        Cache->Remove(Coord);
    }

    // Remove entrada vazia do TileStates
    if (!Current->bHovered && !Current->bSelected  && !Current->bInPath &&
        !Current->bIsNeighbor && !Current->bIsDiscovered && !Current->bIsAnalyzed)
    {
        TileStates.Remove(Coord);
    }

    return true;
}

FGridTileRuntimeState UGridRuntimeStateComponent::GetTileState(const FIntPoint& Coord) const
{
    if (const FGridTileRuntimeState* State = TileStates.Find(Coord))
    {
        return *State;
    }
    return FGridTileRuntimeState();
}

TArray<FIntPoint> UGridRuntimeStateComponent::GetTilesByState(EGridTileStateType StateType) const
{
    if (const TSet<FIntPoint>* Cache = TileStateCache.Find(StateType))
    {
        return Cache->Array();
    }
    return TArray<FIntPoint>();
}

void UGridRuntimeStateComponent::ClearAllStates()
{
    TileStates.Empty();
    TileStateCache.Empty();
}
