// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "PathTiles.generated.h"

/**
 * Action component that computes and stores a movement path between two tiles.
 * Uses the grid's pathfinding to calculate an ordered path and its total cost.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UPathTiles : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the PathTiles component.
	 */
	UPathTiles();

protected:
	/**
	 * Initializes the path tiles component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Recalculates path tiles each frame when scanning is active.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Updates the mesh instances with path tile locations.
	 * @return True if mesh locations were set successfully.
	 */
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	/** The ordered coordinates of the computed path */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> PathTiles;

	/** Total movement cost required to traverse the computed path */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	int32 TotalCost;
	
	/** Returns the computed path tile coordinates */
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetPathTiles();

	/** Returns the total movement cost of the computed path */
	UFUNCTION(BlueprintCallable, Category = "Action")
	int32 GetTotalCost() const;
};
