// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid/GridType.h"
#include "BaseActionComponent.generated.h"

/**
 * Base class for all tactical action components.
 * Provides shared state (source/target coords, tile data, grid reference)
 * and the common execution interface used by specialized action components.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UBaseActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * Constructs the BaseActionComponent.
	 */
	UBaseActionComponent();

protected:
	/**
	 * Initializes the action component state on game start.
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * Per-frame tick for action component updates.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns whether the action is ready to be executed */
	UFUNCTION(BlueprintCallable, Category="Action")
	bool GetReady() const {return bReady; }
	
	/** Sets the ready state of the action */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetReady(bool bIsReady) { bReady = bIsReady;};
	
	/** Assigns the grid actor this action operates on */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetGrid(AGridType* GridType) { Grid = GridType;};
	
	/**
	 * Executes the action with the given source and target coordinates.
	 * Implemented as a BlueprintNativeEvent for override support in both C++ and Blueprints.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool Execute(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);

	/**
	 * Implementation of the Execute BlueprintNativeEvent.
	 * @param InSourceCoord The source grid coordinate.
	 * @param bHasHit Whether a valid hit was detected.
	 * @param InTargetCoord The target grid coordinate.
	 * @return True if execution succeeded.
	 */
	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);
	
	/** Returns tile static data for either the source or target tile (based on bIsTarget) */
	FGridTileStaticData GetTileData(bool bIsTarget);

	/** Returns both source and target tile static data at once */
	UFUNCTION(BlueprintPure, Category = "Action")
	void GetTileData(FGridTileStaticData& OutSourceData, FGridTileStaticData& OutTargetData) const;
	
	/**
	 * Populates LocationsForMeshes with world-space positions for visual mesh instances.
	 * Implemented as BlueprintNativeEvent for per-action customization.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool SetLocationsForMeshes();

	/**
	 * Implementation of the SetLocationsForMeshes BlueprintNativeEvent.
	 * @return True if locations were successfully computed.
	 */
	virtual bool SetLocationsForMeshes_Implementation();
	
	/** Returns the world-space locations for instanced mesh placement */
	UFUNCTION(BlueprintCallable, Category = "Action")
	TArray<FVector> GetLocationsForMeshes();
		
	/** Returns the source (bIsTarget=false) or target (bIsTarget=true) grid coordinate */
	UFUNCTION(BlueprintCallable, Category = "Action")
	const FGridCoord& GetCoord(bool bIsTarget) const { return bIsTarget ? TargetCoord : SourceCoord; }

protected:
	// -----------------------------------------------------------------------
	// State
	// -----------------------------------------------------------------------

	/** Whether this action is ready for execution */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bReady = false;		
		
	/** Source grid coordinate for the action */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord SourceCoord;
	
	/** Target grid coordinate for the action */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord TargetCoord;
	
	/** Whether the action's hit condition was satisfied during execution */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bActionHasHit = false;
	
	/** Cached static tile data for the source tile */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData SourceTileData;

	/** Cached static tile data for the target tile */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData TargetTileData;
	
	/** World-space positions for visual mesh instances */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<FVector> LocationsForMeshes;
	
	// -----------------------------------------------------------------------
	// Dependencies
	// -----------------------------------------------------------------------

	/** Weak pointer to the grid actor this action operates on */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TObjectPtr<AGridType> Grid;
};
