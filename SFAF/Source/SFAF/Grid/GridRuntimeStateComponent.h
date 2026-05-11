// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridRuntimeStateComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FGridTileRuntimeState
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHovered = false;

	UPROPERTY()
	bool bSelected = false;

	UPROPERTY()
	bool bInPath = false;

	UPROPERTY()
	bool bIsNeighbor = false;

	UPROPERTY()
	bool bIsDiscovered = false;

	UPROPERTY()
	bool bIsAnalyzed = false;
};

USTRUCT(BlueprintType)
struct FGridTileOccupancy
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> OccupyingUnit = nullptr;

	TWeakObjectPtr<UAbilitySystemComponent> UnitASC;
	
	UPROPERTY()
	bool bIsBlocked = false;
};

USTRUCT(BlueprintType)
struct FGridCellRuntimeData
{
	GENERATED_BODY()

	UPROPERTY()
	FGridTileRuntimeState State;

	UPROPERTY()
	FGridTileOccupancy Occupancy;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UGridRuntimeStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridRuntimeStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
