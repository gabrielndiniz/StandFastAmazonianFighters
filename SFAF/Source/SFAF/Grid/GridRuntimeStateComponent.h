// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GridCoord.h"
#include "GridRuntimeStateComponent.generated.h"

class ACombatant_Base;
class UAbilitySystemComponent;

// ---------------------------------------------------------------------------
// Enums
// ---------------------------------------------------------------------------

UENUM(BlueprintType)
enum class EGridTileStateType : uint8
{
    None,
    Hovered,
    Selected,
    InPath,
    Neighbor,
    Discovered,
    Analyzed
};

// ---------------------------------------------------------------------------
// Structs
// ---------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FGridTileRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bInPath = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsNeighbor = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsDiscovered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsAnalyzed = false;
};

USTRUCT(BlueprintType)
struct FGridTileOccupancy
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TObjectPtr<ACombatant_Base> OccupyingUnit = nullptr;

    // Occupancy just have the unit. The reason to maintain the UStruct is to make it expensive later if needed.
    // For example, I could put things that are not Base Combatant, like an Hazard.
};

/** Static data —  created on GenerateGrid, update only after unit movement */
USTRUCT(BlueprintType)
struct FGridTileStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector WorldLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 InstanceIndex = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FGameplayTagContainer TileTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FGridTileOccupancy Occupancy;
        
};

// ---------------------------------------------------------------------------
// Component
// ---------------------------------------------------------------------------

/** 
 * Component responsible for managing the runtime state of the grid.
 * Tracks static tile data (location, tags, occupancy) and dynamic states (selection, paths).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridRuntimeStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGridRuntimeStateComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // -----------------------------------------------------------------------
    // Static Tile API
    // -----------------------------------------------------------------------

    /** Register Static Data of Tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void RegisterTile(const FGridCoord& Coord, const FGridTileStaticData& Data);
    
    /** Remove a specific tile*/
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void RemoveTile(const FGridCoord& Coord);

    /** Returns static data from tile (const) */
    const FGridTileStaticData* GetStaticTile(const FGridCoord& Coord) const;

    /** Returns static data from tile (not const) */
    FGridTileStaticData* GetMutableStaticTile(const FGridCoord& Coord);
    
    /** Returns static data from tile (Blueprint) */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool BP_GetStaticTile(const FGridCoord& Coord, FGridTileStaticData& OutTileData) const;

    /** Verify if tile exists */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool HasTile(const FGridCoord& Coord) const;

    /** Update tile occupancy */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void SetTileOccupancy(const FGridCoord& Coord, const FGridTileOccupancy& Occupancy);

    /** Returns tile occupancy */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGridTileOccupancy GetTileOccupancy(const FGridCoord& Coord) const;

    /** Clear all tiles */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void ClearAllTiles();
    
    /** Best way to have all points */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    TArray<FGridCoord> GetTilesWithStaticData () const;
    
    /** Returns the cost according to DataTag */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    static int32 GetTilePathCost(bool bConsiderFlying, FGameplayTagContainer DataTags);

    /** Given an list, closest position considering costs. Returns its index on the list */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    int32 GetNearestTileFromTargetPosition (const TArray<FGridCoord>& Positions, FGridCoord Target, bool bConsiderFlying) const;
    
    /** Given a list, the closest position considering costs. Returns its index on the list */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGameplayTagContainer GetTileTags (FGridCoord Target) const;
    
    /** Return true if Coord is fly only */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool GetTileFlyOnly (FGridCoord Coord) const;
    bool GetTileCost(FGridCoord Coord, int32& Cost) const;

    /** Give the center location of the Grid */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FVector GetGridCenterLocation () const;

    /** Give the center location of the Grid */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FVector GetBottomLocation () const;
    
    /** Give the first spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    FGridCoord GetFirstTile() const { return FirstTile; }

    /** Set the first spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    void SetFirstTile(const FGridCoord& NewFirstTile) { FirstTile = NewFirstTile; }

    /** Give the last spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    FGridCoord GetLastTile() const { return LastTile; }

    /** Set the last spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    void SetLastTile(const FGridCoord& NewLastTile) { LastTile = NewLastTile; }

    /** Register a mesh for a specific tactical modifier tag */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void RegisterTacticalMesh(FGameplayTag ModifierTag, UInstancedStaticMeshComponent* Mesh);

    /** Returns tactical mesh according to tag */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    UInstancedStaticMeshComponent* SelectTacticMeshWithTag(FGameplayTag ModifierTag) const;

    /** Add a tactical modifier tag to a position */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void AddTacticalModifierPosition(const FGridCoord& Coord, FGameplayTag ModifierTag);

    /** Remove a tactical modifier from a position */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void RemoveTacticalModifierPosition(const FGridCoord& Coord);

    /** Returns modifier tag on a position */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    bool GetTileModifier(const FGridCoord& Coord, FGameplayTag& OutModifier) const;
    
    /** Clear all tactical data */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void ClearTacticalData();
    
    /** Return true if tile is walkable */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    bool IsTileTypeWalkable(const FGameplayTagContainer TileTags) const;
        
    // -----------------------------------------------------------------------
    // Runtime State API
    // -----------------------------------------------------------------------

    /** Apply a state to a tile and update the cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool AddTileState(const FGridCoord& Coord, EGridTileStateType StateType);

    /** Remove the state of a tile and update the cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool RemoveTileState(const FGridCoord& Coord, EGridTileStateType StateType);

    /** Returns the current state of a tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    FGridTileRuntimeState GetTileState(const FGridCoord& Coord) const;

    /** Returns all tiles on the picked state */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    TArray<FGridCoord> GetTilesByState(EGridTileStateType StateType) const;

    /** Clear All Dynamic States */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    void ClearAllStates();

    // -----------------------------------------------------------------------
    // Data
    // -----------------------------------------------------------------------

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGridTileStaticData> StaticTiles;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGridTileRuntimeState> TileStates;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGameplayTag, TObjectPtr<UInstancedStaticMeshComponent>> TacticalModifiersMeshes;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGameplayTag> TacticalModifiersPositions;
    
    TMap<EGridTileStateType, TSet<FGridCoord>> TileStateCache;    
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile", meta = (AllowPrivateAccess = "true"))
    FGridCoord FirstTile = FGridCoord(TNumericLimits<int32>::Max(), 0);
	
    UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile", meta = (AllowPrivateAccess = "true"))
    FGridCoord LastTile = FGridCoord(0, 0);
    
};
