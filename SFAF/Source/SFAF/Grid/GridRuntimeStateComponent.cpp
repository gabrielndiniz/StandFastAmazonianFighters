// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "GridRuntimeStateComponent.h"
#include "GridCoord.h"
#include "GridMathLibrary.h"
#include "Combatant/Combatant_Base.h"
#include "Components/InstancedStaticMeshComponent.h"

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

void UGridRuntimeStateComponent::RegisterTile(const FGridCoord& Coord, const FGridTileStaticData& Data)
{
    StaticTiles.Add(Coord, Data);
}

void UGridRuntimeStateComponent::RemoveTile(const FGridCoord& Coord)
{
    StaticTiles.Remove(Coord);
}

const FGridTileStaticData* UGridRuntimeStateComponent::GetStaticTile(const FGridCoord& Coord) const
{
    return StaticTiles.Find(Coord);
}

FGridTileStaticData* UGridRuntimeStateComponent::GetMutableStaticTile(const FGridCoord& Coord)
{
    return StaticTiles.Find(Coord);
}

bool UGridRuntimeStateComponent::BP_GetStaticTile(const FGridCoord& Coord, FGridTileStaticData& OutTileData) const
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

bool UGridRuntimeStateComponent::HasTile(const FGridCoord& Coord) const
{
    return StaticTiles.Contains(Coord);
}

void UGridRuntimeStateComponent::SetTileOccupancy(const FGridCoord& Coord, const FGridTileOccupancy& Occupancy)
{
    if (FGridTileStaticData* Tile = StaticTiles.Find(Coord))
    {
        Tile->Occupancy = Occupancy;
    }
}

FGridTileOccupancy UGridRuntimeStateComponent::GetTileOccupancy(const FGridCoord& Coord) const
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
    ClearTacticalData();
}

TArray<FGridCoord> UGridRuntimeStateComponent::GetTilesWithStaticData() const
{
    TArray<FGridCoord> Result;

    StaticTiles.GetKeys(Result);

    return Result;
}

int32 UGridRuntimeStateComponent::GetTilePathCost(bool bConsiderFlying, FGameplayTagContainer DataTags)
{
    if (DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Type.Blocked")))
    {
        return 999;
    }

    int32 Cost = 1;

    if (DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Cost.Double")))
    {
        Cost = 2;
    }
    else if (DataTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Cost.Triple")))
    {
        Cost = 3;
    }

    if (bConsiderFlying)
    {
        Cost = 1;
    }
    
    return Cost;
}

int32 UGridRuntimeStateComponent::GetNearestTileFromTargetPosition(const TArray<FGridCoord>& Positions, FGridCoord Target,
                                                                   bool bConsiderFlying) const
{
    return UGridMathLibrary::FindNearestTileIndex(
        Positions,
        StaticTiles,
        Target,
        bConsiderFlying
    );
}

FGameplayTagContainer UGridRuntimeStateComponent::GetTileTags(FGridCoord Target) const
{
    if (const FGridTileStaticData* Tile = GetStaticTile(Target))
    {
        return Tile->TileTags;
    }

    return FGameplayTagContainer();
}

FVector UGridRuntimeStateComponent::GetGridCenterLocation() const
{
    //TODO: Consider if spawn around location instead.
    
    // Retrieve static data for both corner tiles
    const FGridTileStaticData* FirstData = GetStaticTile(FirstTile);
    const FGridTileStaticData* LastData = GetStaticTile(LastTile);

    // If both exist, return the average (midpoint) of their world locations
    if (FirstData && LastData)
    {
        return (FirstData->WorldLocation + LastData->WorldLocation) * 0.5f;
    }

    // Fallback if only one is valid
    if (LastData) return LastData->WorldLocation;
    if (FirstData) return FirstData->WorldLocation;

    return FVector::ZeroVector;
}

FVector UGridRuntimeStateComponent::GetBottomLocation() const
{
    //TODO: Consider if spawn around location instead.
    
    // Retrieve static data for both corner tiles
    const FGridTileStaticData* FirstData = GetStaticTile(FirstTile);
    
    if (FirstData) return FirstData->WorldLocation;

    return FVector::ZeroVector;
}

// ---------------------------------------------------------------------------
// Runtime State API
// ---------------------------------------------------------------------------

void UGridRuntimeStateComponent::RegisterTacticalMesh(FGameplayTag ModifierTag, UInstancedStaticMeshComponent* Mesh)
{
    if (ModifierTag.IsValid() && Mesh)
    {
        TacticalModifiersMeshes.Add(ModifierTag, Mesh);
    }
}

UInstancedStaticMeshComponent* UGridRuntimeStateComponent::SelectTacticMeshWithTag(FGameplayTag ModifierTag) const
{
    if (const TObjectPtr<UInstancedStaticMeshComponent>* FoundMesh = TacticalModifiersMeshes.Find(ModifierTag))
    {
        return FoundMesh->Get();
    }
    return nullptr;
}

void UGridRuntimeStateComponent::AddTacticalModifierPosition(const FGridCoord& Coord, FGameplayTag ModifierTag)
{
    if (ModifierTag.IsValid())
    {
        TacticalModifiersPositions.Add(Coord, ModifierTag);
    }
}

void UGridRuntimeStateComponent::RemoveTacticalModifierPosition(const FGridCoord& Coord)
{
    TacticalModifiersPositions.Remove(Coord);
}

bool UGridRuntimeStateComponent::GetTileModifier(const FGridCoord& Coord, FGameplayTag& OutModifier) const
{
    if (const FGameplayTag* FoundTag = TacticalModifiersPositions.Find(Coord))
    {
        OutModifier = *FoundTag;
        return true;
    }
    OutModifier = FGameplayTag::EmptyTag;
    return false;
}

void UGridRuntimeStateComponent::ClearTacticalData()
{
    TacticalModifiersPositions.Empty();
    // We don't want to clear the TacticalModifiersMeshes map because it's populated in BeginPlay.
    // Instead, we clear all instances from all registered meshes.
    for (auto& Pair : TacticalModifiersMeshes)
    {
        if (Pair.Value)
        {
            Pair.Value->ClearInstances();
        }
    }
}

bool UGridRuntimeStateComponent::IsTileTypeWalkable(const FGameplayTagContainer TileTags) const
{
    if (TileTags.IsEmpty()) return false;    
    
    return TileTags.HasTagExact(FGameplayTag::RequestGameplayTag("Grid.Type.Walkable"));
}

bool UGridRuntimeStateComponent::AddTileState(const FGridCoord& Coord, EGridTileStateType StateType)
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

bool UGridRuntimeStateComponent::RemoveTileState(const FGridCoord& Coord, EGridTileStateType StateType)
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

    if (TSet<FGridCoord>* Cache = TileStateCache.Find(StateType))
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

FGridTileRuntimeState UGridRuntimeStateComponent::GetTileState(const FGridCoord& Coord) const
{
    if (const FGridTileRuntimeState* State = TileStates.Find(Coord))
    {
        return *State;
    }
    return FGridTileRuntimeState();
}

TArray<FGridCoord> UGridRuntimeStateComponent::GetTilesByState(EGridTileStateType StateType) const
{
    if (const TSet<FGridCoord>* Cache = TileStateCache.Find(StateType))
    {
        return Cache->Array();
    }
    return TArray<FGridCoord>();
}

void UGridRuntimeStateComponent::ClearAllStates()
{
    TileStates.Empty();
    TileStateCache.Empty();
}
