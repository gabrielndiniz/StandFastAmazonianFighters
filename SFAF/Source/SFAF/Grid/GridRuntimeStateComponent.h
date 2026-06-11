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

/** Runtime states a grid tile can be in during tactical gameplay */
UENUM(BlueprintType)
enum class EGridTileStateType : uint8
{
    None,
    /** Tile is currently under the mouse cursor */
    Hovered,
    /** Tile has been clicked/selected by the player */
    Selected,
    /** Tile is part of the current movement path */
    InPath,
    /** Tile is a neighbor of the selected tile */
    Neighbor,
    /** Tile was discovered during pathfinding expansion */
    Discovered,
    /** Tile was analyzed during pathfinding evaluation */
    Analyzed
};

// ---------------------------------------------------------------------------
// Structs
// ---------------------------------------------------------------------------

/** Dynamic runtime flags for a single tile (distinct from the static tile data) */
USTRUCT(BlueprintType)
struct FGridTileRuntimeState
{
    GENERATED_BODY()

    /** Whether this tile is part of the current movement path */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bInPath = false;

    /** Whether this tile is adjacent to the selected tile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsNeighbor = false;

    /** Whether this tile was discovered during pathfinding */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsDiscovered = false;

    /** Whether this tile was analyzed during pathfinding */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsAnalyzed = false;
};

/** Occupancy information for a tile, identifying which combatant (if any) is present */
USTRUCT(BlueprintType)
struct FGridTileOccupancy
{
    GENERATED_BODY()

    /** The combatant currently occupying this tile, or nullptr if unoccupied */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TObjectPtr<ACombatant_Base> OccupyingUnit = nullptr;
};

/**
 * Static tile data created during grid generation.
 * Stores the world location, instance index, gameplay tags, and occupancy for each tile.
 * Updated when units move on or off the tile.
 */
USTRUCT(BlueprintType)
struct FGridTileStaticData
{
    GENERATED_BODY()

    /** World-space position of the tile center */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector WorldLocation = FVector::ZeroVector;

    /** Index of the instanced mesh representing this tile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 InstanceIndex = INDEX_NONE;

    /** Gameplay tags describing tile properties (walkable, cost, visual, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FGameplayTagContainer TileTags;

    /** Occupancy state identifying which unit (if any) is on this tile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FGridTileOccupancy Occupancy;
};

// ---------------------------------------------------------------------------
// Component
// ---------------------------------------------------------------------------

/**
 * Manages all runtime state for the grid system.
 * Tracks static tile data (location, tags, occupancy) and dynamic states
 * (selection, path highlighting, neighbor discovery).
 * Also manages tactical modifier meshes and their positions.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridRuntimeStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Default constructor for the runtime state component */
    UGridRuntimeStateComponent();

protected:
    /** Initializes the component when the game starts */
    virtual void BeginPlay() override;

public:
    /** Updates the component each frame */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    // -----------------------------------------------------------------------
    // Static Tile API
    // -----------------------------------------------------------------------

    /** Registers a tile's static data at the given grid coordinate */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void RegisterTile(const FGridCoord& Coord, const FGridTileStaticData& Data);

    /** Removes a tile and its static data from the registry */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void RemoveTile(const FGridCoord& Coord);

    /** Returns const pointer to tile static data, or nullptr if not found */
    const FGridTileStaticData* GetStaticTile(const FGridCoord& Coord) const;

    /** Returns mutable pointer to tile static data, or nullptr if not found */
    FGridTileStaticData* GetMutableStaticTile(const FGridCoord& Coord);

    /** Blueprint-friendly version of GetStaticTile that copies data out */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool BP_GetStaticTile(const FGridCoord& Coord, FGridTileStaticData& OutTileData) const;

    /** Returns true if a tile is registered at the given coordinate */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool HasTile(const FGridCoord& Coord) const;

    /** Updates the occupancy data for a specific tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void SetTileOccupancy(const FGridCoord& Coord, const FGridTileOccupancy& Occupancy);

    /** Returns the occupancy data for a specific tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGridTileOccupancy GetTileOccupancy(const FGridCoord& Coord) const;

    /** Removes all tile registrations, states, and tactical data */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void ClearAllTiles();

    /** Returns all grid coordinates that have registered static data */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    TArray<FGridCoord> GetTilesWithStaticData() const;

    /** Evaluates movement cost from tile tags, accounting for flying status */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    static int32 GetTilePathCost(bool bConsiderFlying, FGameplayTagContainer DataTags);

