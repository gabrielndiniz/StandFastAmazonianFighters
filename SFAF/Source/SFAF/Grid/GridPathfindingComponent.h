// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "GridRuntimeStateComponent.h"
#include "Components/ActorComponent.h"
#include "GridPathfindingComponent.generated.h"

USTRUCT()
struct FTilePathfindData
{
	GENERATED_BODY()
	
	TArray<FGridCoord> Neighbors;
	
	int32 Cost = 1;
	
	bool bFlyOnly = false;
	
};

struct FPathNode
{
	FGridCoord Coord;
	int CostSoFar = 0;
	int Heuristic = 0;
	int TotalCost = 0;
	bool IsClosed = false;
	TSharedPtr<FPathNode> Parent = nullptr;

	// Comparing for priority queue
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
	
	/** Get the neighbors*/
	bool GetNeighborsCoords(FGridCoord Coord, TArray<FGridCoord>& NeighborsCoords,
	                        TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent, bool bIsFlying);
	
	UFUNCTION()
	void ClearPathfindingCache();	
	
	/** Get the reachable coords*/
	bool GetAllReachableCoords(
		FGridCoord Coord, 
		TArray<FGridCoord>& ReachableCoords, 
		const int32 Points, 
		TObjectPtr<UGridRuntimeStateComponent> RuntimeStateComponent);

	UPROPERTY()
	TMap<FGridCoord, FTilePathfindData> TilePathfindMap;
	
	TMap<FGridCoord, FPathNode> TilesPaths;
};
