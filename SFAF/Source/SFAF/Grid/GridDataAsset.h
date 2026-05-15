// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GridTacticalTypes.h"
#include "GridDataAsset.generated.h"

class UStaticMesh;
class UMaterialInstance;

/*
Expected Gameplay Tags:

Grid.Visual.Black
Grid.Visual.White
Grid.Visual.Highlight

Grid.Size.Standard

Grid.Type.Walkable
Grid.Type.Obstacle
*/


// ---------------------------------------------------------------------------
// Structs
// ---------------------------------------------------------------------------

/**
 * Visual definition for a grid tile
 * (materials + meshes only)
 */
USTRUCT(BlueprintType)
struct FGridVisualData
{
	GENERATED_BODY()

	/** Mesh for the solid part of the tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Solid;

	/** Material for solid mesh with border/moldure */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> SolidWithMoldureMaterialInstance;

	/** Plane mesh for the tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Plane;

	/** Primary material for the plane */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneMaterialInstance;

	/** Material for the plane border */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneBorderMaterialInstance;

	/** Universal material instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> UniversalMaterialInstance;
	
	/** Maps gameplay tags to tactical rendering channels */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TMap<FGameplayTag, EGridTacticalChannel> TacticalMapping;
};

/**
 * Size definition for a grid tile
 * (physical dimensions only)
 */
USTRUCT(BlueprintType)
struct FGridSizeData
{
	GENERATED_BODY()

	/** Dimensions of the tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Size")
	FVector MeshSize = FVector(100.0f, 100.0f, 10.0f);
};

/**
 * Optional gameplay definition for tile behavior
 */
USTRUCT(BlueprintType)
struct FGridTypeData
{
	GENERATED_BODY()

	/** Whether units can be spawned on this tile type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	bool bCanSpawnUnits = true;
};

/**
 * Movement cost definition
 */
USTRUCT(BlueprintType)
struct FGridCostData
{
	GENERATED_BODY()

	/** Cost value for entering/passing through this tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	int Cost = 1;
};

// ---------------------------------------------------------------------------
// Data Asset
// ---------------------------------------------------------------------------

/**
 * Centralized grid definitions using Gameplay Tags
 */
UCLASS(BlueprintType)
class SFAF_API UGridDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// -----------------------------------------------------------------------
	// Definitions
	// -----------------------------------------------------------------------

	/** Maps tags to visual data (e.g., Grid.Visual.Black) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TMap<FGameplayTag, FGridVisualData> GridVisualDefinitions;

	/** Maps tags to size data (e.g., Grid.Size.Standard) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Size")
	TMap<FGameplayTag, FGridSizeData> GridSizeDefinitions;

	/** Maps tags to type data (e.g., Grid.Type.Walkable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	TMap<FGameplayTag, FGridTypeData> GridTypeDefinitions;

	/** Maps tags to cost data (e.g., Grid.Cost.Standard) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	TMap<FGameplayTag, FGridCostData> GridCostDefinitions;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns visual data for a given tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridVisualData(FGameplayTag Tag, FGridVisualData& OutData) const;
	
	/** Returns size data for a given tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridSizeData(FGameplayTag Tag, FGridSizeData& OutData) const;

	/** Returns type data for a given tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridTypeData(FGameplayTag Tag, FGridTypeData& OutData) const;

	/** Returns cost data for a given tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridCostData(FGameplayTag Tag, FGridCostData& OutData) const;

};
