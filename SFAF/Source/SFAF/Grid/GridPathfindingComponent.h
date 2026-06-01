// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "GridRuntimeStateComponent.h"
#include "Components/ActorComponent.h"
#include "GridPathfindingComponent.generated.h"

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
	

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridPathfindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridPathfindingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;	
	
	/** Populates neighbor coords for a given tile, respecting tile costs and flying state */
	bool GetNeighborsCoords(FGridCoord Coord, TArray<FGridCoord>& NeighborsCoords,
	                        TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent, bool bIsFlying);
	
	/** Clears all cached pathfinding data */
	UFUNCTION()
	void ClearPathfindingCache();	
	
	/** Returns all coordinates reachable from Coord within the given movement point budget */
	bool GetAllReachableCoords(
		FGridCoord Coord, 
		TArray<FGridCoord>& ReachableCoords, 
		const int32 Points, 
		TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);

	/** Cached pathfinding data map for quick neighbor/cost lookups */
	UPROPERTY()
	TMap<FGridCoord, FTilePathfindData> TilePathfindMap;
	
	/** Map of path nodes used during A* calculations */
	TMap<FGridCoord, FPathNode> TilesPaths;
};
