// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "GridMathLibrary.h"
#include "GridRuntimeStateComponent.h"
#include "Components/ActorComponent.h"
#include "GridPathfindingComponent.generated.h"

/**
 * Component responsible for pathfinding on the hex grid.
 * Provides neighbor discovery, reachable-tile BFS expansion, and A* pathfinding.
 * Caches pathfinding data to avoid redundant calculations.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridPathfindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor for the pathfinding component */
	UGridPathfindingComponent();

protected:
	/** Initializes the component when the game starts */
	virtual void BeginPlay() override;

public:
	/** Updates the component each frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;	
	
	/**
	 * Populates the neighbor coordinates for a given tile, respecting tile costs and flying state.
	 * Results are cached in TilePathfindMap for fast subsequent lookups.
	 */
	bool GetNeighborsCoords(FGridCoord Coord, TArray<FGridCoord>& NeighborsCoords,
	                        TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent, bool bIsFlying);
	
	/** Clears all cached pathfinding data (TilePathfindMap) */
	UFUNCTION()
	void ClearPathfindingCache();	
	
	/**
	 * Computes all tiles reachable from the given coordinate within the specified movement point budget.
	 * Uses BFS expansion and caches results in CachedReachableCoords.
	 */
	bool GetAllReachableCoords(
			FGridCoord Coord,
			TArray<FGridCoord>& ReachableCoords,
			const int32 Points,
			const bool bIsFlying,
			TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);
	
	/**
	 * Computes the A* path from Source to Target.
	 * Requires that GetAllReachableCoords has been called first, as it operates on cached reachable data.
	 */
	bool GetPathCoords(
		const FGridCoord Source, 
		const FGridCoord Target, 
		TArray<FGridCoord>& PathCoords, 
		TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);

	/** Retrieves the pathfinding node data for a given grid coordinate from TilesPaths */
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool GetPathNode(const FGridCoord& Coord, FPathNode& OutPathNode) const;

	/** Retrieves the path search node data for a given grid coordinate from the A* cache */
	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
	bool GetPathSearchNode(const FGridCoord& Coord, FPathSearchNode& OutNode) const;

	/** Cached map of tile pathfinding data for fast neighbor/cost lookups */
	UPROPERTY()
	TMap<FGridCoord, FTilePathfindData> TilePathfindMap;
	
	/** Map of path nodes used during reachability and A* calculations */
	TMap<FGridCoord, FPathNode> TilesPaths;

	/** Cached A* search nodes populated during GetPathCoords for blueprint access */
	TMap<FGridCoord, FPathSearchNode> PathSearchNodes;
	
	/** Cached flying state from the last pathfinding query */
	bool bLastIsFlying = false;
	
	/** Cached movement points from the last pathfinding query */
	int32 LastPoints = -1;
	
	/** Cached origin coordinate from the last pathfinding query */
	FGridCoord LastCoord = FGridCoord(-1, -1);

	/** Cached reachable coordinates from the most recent GetAllReachableCoords call */
	UPROPERTY()
	TArray<FGridCoord> CachedReachableCoords;
};
