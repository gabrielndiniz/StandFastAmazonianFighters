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

/** Visual definition for a grid tile, containing meshes and material instances */
USTRUCT(BlueprintType)
struct FGridVisualData
{
	GENERATED_BODY()

	/** Mesh for the solid 3D portion of the tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Solid;

	/** Material instance applied to the solid mesh when a border/moldure is desired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> SolidWithMoldureMaterialInstance;

	/** Flat plane mesh for the tile surface */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Plane;

	/** Primary material instance for the plane mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneMaterialInstance;

	/** Material instance for the plane's border edges */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneBorderMaterialInstance;

	/** General-purpose material instance used when no specialized material is needed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> UniversalMaterialInstance;

	/** Maps gameplay tags to their corresponding tactical rendering channels (e.g., obstacle, double cost) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TMap<FGameplayTag, EGridTacticalChannel> TacticalMapping;
};

/** Physical size definition for a grid tile (dimensions only) */
USTRUCT(BlueprintType)
struct FGridSizeData
{
	GENERATED_BODY()

	/** The 3D scale/dimensions of the tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Size")
	FVector MeshSize = FVector(100.0f, 100.0f, 10.0f);
};

/** Gameplay behavior definition for a tile type */
USTRUCT(BlueprintType)
struct FGridTypeData
{
	GENERATED_BODY()

	/** Whether units can be spawned on this tile type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	bool bCanSpawnUnits = true;
};

/** Movement cost definition for a tile */
USTRUCT(BlueprintType)
struct FGridCostData
{
	GENERATED_BODY()

	/** Movement points required to enter or pass through this tile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	int Cost = 1;
};

// ---------------------------------------------------------------------------
// Data Asset
// ---------------------------------------------------------------------------

/**
 * Centralized data asset that defines grid visual, size, type, and cost properties
 * keyed by gameplay tags. Grid generation reads from this asset to configure tiles.
 */
UCLASS(BlueprintType)
class SFAF_API UGridDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// -----------------------------------------------------------------------
	// Definitions
	// -----------------------------------------------------------------------

	/** Maps visual tags (e.g., Grid.Visual.Black) to their visual data (meshes, materials) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TMap<FGameplayTag, FGridVisualData> GridVisualDefinitions;

	/** Maps size tags (e.g., Grid.Size.Standard) to their physical dimension data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Size")
	TMap<FGameplayTag, FGridSizeData> GridSizeDefinitions;

	/** Maps type tags (e.g., Grid.Type.Walkable) to their behavior data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	TMap<FGameplayTag, FGridTypeData> GridTypeDefinitions;

	/** Maps cost tags (e.g., Grid.Cost.Standard) to their movement cost data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	TMap<FGameplayTag, FGridCostData> GridCostDefinitions;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Looks up visual data by gameplay tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridVisualData(FGameplayTag Tag, FGridVisualData& OutData) const;

	/** Looks up size data by gameplay tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridSizeData(FGameplayTag Tag, FGridSizeData& OutData) const;

	/** Looks up type data by gameplay tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridTypeData(FGameplayTag Tag, FGridTypeData& OutData) const;

	/** Looks up cost data by gameplay tag */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridCostData(FGameplayTag Tag, FGridCostData& OutData) const;
};
