// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "GridMathLibrary.h"
#include "GridRuntimeStateComponent.h"
#include "Components/ActorComponent.h"
#include "GridPathfindingComponent.generated.h"

	

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
			const bool bIsFlying,
			TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);
	
	/** Returns Path coordinates reachable from Source to Target using internally cached reachable coords */
	bool GetPathCoords(
		const FGridCoord Source, 
		const FGridCoord Target, 
		TArray<FGridCoord>& PathCoords, 
		TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);

	/** Cached pathfinding data map for quick neighbor/cost lookups */
	UPROPERTY()
	TMap<FGridCoord, FTilePathfindData> TilePathfindMap;
	
	/** Map of path nodes used during A* calculations */
	TMap<FGridCoord, FPathNode> TilesPaths;
	
	/** The flying property from the last pathfinding*/
	bool bLastIsFlying = false;
	
	/** The movement points property from the last pathfinding*/
	int32 LastPoints = -1;
	
	/** The movement points property from the last pathfinding*/
	FGridCoord LastCoord = FGridCoord(-1, -1);

	/** Cached reachable coords from the last GetAllReachableCoords call */
	UPROPERTY()
	TArray<FGridCoord> CachedReachableCoords;
};
