// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridDataComponent.h"
#include "GridPathfindingComponent.h"
#include "GridRuntimeStateComponent.h"
#include "GameFramework/Actor.h"
#include "GridType.generated.h"

/**
 * The main grid actor that manages tile generation, visualization, and pathfinding.
 * Contains components for data, runtime state, and pathfinding.
 * Renders tiles using instanced static meshes with tactical overlays for modifiers.
 */
UCLASS()
class SFAF_API AGridType : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor for the grid actor */
	AGridType();

	/** Sets up collision responses for the grid meshes */
	void InitializeCollision() const;

protected:
	/** Initializes the grid and its components when the game starts */
	virtual void BeginPlay() override;

public:
	/** Updates the grid each frame */
	virtual void Tick(float DeltaTime) override;

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Component holding grid configuration and data asset reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridDataComponent> GridDataComponent;

	/** Component managing runtime tile states and static data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridRuntimeStateComponent> GridRuntimeStateComponent;

	/** Component handling neighbor discovery and pathfinding */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridPathfindingComponent> GridPathFindingComponent;
	
private:
	// -----------------------------------------------------------------------
	// Internal Configuration
	// -----------------------------------------------------------------------
	
	UPROPERTY()
	FVector GridLocation;

	/** Vertical spacing between grid layers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float GridVerticalDistance = 1.f;

	/** Maximum distance for ground traces during grid generation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float TraceRange = 10000.f;

	/** Radius of the sphere trace used for ground detection */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid|Config", meta = (AllowPrivateAccess = "true"))
	float TraceSphereRadius = 1.f;

	/** Root scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneRoot;

	/** Instanced mesh for the main grid tile visuals */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> GridMesh;

	/** Instanced mesh for obstacle tactical overlays */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalObstacleMesh;

	/** Instanced mesh for double-cost tactical overlays */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalDoubleCostMesh;

	/** Instanced mesh for triple-cost tactical overlays */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalTripleCostMesh;

	/** Instanced mesh for flying-only tactical overlays */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalFlyingOnlyMesh;
		
	/** Gameplay tags assigned to standard (walkable) tiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer StandardTileTags;
	
	/** Gameplay tags assigned to environment-blocked tiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer EnvironmentTileTags;
	
	/** Visual tag used for tactical overlays (default: Grid.Visual.Black) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	FGameplayTag TacticalVisualTag;
	
	/** Horizontal spacing multiplier between hex tiles */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float XOffset = 0.501f;

	/** Vertical spacing factor for hex grid layout */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float YOffset = 0.866025f * 2.3f;
	
	/** Actors to exclude from ground traces during grid generation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
	
	/** Small Z offset applied to grid meshes to prevent z-fighting */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float ZOffset = 0.01f;

public:
	// -----------------------------------------------------------------------
	// Grid API
	// -----------------------------------------------------------------------

	/** Removes all instanced mesh visuals */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void ClearInstancedMeshes() const;
	
	/** Returns the tactical instanced mesh component matching the given modifier tag */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	UInstancedStaticMeshComponent* SelectTacticMeshWithTag(FGameplayTag GridModifierTag) const;
	
	/** Returns the modifier gameplay tag active at the given grid coordinate */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	bool GetTileModifier(const FGridCoord& Coord, FGameplayTag& OutModifier) const;
	
	/** Adds an instance of the tile mesh (and any matching tactical meshes) at the given transform */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void AddInstanceMesh(const FGameplayTagContainer& TileTags, const FTransform& Transform);

	/** Removes an instanced mesh by tile type and instance index */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	bool RemoveInstanceMesh(int TileType, int Index) const;

	/** Returns the trace sphere radius used for ground detection */
	UFUNCTION(BlueprintPure, Category = "Grid|Trace")
	float GetTraceSphereRange() const { return TraceSphereRadius; }

	/**
	 * Registers a tile on the grid with its transform, position, tags, and optional unit.
	 * Adds the visual instance and registers static data with the runtime component.
	 */
	bool AddGridTileInstance(int32 TileIndex, const FTransform& TileTransform, FGridCoord TilePosition, bool bCheckForEquivalents,
	                         const FGameplayTagContainer& TileTags, ACombatant_Base* UnitOnTile);
	/** Returns true if the given tile tags describe a valid, addable tile */
	static bool CanAddTile(const FGameplayTagContainer& TileTags);

	/** Generates the entire grid at the given world location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GenerateGrid(const FVector Location);

	/** Shows or hides the tactical overlay meshes */
	UFUNCTION(BlueprintCallable, Category = "Grid|Visual")
	void ShowTacticalGrid(bool bShow);
	
	/** Destroys all generated grid tiles, meshes, and clears runtime data */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DestroyGridTiles();
	
	/** Returns the first tile coordinate registered during generation */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FGridCoord GetFirstTile () const;
    
	/** Returns the last tile coordinate registered during generation */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	FGridCoord GetLastTile () const;
	
	/** Returns true once the grid has been fully generated and is ready for gameplay */
	UFUNCTION(BlueprintCallable, Category = "Grid|Static")
	bool GetIsReady() const;

	/** Finds the nearest registered tile from a world-space location */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool FindNearestTileFromLocation(const FVector& WorldLocation, FGridCoord& OutCoord, FGridTileStaticData& OutTileData) const;
	
	/** Returns mutable static data for a tile. For Blueprint, use GridRuntimeStateComponent instead. */
	FGridTileStaticData* GetTileStaticData(FGridCoord Coord);
	
	/** Returns the movement cost to enter the tile at the given coordinate */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	int32 GetTileCost(FGridCoord Coord);
		
	/** Populates the array with neighbor coordinates for the given tile */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetNeighbors(FGridCoord Coord, TArray<FGridCoord>& Neighbors, bool bConsiderFly);
	
	/** Populates the array with all coordinates reachable within the given movement points */
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetReachableCoords(FGridCoord Coord, TArray<FGridCoord>& ReachableCoords, bool bIsFlying, int32 MovementPoints);

	/** Computes the path from Source to Target and populates PathCoords */
	bool GetPathCoords(FGridCoord Source, FGridCoord Target,
	                   TArray<FGridCoord>& PathCoords);

protected:
	// -----------------------------------------------------------------------
	// Internal Cache
	// -----------------------------------------------------------------------
	
	/** Whether the grid has finished generating and is ready for use */
	UPROPERTY(BlueprintReadOnly, Category = "Grid|Tile")
	bool bReady;
};
