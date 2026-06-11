// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "Components/ActorComponent.h"
#include "GridSnapComponent.generated.h"

class AGridType;

/**
 * Component that snaps world-space locations to the nearest valid hex grid position.
 * Used by GridModifier and InstancedProps to align with the tile grid.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UGridSnapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor for the grid snap component */
	UGridSnapComponent();

	/** Initializes the component when the game starts */
	virtual void BeginPlay() override;

	/** Assigns the target grid actor used as the snapping origin */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	void SetGrid(AGridType* Grid);

	/** Returns the currently assigned grid actor */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	AGridType* GetGrid() const;

	/**
	 * Converts a world-space location to the nearest grid tile center.
	 * Updates the cached Coord member with the computed grid coordinate.
	 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	FVector GetSnappedLocation(FVector WorldLocation) const;

	/** Returns the last computed grid coordinate from GetSnappedLocation */
	UFUNCTION(CallInEditor, BlueprintPure, Category = "Grid")
	FGridCoord GetSnapCoord() const;

	/** Physical dimensions of a single tile used for snap calculations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize = FVector(400.f, 350.f, 50.f);

	/** Grid actor reference used as the snapping origin */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	AGridType* GridOriginActor;

	/** Vertical offset applied on top of the snapped location Z */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	float ZAlteration = 0.f;

	/** Cached grid coordinate from the most recent snap operation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	mutable FGridCoord Coord;
};
