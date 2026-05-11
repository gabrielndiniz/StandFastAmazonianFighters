// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridDataComponent.h"

// Sets default values for this component's properties
UGridDataComponent::UGridDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UGridDataComponent::GetVisualDataByTag(FGameplayTag Tag, FGridVisualData& OutData) const
{
	if (GridDataAsset)
	{
		return GridDataAsset->GetGridVisualData(Tag, OutData);
	}
	return false;
}

bool UGridDataComponent::GetSizeDataByTag(FGameplayTag Tag, FGridSizeData& OutData) const
{
	if (GridDataAsset)
	{
		return GridDataAsset->GetGridSizeData(Tag, OutData);
	}
	return false;
}

bool UGridDataComponent::GetTypeDataByTag(FGameplayTag Tag, FGridTypeData& OutData) const
{
	if (GridDataAsset)
	{
		return GridDataAsset->GetGridTypeData(Tag, OutData);
	}
	return false;
}

bool UGridDataComponent::GetCostDataByTag(FGameplayTag Tag, FGridCostData& OutData) const
{
	if (GridDataAsset)
	{
		return GridDataAsset->GetGridCostData(Tag, OutData);
	}
	return false;
}

FIntPoint UGridDataComponent::GetNumberOfTileCount() const
{
	return TileCount;
}

void UGridDataComponent::SetNumberOfTileCount(FIntPoint NewTileCount)
{
	TileCount = NewTileCount;
}

bool UGridDataComponent::GetSpawnAroundGivenLocation() const
{
	return bSpawnAroundGivenLocation;
}

void UGridDataComponent::SetSpawnAroundGivenLocation(bool bNewSpawnAroundGivenLocation)
{
	bSpawnAroundGivenLocation = bNewSpawnAroundGivenLocation;
}

bool UGridDataComponent::GetSpawnOnEnvironment() const
{
	return bSpawnOnEnvironment;
}

void UGridDataComponent::SetSpawnOnEnvironment(bool bNewSpawnOnEnvironment)
{
	bSpawnOnEnvironment = bNewSpawnOnEnvironment;
}

FVector UGridDataComponent::GetTileSize() const
{	
	return TileSize;
}

void UGridDataComponent::SetTileSize(FVector NewTileSize)
{
	TileSize = NewTileSize;
}

FVector UGridDataComponent::GetInitialSpawnLocation() const
{
	return InitialSpawnLocation;
}

void UGridDataComponent::SetInitialSpawnLocation(FVector NewSpawnLocation)
{
	InitialSpawnLocation = NewSpawnLocation;
}

FGameplayTag UGridDataComponent::GetGridVisualTag() const
{
	return GridVisualTag;
}

void UGridDataComponent::SetGridVisualTag(FString NewVisualTagString)
{
	GridVisualTag = FGameplayTag::RequestGameplayTag(FName(*NewVisualTagString));
}


// Called when the game starts
void UGridDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGridDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

