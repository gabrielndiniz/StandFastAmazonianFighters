// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combatant/Combatant_Base.h"
#include "GameplayTagContainer.h"
#include "GridRuntimeStateComponent.generated.h"

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
    bool bHovered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bSelected = false;

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
    void RegisterTile(const FIntPoint& Coord, const FGridTileStaticData& Data);

    /** Returns static data from tile (const) */
    const FGridTileStaticData* GetStaticTile(const FIntPoint& Coord) const;

    /** Returns static data from tile (not const) */
    FGridTileStaticData* GetMutableStaticTile(const FIntPoint& Coord);
    
    /** Returns static data from tile (Blueprint) */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool BP_GetStaticTile(const FIntPoint& Coord, FGridTileStaticData& OutTileData) const;

    /** Verify if tile exists */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    bool HasTile(const FIntPoint& Coord) const;

    /** Update tile occupancy */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void SetTileOccupancy(const FIntPoint& Coord, const FGridTileOccupancy& Occupancy);

    /** Returns tile occupancy */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGridTileOccupancy GetTileOccupancy(const FIntPoint& Coord) const;

    /** Clear all tiles */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    void ClearAllTiles();
    
    /** Best way to have all points */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    TArray<FIntPoint> GetTilesWithStaticData () const;
    
    /** Returns the cost according to DataTag */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    static int32 GetTilePathCost(bool bConsiderFlying, FGameplayTagContainer DataTags);

    /** Given an list, closest position considering costs. Returns its index on the list */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    int32 GetNearestTileFromTargetPosition (const TArray<FIntPoint>& Positions, FIntPoint Target, bool bConsiderFlying) const;
    
    /** Given an list, closest position considering costs. Returns its index on the list */
    UFUNCTION(BlueprintCallable, Category = "Grid|Static")
    FGameplayTagContainer GetTileTags (FIntPoint Target) const;
        
    // -----------------------------------------------------------------------
    // Runtime State API
    // -----------------------------------------------------------------------

    /** Apply a state to a tile and update the cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool AddTileState(const FIntPoint& Coord, EGridTileStateType StateType);

    /** Remove the state of a tile and update the cache */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    bool RemoveTileState(const FIntPoint& Coord, EGridTileStateType StateType);

    /** Returns the current state of a tile */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    FGridTileRuntimeState GetTileState(const FIntPoint& Coord) const;

    /** Returns all tiles on the picked state */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    TArray<FIntPoint> GetTilesByState(EGridTileStateType StateType) const;

    /** Clear All Dynamic States */
    UFUNCTION(BlueprintCallable, Category = "Grid|State")
    void ClearAllStates();

    // -----------------------------------------------------------------------
    // Data
    // -----------------------------------------------------------------------

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FIntPoint, FGridTileStaticData> StaticTiles;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TMap<FIntPoint, FGridTileRuntimeState> TileStates;
    
    TMap<EGridTileStateType, TSet<FIntPoint>> TileStateCache;
};
