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
	float TraceRange = 1000.f;

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
	
	//TODO: Mapped State to tile pathfind

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
	void AddInstanceMesh(const FGameplayTag& TileTypeTag, const FGameplayTagContainer& TileTags, const FTransform& Transform) const;

	/** Remove an Instanced Mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	bool RemoveInstanceMesh(int TileType, int Index) const;

	/** Returns the Hits Results for tracing ground */
	UFUNCTION(BlueprintCallable, Category = "Grid|Trace")
	FHitResult HitTraceGround(FVector Location, TArray<AActor*> ActorsToIgnore) const;

	/** Returns the Trace Sphere considered range */
	UFUNCTION(BlueprintPure, Category = "Grid|Trace")
	float GetTraceSphereRange() const { return TraceSphereRadius; }

	/** Generate the entire grid before player start playing */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void GenerateGrid();

	/** Set visibility for the Tactical Mesh */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void ShowTacticalGrid(bool bShow);
	
	/** Get the first tile from the Grid */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FIntPoint GetFirstTile () const;
    
	/** Get the last tile from the Grid */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FIntPoint GetLastTile () const;
	
protected:
	// -----------------------------------------------------------------------
	// Tile Data
	// -----------------------------------------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	FIntPoint FirstTile = FIntPoint(0, 0);
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	FIntPoint LastTile = FIntPoint(0, 0);
	
private:
	// -----------------------------------------------------------------------
	// Internal Cache
	// -----------------------------------------------------------------------

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UInstancedStaticMeshComponent>> TacticalModifiersMeshes;
};
