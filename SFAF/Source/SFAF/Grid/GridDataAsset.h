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


/**
 * Visual definition for a grid tile
 * (materials + meshes only)
 */
USTRUCT(BlueprintType)
struct FGridVisualData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Solid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> SolidWithMoldureMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UStaticMesh> Plane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> PlaneBorderMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TObjectPtr<UMaterialInstance> UniversalMaterialInstance;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	bool bCanSpawnUnits = true;
};


/**
 * MovementCost
 */
USTRUCT(BlueprintType)
struct FGridCostData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	int Cost = 1;
};


/**
 * Centralized grid definitions using Gameplay Tags
 */
UCLASS(BlueprintType)
class SFAF_API UGridDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/*
	Example:
	Grid.Visual.Black
	Grid.Visual.Green
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Visual")
	TMap<FGameplayTag, FGridVisualData> GridVisualDefinitions;


	/*
	Example:
	Grid.Size.Standard
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Size")
	TMap<FGameplayTag, FGridSizeData> GridSizeDefinitions;


	/*
	Example:
	Grid.Type.Walkable
	Grid.Type.Blocked
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Type")
	TMap<FGameplayTag, FGridTypeData> GridTypeDefinitions;


	/*
	Example:
	Grid.Cost.Standard
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid|Cost")
	TMap<FGameplayTag, FGridCostData> GridCostDefinitions;


	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridVisualData(FGameplayTag Tag, FGridVisualData& OutData) const
	{
		if (const FGridVisualData* Data = GridVisualDefinitions.Find(Tag))
		{
			OutData = *Data;
			return true;
		}
		return false;
	}


	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridSizeData(FGameplayTag Tag, FGridSizeData& OutData) const
	{
		if (const FGridSizeData* Data = GridSizeDefinitions.Find(Tag))
		{
			OutData = *Data;
			return true;
		}
		return false;
	}


	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridTypeData(FGameplayTag Tag, FGridTypeData& OutData) const
	{
		if (const FGridTypeData* Data = GridTypeDefinitions.Find(Tag))
		{
			OutData = *Data;
			return true;
		}
		return false;
	}


	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridCostData(FGameplayTag Tag, FGridCostData& OutData) const
	{
		if (const FGridCostData* Data = GridCostDefinitions.Find(Tag))
		{
			OutData = *Data;
			return true;
		}
		return false;
	}
};