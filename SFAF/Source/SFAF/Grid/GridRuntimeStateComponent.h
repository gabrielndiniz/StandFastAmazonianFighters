// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GridRuntimeStateComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FGridTileRuntimeState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bHovered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bInPath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bIsNeighbor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bIsDiscovered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bIsAnalyzed = false;
};

USTRUCT(BlueprintType)
struct FGridTileOccupancy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TWeakObjectPtr<AActor> OccupyingUnit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TWeakObjectPtr<UAbilitySystemComponent> UnitASC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	bool bIsBlocked = false;
};

USTRUCT(BlueprintType)
struct FGridCellRuntimeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FGridTileRuntimeState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FGridTileOccupancy Occupancy;
};

USTRUCT(BlueprintType)
struct FGridTileRuntimeData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    FIntPoint Coord;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    FVector WorldLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
    int32 InstanceIndex = INDEX_NONE;

    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    FGameplayTagContainer TileTags;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