    /** Finds the nearest tile index from a list, weighted by cost */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    int32 GetNearestTileFromTargetPosition (const TArray<FGridCoord>& Positions, FGridCoord Target, bool bConsiderFlying) const;

    /** Returns the gameplay tags for a tile at the given coordinate */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGameplayTagContainer GetTileTags (FGridCoord Target) const;

    /** Returns true if the tile at the given coordinate is fly-only */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool GetTileFlyOnly (FGridCoord Coord) const;

    /**
     * Returns the movement cost for a tile, writing it into the output parameter.
     * @param Coord The grid coordinate to query.
     * @param Cost  Output parameter receiving the movement cost.
     * @return True if the tile was found and its cost was retrieved.
     */
    bool GetTileCost(FGridCoord Coord, int32& Cost) const;

    /** Returns the geometric center of the grid based on first and last tile positions */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FVector GetGridCenterLocation () const;

    /** Returns the world location of the first (bottom) tile in the grid */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FVector GetBottomLocation () const;

    /** Returns the coordinate of the first spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    FGridCoord GetFirstTile() const { return FirstTile; }

    /** Sets the coordinate of the first spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    void SetFirstTile(const FGridCoord& NewFirstTile) { FirstTile = NewFirstTile; }

    /** Returns the coordinate of the last spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    FGridCoord GetLastTile() const { return LastTile; }

    /** Sets the coordinate of the last spawned tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    void SetLastTile(const FGridCoord& NewLastTile) { LastTile = NewLastTile; }

    /** Registers an instanced mesh component for a given tactical modifier tag */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void RegisterTacticalMesh(FGameplayTag ModifierTag, UInstancedStaticMeshComponent* Mesh);

    /** Returns the tactical mesh associated with a modifier tag, or nullptr */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    UInstancedStaticMeshComponent* SelectTacticMeshWithTag(FGameplayTag ModifierTag) const;

    /** Records a tactical modifier at a specific grid position */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void AddTacticalModifierPosition(const FGridCoord& Coord, FGameplayTag ModifierTag);

    /** Removes the tactical modifier record at a grid position */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void RemoveTacticalModifierPosition(const FGridCoord& Coord);

    /** Reads the modifier tag at a grid position, returns false if none exists */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    bool GetTileModifier(const FGridCoord& Coord, FGameplayTag& OutModifier) const;

    /** Clears all tactical modifier positions and resets their meshes */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tactical")
    void ClearTacticalData();

    /** Returns true if the given gameplay tags mark the tile as walkable */
    UFUNCTION(BlueprintCallable, Category = "Grid|Tile")
    bool IsTileTypeWalkable(const FGameplayTagContainer TileTags) const;

    // -----------------------------------------------------------------------
    // Runtime State API
    // -----------------------------------------------------------------------

    /** Applies a state flag to a tile and updates the state cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool AddTileState(const FGridCoord& Coord, EGridTileStateType StateType);

    /** Removes a state flag from a tile and updates the state cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool RemoveTileState(const FGridCoord& Coord, EGridTileStateType StateType);

    /** Returns the current runtime state flags for a tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    FGridTileRuntimeState GetTileState(const FGridCoord& Coord) const;

    /** Returns all tiles currently in the given state */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    TArray<FGridCoord> GetTilesByState(EGridTileStateType StateType) const;

    /** Clears all dynamic state flags and cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    void ClearAllStates();

    // -----------------------------------------------------------------------
    // Data
    // -----------------------------------------------------------------------

    /** Map of all registered static tile data keyed by grid coordinate */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGridTileStaticData> StaticTiles;

    /** Map of dynamic runtime state flags keyed by grid coordinate */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGridTileRuntimeState> TileStates;

    /** Maps modifier gameplay tags to their corresponding instanced mesh components */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGameplayTag, TObjectPtr<UInstancedStaticMeshComponent>> TacticalModifiersMeshes;

    /** Maps grid coordinates to their active tactical modifier tags */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FGridCoord, FGameplayTag> TacticalModifiersPositions;

    /** Cache mapping each state type to the set of tiles currently in that state (fast lookups) */
    TMap<EGridTileStateType, TSet<FGridCoord>> TileStateCache;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile", meta = (AllowPrivateAccess = "true"))
    FGridCoord FirstTile = FGridCoord(TNumericLimits<int32>::Max(), 0);

    UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile", meta = (AllowPrivateAccess = "true"))
    FGridCoord LastTile = FGridCoord(0, 0);
};
