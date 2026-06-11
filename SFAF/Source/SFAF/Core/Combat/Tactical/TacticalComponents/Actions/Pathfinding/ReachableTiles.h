// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "ReachableTiles.generated.h"

/**
 * Action component that computes all tiles reachable from a source tile
 * within a given movement point budget, optionally considering flying movement.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UReachableTiles : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the ReachableTiles component.
	 */
	UReachableTiles();

protected:
	/**
	 * Initializes the reachable tiles component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Recalculates reachable tiles each frame when needed.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Updates the mesh instances with reachable tile locations.
	 * @return True if mesh locations were set successfully.
	 */
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	/** Returns the list of reachable tile coordinates */
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetReachableTiles();
	
	/** Sets the movement points available for reachability calculations */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetCurrentMovementPoints(int32 Points);
	
	/** Returns the current movement point budget */
	UFUNCTION(BlueprintCallable, Category="Action")
	int32 GetCurrentMovementPoints();
		
	/** Enables or disables flying movement consideration */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetConsiderFly(bool bConsider);
	
	/** Cached list of reachable tile coordinates */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> ReachableTiles;
	
	/** Current movement point budget for reachability */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	int32 CurrentMovementPoints;	
	
	/** Whether flying movement is considered (ignores ground-based restrictions) */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bConsiderFly = false;	
};
