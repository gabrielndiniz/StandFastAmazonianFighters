// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "GridRuntimeStateComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridMathLibrary.generated.h"

/** Cached pathfinding data for a single tile (neighbors and movement cost) */
USTRUCT()
struct FTilePathfindData
{
    GENERATED_BODY()

    /** List of neighbor coordinates */
    TArray<FGridCoord> Neighbors;

    /** Movement cost to enter this tile */
    int32 Cost = 1;

    /** Whether only flying units can occupy this tile */
    bool bFlyOnly = false;
};

/** Internal node used by the A* pathfinding algorithm */
struct FPathNode
{
    /** Grid coordinate this node represents */
    FGridCoord Coord;

    /** Accumulated cost from the start node */
    int CostSoFar = 0;

    /** Estimated remaining cost to the goal (heuristic) */
    int Heuristic = 0;

    /** Sum of CostSoFar + Heuristic */
    int TotalCost = 0;

    /** Whether this node has already been evaluated */
    bool IsClosed = false;

    /** Pointer to the predecessor node for reconstructing the path */
    TSharedPtr<FPathNode> Parent = nullptr;

    /** Comparison operator for the priority queue (lower TotalCost = higher priority) */
    bool operator<(const FPathNode& Other) const {
        return TotalCost < Other.TotalCost;
    }
};

UCLASS()
class SFAF_API UGridMathLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** 
     * Converts hex grid offset coordinates to world space position.
     * @param GridCoord The coordinates on the grid.
     * @param GridOrigin The world space origin of the grid.
     * @param TileSize The size/dimensions of a single tile.
     * @param ZCorrection Vertical offset to apply to the final location.
     * @return The calculated world space location of the tile center.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexOffsetToWorld(FGridCoord GridCoord, FVector GridOrigin, FVector TileSize, float ZCorrection);

    /** 
     * Given a world position, estimate the closest tile possible.
     * @param Position The world-space position to snap.
     * @param TileSize The size/dimensions of a single tile.
     * @return The snapped world-space position.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexFindNearestTilePositionOnXYPlane(FVector Position, const FVector TileSize);

    /** Converts a world-space position to hex offset grid coordinates */
    static FGridCoord HexWorldToOffsetCoord(const FVector& WorldPos, const FVector& GridOrigin, const FVector& TileSize);

    /** 
     * Given an list, closest position considering costs. Returns its index on the list
     * @param Positions List of potential coordinates
     * @param StaticTiles Map of grid static data
     * @param Target The target coordinate
     * @param bConsiderFlying If true, ignores tile costs
     * @return The index in the Positions array
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 FindNearestTileIndex(
        const TArray<FGridCoord>& Positions,
        const TMap<FGridCoord, FGridTileStaticData>& StaticTiles,
        const FGridCoord& Target,
        bool bConsiderFlying
    );
    
    /** 
    * Snaps a world-space position to the nearest valid hex grid position.
    * Uses only mathematical conversion and does not depend on tile existence.
    * @param WorldPosition The world-space position to snap.
    * @param GridOrigin The world origin/pivot of the grid.
    * @param TileSize The size/dimensions of a single tile.
    * @return The snapped world-space position on the hex grid.
    */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexSnapWorldToGrid(
        const FVector& WorldPosition,
        const FVector& GridOrigin,
        const FVector& TileSize
    );
    
    /**
    * Finds the nearest registered tile from a world position.
    * @param WorldPosition World-space position to search from.
    * @param StaticTiles Map of grid static data.
    * @param OutCoord Closest tile coordinate.
    * @param OutTileData Closest tile static data.
    * @return True if a valid tile was found.
    */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool FindNearestTileFromWorldPosition(
        const FVector& WorldPosition,
        const TMap<FGridCoord, FGridTileStaticData>& StaticTiles,
        FGridCoord& OutCoord,
        FGridTileStaticData& OutTileData
    );
    
    /**
    * Return true if the number is even.
    * @param Number is the analyzed number.
    */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool IsItEven(int32 Number);
    
    /**
    * Finds the neighbors of a tile.
    * @param Coord Considered coordinate.
    * @param StaticTiles Map of grid static data.
    */
    /** Finds the six neighboring coordinates for a given hex tile */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool GetHexNeighborTiles(
        const FGridCoord Coord,
        TArray<FGridCoord>& NeighborsCoords
    );
    
    /** Returns true if both coordinates share the same row (same Y) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool IsInTheSameLine(FGridCoord Coord1, FGridCoord Coord2);

    /** Returns the hex grid distance between two coordinates (admissible heuristic for A*) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetHexDistance(const FGridCoord& A, const FGridCoord& B);

    /**
     * Returns a linearity penalty for a tile relative to the Source->Target line.
     * Lower values mean the tile is closer to the direct line (more linear path).
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetLinearityPenalty(const FGridCoord& Source, const FGridCoord& Target, const FGridCoord& Tile);

    /**
     * BFS expansion loop for reachable coordinate computation.
     * Processes the OpenList, expanding neighbors via GetNeighborsFunc,
     * tracking visited nodes in TilesPaths, and respecting the movement budget.
     */
    static void ComputeReachableCoordsWhileLoop(
        const FGridCoord& StartCoord,
        int32 MaxPoints,
        TArray<TPair<int32, FGridCoord>>& OpenList,
        TMap<FGridCoord, int32>& CostSoFar,
        TMap<FGridCoord, FPathNode>& TilesPaths,
        const TMap<FGridCoord, FTilePathfindData>& TilePathfindMap,
        const TFunction<void(const FGridCoord&, TArray<FGridCoord>&)>& GetNeighborsFunc
    );
    
    /** Sorts an unsorted path tile array from Source to Target using neighbor traversal */
    static TArray<FGridCoord> SortPathTiles(
        const FGridCoord& Source,
        const FGridCoord& Target,
        const TArray<FGridCoord>& InPath,
        UGridRuntimeStateComponent* RuntimeState
    );

    /** Returns the movement cost to enter a single tile (reads from runtime state) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetTileMovementCost(UGridRuntimeStateComponent* RuntimeState, const FGridCoord& Coord);

    /** Returns the total movement cost of a path (skips the first tile / source) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetPathMovementCost(UGridRuntimeStateComponent* RuntimeState, const TArray<FGridCoord>& InPath);

    /** Horizontal spacing multiplier between hex tiles (slight overlap adjustment) */
    static constexpr float HEX_HORIZONTAL_SPACING = 0.501f;

    /** Vertical spacing factor for hex grid layout (based on hex geometry ratio) */
    static constexpr float HEX_VERTICAL_SPACING = 0.866025f * 2.3f;
    
};
