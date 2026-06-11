// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GridModifierTypes.h"
#include "GameFramework/Actor.h"
#include "GridTacticalTypes.h"
#include "GridSnapComponent.h"
#include "GridModifier.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

/**
 * An actor that modifies grid tiles within its volume.
 * Can override or add to tile cost, apply gameplay tags, and change tile type.
 * Snaps to the nearest grid position automatically.
 */
UCLASS()
class SFAF_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Default constructor for the grid modifier actor */
	AGridModifier();

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------
	
	/** Toggles the visibility of the editor preview mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetPreviewVisible(bool bVisible);
	
	/**
	 * Applies this modifier's effect to tile data.
	 * Modifies the tile type, cost, and gameplay tags in place.
	 */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void ApplyToTile(FGameplayTag& InOutTileType, int32& InOutCost, FGameplayTagContainer& InOutTags) const;
	
	/** Returns the gameplay tag identifying this modifier's tile type */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTag GetTileModificationTag() const;
	
	/** Returns all gameplay tags to be applied to affected tiles */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTagContainer GetTileTags() const;

	/**
	 * Checks whether a world position falls within this modifier's collision volume.
	 * @param WorldPos The world position to test.
	 * @return True if the position is inside the volume.
	 */
	bool AffectsPosition(const FVector& WorldPos) const;
	
	/** Returns the snapped grid coordinate of this modifier */
	UFUNCTION(BlueprintPure, Category = "Grid")
	FGridCoord GetSnappedCoord() const;
	
	/** Returns the snapped world location of this modifier */
	UFUNCTION(BlueprintPure, Category = "Grid")
	FVector GetSnappedWorldLocation() const;

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------
	
	/** Collision volume defining the spatial area this modifier affects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TObjectPtr<UBoxComponent> Volume;

	/** Static mesh used for visual preview of the modifier in the editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	/** Component responsible for snapping this actor to the nearest grid tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TObjectPtr<UGridSnapComponent> GridSnapComponent;

protected:
	/** Snaps the modifier to the grid when constructed or edited in the editor */
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// -----------------------------------------------------------------------
	// Configuration
	// -----------------------------------------------------------------------

	/** The tile modification type tag (e.g., Grid.Type.Obstacle, Grid.Cost.Double) */
	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTag TileModificationTag;

	/** Movement cost value to apply (adds to or overrides the tile's cost depending on CostMode) */
	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 CostValue = 0;

	/** Priority for resolving conflicts when multiple modifiers overlap the same tile */
	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 Priority = 0;
	
	/** Additional gameplay tags to apply to affected tiles */
	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTagContainer TileTags;	
	
	/** How the cost value is applied: Override (replace) or Additive */
	UPROPERTY(EditAnywhere, Category = "Grid")
	EGridModifierMode CostMode = EGridModifierMode::Override;

	/** The grid actor that provides the snapping origin reference */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	TObjectPtr<AActor> GridOriginActor;

protected:
	/** Initializes the modifier when the game starts */
	virtual void BeginPlay() override;

public:	
	/** Updates the modifier each frame */
	virtual void Tick(float DeltaTime) override;
};
