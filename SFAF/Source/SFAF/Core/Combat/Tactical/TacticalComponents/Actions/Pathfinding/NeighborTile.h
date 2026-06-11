// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "NeighborTile.generated.h"

/**
 * Action component that discovers and caches the valid neighbor tiles
 * adjacent to a given source tile, respecting movement restrictions and flying state.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UNeighborTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the NeighborTile component.
	 */
	UNeighborTile();

protected:
	/**
	 * Initializes the neighbor tile component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Refreshes neighbor tile data each frame.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Updates the mesh instances with neighbor tile locations.
	 * @return True if mesh locations were set successfully.
	 */
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	/** Returns the list of valid neighbor coordinates */
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetNeighbors();
	
	/** Sets whether flying movement is considered when computing neighbors */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetConsiderFly(bool bConsider);
	
	/** Cached list of valid neighbor coordinates */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> NeighborsCoords;
	
	/** Whether to ignore ground-based movement restrictions for neighbor discovery */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bConsiderFly = true;	
};
