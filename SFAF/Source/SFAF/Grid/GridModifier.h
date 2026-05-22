// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

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

// ---------------------------------------------------------------------------
// Actor
// ---------------------------------------------------------------------------

UCLASS()
class SFAF_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------
	
	/** Change Preview Visibility */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetPreviewVisible(bool bVisible);
	
	/** Apply the modifier effect to tile data */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void ApplyToTile(FGameplayTag& InOutTileType, int32& InOutCost, FGameplayTagContainer& InOutTags) const;
	
	/** Returns the Tile Type Tag associated with this modifier */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTag GetTileModificationTag() const;
	
	/** Returns additional tags to be applied to the tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGameplayTagContainer GetTileTags() const;

	/** Checks if a world position is within the modifier's volume */
	bool AffectsPosition(const FVector& WorldPos) const;

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------
	
	/** Collision volume defining the area of effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Volume;

	/** Visual preview of the modifier in the editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridSnapComponent> GridSnapComponent;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// -----------------------------------------------------------------------
	// Configuration
	// -----------------------------------------------------------------------

	/** The modification type tag to apply */
	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTag TileModificationTag;

	/** Movement cost value-added or overridden */
	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 CostValue = 0;

	/** Priority for resolving multiple overlapping modifiers */
	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 Priority = 0;
	
	/** All gameplay tags to add to affected tiles */
	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTagContainer TileTags;	
	
	/** How the cost should be applied (Add, Override, etc.) */
	UPROPERTY(EditAnywhere, Category = "Grid")
	EGridModifierMode CostMode = EGridModifierMode::Override;

	/** The origin actor for grid snapping operations. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	TObjectPtr<AActor> GridOriginActor;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
