// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridDataAsset.h"
#include "GridDataComponent.generated.h"


// ---------------------------------------------------------------------------
// Component
// ---------------------------------------------------------------------------

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridDataComponent();

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns the assigned Grid Data Asset */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	UGridDataAsset* GetGridDataAsset() const { return GridDataAsset; }
	
	/** Returns visual data from the data asset by tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetVisualDataByTag(FGameplayTag Tag, FGridVisualData& OutData) const;

	/** Returns size data from the data asset by tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSizeDataByTag(FGameplayTag Tag, FGridSizeData& OutData) const;
	
	/** Returns type data from the data asset by tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetTypeDataByTag(FGameplayTag Tag, FGridTypeData& OutData) const;

	/** Returns cost data from the data asset by tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCostDataByTag(FGameplayTag Tag, FGridCostData& OutData) const;

	/** Returns the number of tiles on each axis */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FIntPoint GetNumberOfTileCount() const;

	/** Sets the number of tiles on each axis */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetNumberOfTileCount(FIntPoint NewTileCount);

	/** Returns whether the grid spawns centered around the actor location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSpawnAroundGivenLocation() const;

	/** Sets whether the grid should spawn centered around the actor location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetSpawnAroundGivenLocation(bool bNewSpawnAroundGivenLocation);

	/** Returns whether tiles should snap to environment geometry */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetSpawnOnEnvironment() const;

	/** Sets whether tiles should snap to environment geometry */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetSpawnOnEnvironment(bool bNewSpawnOnEnvironment);

	/** Returns the physical size of a single tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetTileSize() const;

	/** Sets the physical size of a single tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetTileSize(FVector NewTileSize);

	/** Returns the initial location where grid generation starts */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GetInitialSpawnLocation() const;

	/** Sets the initial location where grid generation starts */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetInitialSpawnLocation(FVector NewSpawnLocation);
		
	/** Returns the current visual tag for the grid */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTag GetGridVisualTag() const;

	/** Sets the visual tag for the grid from a string */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetGridVisualTag(FString NewVisualTagString);
	
protected:
	virtual void BeginPlay() override;

	// -----------------------------------------------------------------------
	// Configuration
	// -----------------------------------------------------------------------

	/** Asset containing all grid definitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TObjectPtr<UGridDataAsset> GridDataAsset;
	
	/** Number of tiles (X, Y) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntPoint TileCount;
	
	/** If true, grid grows in all directions from pivot; if false, grows positively */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bSpawnAroundGivenLocation;
	
	/** If true, uses traces to find ground height for each tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bSpawnOnEnvironment;
	
	/** Scale of each tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize;
	
	/** Start point for grid generation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector InitialSpawnLocation;
		
	/** Default visual tag for the grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FGameplayTag GridVisualTag = FGameplayTag::RequestGameplayTag("Grid.Visual.Green");

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
