// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridDataComponent.h"
#include "GridRuntimeStateComponent.h"
#include "GameFramework/Actor.h"
#include "GridTacticalTypes.h"
#include "GridType.generated.h"

// ---------------------------------------------------------------------------
// Actor
// ---------------------------------------------------------------------------

UCLASS()
class SFAF_API AGridType : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridType();

	/** Initialize grid collisions */
	void InitializeCollision() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridDataComponent> GridDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridRuntimeStateComponent> GridRuntimeStateComponent;
	
private:
	// -----------------------------------------------------------------------
	// Internal Configuration
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float GridVerticalDistance = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float TraceRange = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float TraceSphereRadius = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> GridMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalObstacleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalDoubleCostMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalTripleCostMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalFlyingOnlyMesh;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer StandardTileTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer EnvironmentTileTags;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTag TacticalVisualTag;
	
	/** Horizontal spacing multiplier between hex tiles (slight overlap adjustment) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float XOffset = 0.501f;

	/** Vertical spacing factor for hex grid layout (based on hex geometry ratio) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float YOffset = 0.866025f * 2.3f;
	
	/** If needed, it is possible on this code to ignore specific actors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();

public:
	// -----------------------------------------------------------------------
	// Grid API
	// -----------------------------------------------------------------------

	/** Clear all Instanced Meshes */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void ClearInstancedMeshes() const;
	
	/** Returns tactical mesh according to tag */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	UInstancedStaticMeshComponent* SelectTacticMeshWithTag(FGameplayTag GridModifierTag) const;

	/** Add an Instanced Mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void AddInstanceMesh(const FGameplayTagContainer& TileTags, const FTransform& Transform);

	/** Remove an Instanced Mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	bool RemoveInstanceMesh(int TileType, int Index) const;

	/** Returns the Hits Results for tracing ground */
	UFUNCTION(BlueprintCallable, Category = "Grid|Trace")
	FHitResult HitTraceGround(FVector Location) const;

	/** Returns the Trace Sphere considered range */
	UFUNCTION(BlueprintPure, Category = "Grid|Trace")
	float GetTraceSphereRange() const { return TraceSphereRadius; }
	
	/** Used when spawning grid. Trace ground on Z axis to see where to locate a tile */
	UFUNCTION()
	bool TraceGround(FVector& Location, FGameplayTagContainer& TileTags, bool& bGridModifier,
	                 FGameplayTag& ModifierTag, float& ZScale) const;

	/** Add the Tile on the Grid*/
	bool AddGridTileInstance(int32 TileIndex, const FTransform& TileTransform, FIntPoint TilePosition, bool bCheckForEquivalents,
	                         const FGameplayTagContainer& TileTags, ACombatant_Base* UnitOnTile);
	static bool CanAddTile(const FGameplayTagContainer& TileTags);

	/** Generate the entire grid before player start playing */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GenerateGrid(const FVector Location);

	/** Set visibility for the Tactical Mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void ShowTacticalGrid(bool bShow);
	
	/** Destroy all generated grid tiles and clear runtime data */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DestroyGridTiles();
	
	/** Get the first tile from the Grid */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FIntPoint GetFirstTile () const;
    
	/** Get the last tile from the Grid */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FIntPoint GetLastTile () const;
	
	/** Returns true if the Grid is ready for combat*/
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	bool GetIsReady() const;
	
protected:
	// -----------------------------------------------------------------------
	// Tile Data
	// -----------------------------------------------------------------------

	
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	FIntPoint FirstTile = FIntPoint(TNumericLimits<int32>::Max(), 0);
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	FIntPoint LastTile = FIntPoint(0, 0);
			
		
	// -----------------------------------------------------------------------
	// Internal Cache
	// -----------------------------------------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	bool bReady;
	
private:
			//Every Tactical variable and functions is used for debug
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UInstancedStaticMeshComponent>> TacticalModifiersMeshes;
	
	UPROPERTY()
	TMap<FIntPoint, FGameplayTag> TacticalModifiersPositions;
};
