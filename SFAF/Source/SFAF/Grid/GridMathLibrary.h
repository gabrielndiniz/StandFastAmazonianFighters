// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

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

    /** Valid neighbor coordinates adjacent to this tile */
    TArray<FGridCoord> Neighbors;

    /** Movement cost required to enter this tile */
    int32 Cost = 1;

    /** When true, only flying units can occupy this tile */
    bool bFlyOnly = false;
};

/** Internal node used by the A* pathfinding algorithm for priority queue processing */
struct FPathNode
{
    /** Grid coordinate this node represents */
    FGridCoord Coord;

    /** Accumulated movement cost from the start node to this node */
    int CostSoFar = 0;

    /** Estimated remaining cost to the goal (admissible heuristic) */
    int Heuristic = 0;

    /** Sum of CostSoFar + Heuristic (used as priority in the queue) */
    int TotalCost = 0;

    /** Whether this node has already been fully evaluated (closed set) */
    bool IsClosed = false;

    /** Pointer to the predecessor node, used to reconstruct the final path */
    TSharedPtr<FPathNode> Parent = nullptr;

    /** Comparison operator: lower TotalCost means higher priority in the queue */
    bool operator<(const FPathNode& Other) const {
        return TotalCost < Other.TotalCost;
    }
};

/**
 * Static utility library providing hex grid mathematics and pathfinding operations.
 * Includes coordinate conversions, neighbor detection, A* pathfinding helpers,
 * and reachable-tile computation via BFS.
 */
UCLASS()
class SFAF_API UGridMathLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Converts hex grid offset coordinates to a world-space location.
     * Accounts for the staggered row offset typical of hex grids.
     * @param GridCoord     The tile coordinates on the grid.
     * @param GridOrigin    The world-space origin (pivot) of the grid.
     * @param TileSize      The physical dimensions of a single tile.
     * @param ZCorrection   Additional vertical offset applied to the result.
     * @return The world-space location at the center of the specified tile.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexOffsetToWorld(FGridCoord GridCoord, FVector GridOrigin, FVector TileSize, float ZCorrection);

    /**
     * Snaps an arbitrary XY world position to the nearest hex tile center on the grid plane.
     * Does not check whether a tile actually exists at that position.
     * @param Position  The world-space position to snap.
     * @param TileSize  The physical dimensions of a single tile.
     * @return The snapped world-space position on the hex grid.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexFindNearestTilePositionOnXYPlane(FVector Position, const FVector TileSize);

    /**
     * Converts a world-space position back to hex offset grid coordinates.
     * Inverse of HexOffsetToWorld.
     * @param WorldPos   The world-space position to convert.
     * @param GridOrigin The origin of the grid in world space.
     * @param TileSize   The physical dimensions of a single tile.
     * @return The grid coordinate corresponding to the given world position.
     */
    static FGridCoord HexWorldToOffsetCoord(const FVector& WorldPos, const FVector& GridOrigin, const FVector& TileSize);

    /**
     * Finds the nearest valid tile index from a list of positions by comparing distances
     * to a target coordinate, optionally weighted by tile movement costs.
     * @param Positions      Array of candidate grid coordinates.
     * @param StaticTiles    Map of registered tile static data.
     * @param Target         The target coordinate to measure distance from.
     * @param bConsiderFlying If true, ignores tile cost multipliers.
     * @return The index in Positions of the nearest valid tile, or INDEX_NONE.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 FindNearestTileIndex(
        const TArray<FGridCoord>& Positions,
        const TMap<FGridCoord, FGridTileStaticData>& StaticTiles,
        const FGridCoord& Target,
        bool bConsiderFlying
    );

    /**
     * Snaps a world-space position to the nearest hex grid tile center.
     * Pure mathematical conversion without checking tile existence.
     * @param WorldPosition  The world-space position to snap.
     * @param GridOrigin     The world origin of the grid.
     * @param TileSize       The physical dimensions of a single tile.
     * @return The snapped world-space position on the hex grid.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FVector HexSnapWorldToGrid(
        const FVector& WorldPosition,
        const FVector& GridOrigin,
        const FVector& TileSize
    );

    /**
     * Finds the nearest registered tile from a world-space position by iterating
     * all static tiles and checking squared distance.
     * @param WorldPosition  The world-space position to search from.
     * @param StaticTiles    Map of all registered tile static data.
     * @param OutCoord       Output: the closest tile's grid coordinate.
     * @param OutTileData    Output: the closest tile's static data.
     * @return True if a valid tile was found.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool FindNearestTileFromWorldPosition(
        const FVector& WorldPosition,
        const TMap<FGridCoord, FGridTileStaticData>& StaticTiles,
        FGridCoord& OutCoord,
        FGridTileStaticData& OutTileData
    );

    /** Returns true if the given integer is even */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool IsItEven(int32 Number);

    /**
     * Computes the six neighboring grid coordinates for a hex tile.
     * Accounts for the staggered row pattern of the offset coordinate system.
     * @param Coord           The source tile coordinate.
     * @param NeighborsCoords Output array populated with up to 6 neighbor coordinates.
     * @return True if at least one neighbor was found.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool GetHexNeighborTiles(
        const FGridCoord Coord,
        TArray<FGridCoord>& NeighborsCoords
    );

    /** Returns true if both coordinates share the same row (same Y) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static bool IsInTheSameLine(FGridCoord Coord1, FGridCoord Coord2);

    /** Returns the Manhattan distance between two hex grid coordinates (admissible heuristic for A*) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetHexDistance(const FGridCoord& A, const FGridCoord& B);

    /**
     * Returns a linearity penalty for a tile relative to the Source->Target line.
     * Lower values indicate the tile lies closer to the direct line, producing straighter paths.
     */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetLinearityPenalty(const FGridCoord& Source, const FGridCoord& Target, const FGridCoord& Tile);

    /**
     * BFS expansion loop for reachable-coordinate computation.
     * Processes an open list, expands neighbors via the provided function,
     * tracks visited nodes in TilesPaths, and respects the movement point budget.
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

    /**
     * Sorts an unsorted array of path tiles from Source to Target using neighbor traversal.
     * Produces an ordered sequence suitable for movement execution.
     */
    static TArray<FGridCoord> SortPathTiles(
        const FGridCoord& Source,
        const FGridCoord& Target,
        const TArray<FGridCoord>& InPath,
        UGridRuntimeStateComponent* RuntimeState
    );

    /** Returns the movement cost of entering a single tile by reading from the runtime state */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetTileMovementCost(UGridRuntimeStateComponent* RuntimeState, const FGridCoord& Coord);

    /** Returns the total movement cost for an entire path (skips the source tile) */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetPathMovementCost(UGridRuntimeStateComponent* RuntimeState, const TArray<FGridCoord>& InPath);

    /** Horizontal spacing multiplier between hex tiles */
    static constexpr float HEX_HORIZONTAL_SPACING = 0.501f;

    /** Vertical spacing factor accounting for hex geometry ratio and staggered rows */
    static constexpr float HEX_VERTICAL_SPACING = 0.866025f * 2.3f;
};
