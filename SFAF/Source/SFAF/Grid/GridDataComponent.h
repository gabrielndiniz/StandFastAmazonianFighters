// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridCoord.h"
#include "Components/ActorComponent.h"
#include "GridDataAsset.h"
#include "GridDataComponent.generated.h"

/**
 * Data configuration component for the grid system.
 * Stores references to the GridDataAsset and configurable parameters
 * such as tile count, tile size, spawn behavior, and visual tag.
 * Acts as the bridge between the grid actor and its data asset definitions.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Default constructor for the grid data component */
	UGridDataComponent();

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns the assigned grid data asset */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	UGridDataAsset* GetGridDataAsset() const { return GridDataAsset; }
	
	/** Returns the visual data for the currently selected grid visual tag */
	FGridVisualData GetVisualData() const;
	
	/** Returns visual data from the data asset for a specific tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetVisualDataByTag(FGameplayTag Tag, FGridVisualData& OutData) const;

	/** Returns size data from the data asset for a specific tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSizeDataByTag(FGameplayTag Tag, FGridSizeData& OutData) const;
	
	/** Returns type data from the data asset for a specific tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetTypeDataByTag(FGameplayTag Tag, FGridTypeData& OutData) const;

	/** Returns cost data from the data asset for a specific tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCostDataByTag(FGameplayTag Tag, FGridCostData& OutData) const;

	/** Returns the number of tiles on each axis (X, Y) */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGridCoord GetNumberOfTileCount() const;

	/** Sets the number of tiles on each axis */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetNumberOfTileCount(FGridCoord NewTileCount);

	/** Returns whether the grid spawns centered around the actor location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSpawnAroundGivenLocation() const;

	/** Sets whether the grid spawns centered around the actor location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetSpawnAroundGivenLocation(bool bNewSpawnAroundGivenLocation);

	/** Returns whether tiles should snap to environment geometry via traces */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSpawnOnEnvironment() const;

	/** Sets whether tiles should snap to environment geometry */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetSpawnOnEnvironment(bool bNewSpawnOnEnvironment);

	/** Returns the physical dimensions of a single tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetTileSize() const;

	/** Sets the physical dimensions of a single tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetTileSize(FVector NewTileSize);

	/** Returns the initial world-space location where grid generation starts */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetInitialSpawnLocation() const;

	/** Sets the initial world-space location where grid generation starts */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetInitialSpawnLocation(FVector NewSpawnLocation);
		
	/** Returns the current gameplay tag identifying the grid visual set */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTag GetGridVisualTag() const;

	/** Sets the visual tag for the grid from a string (e.g., "Grid.Visual.Green") */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetGridVisualTag(FString NewVisualTagString);
	
protected:
	/** Initializes the component when the game starts */
	virtual void BeginPlay() override;

	// -----------------------------------------------------------------------
	// Configuration
	// -----------------------------------------------------------------------

	/** Data asset containing all grid visual, size, type, and cost definitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TObjectPtr<UGridDataAsset> GridDataAsset;
	
	/** Number of tiles along the X and Y axes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FGridCoord TileCount;
	
	/** If true, the grid expands outward from the pivot in all directions; if false, only in positive axes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bSpawnAroundGivenLocation;
	
	/** If true, ground traces determine each tile's Z height from the environment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bSpawnOnEnvironment;
	
	/** Physical scale/dimensions of each tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize = FVector(400,350,50);
	
	/** World-space starting point for grid generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector InitialSpawnLocation;
		
	/** Default gameplay tag identifying the grid's visual style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FGameplayTag GridVisualTag = FGameplayTag::RequestGameplayTag("Grid.Visual.Green");

public:	
	/** Updates the component each frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
