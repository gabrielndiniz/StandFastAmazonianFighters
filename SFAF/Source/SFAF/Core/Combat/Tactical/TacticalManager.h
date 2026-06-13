// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/TacticalAIController.h"
#include "Controllers/TacticalPlayerController.h"
#include "Controllers/ControllerComponents/TacticalControllerComponent.h"
#include "GameFramework/Actor.h"
#include "Grid/GridType.h"
#include "TacticalComponents/Actions/BaseActionComponent.h"
#include "TacticalComponents/Actions/Interface/HoverTile.h"
#include "TacticalComponents/Actions/Interface/SelectTile.h"
#include "TacticalComponents/Actions/Interface/TargetTile.h"
#include "TacticalComponents/Actions/Pathfinding/AddRemoveUnit.h"
#include "TacticalComponents/Actions/Pathfinding/NeighborTile.h"
#include "TacticalComponents/Actions/Pathfinding/PathTiles.h"
#include "TacticalComponents/Actions/Pathfinding/ReachableTiles.h"
#include "TacticalComponents/Actions/Combatant/MoveCombatant.h"
#include "TacticalManager.generated.h"

/** Bundles references to a team's player controller, AI controller, and tactical component */
USTRUCT(BlueprintType)
struct FControllers
{
	GENERATED_BODY()
	
	/** Whether this team is controlled by a human player */
	bool bIsPlayer = false;
	
	/** The player controller for human-controlled teams */
	UPROPERTY()
	ATacticalPlayerController* PlayerController = nullptr;
	
	/** The AI controller for AI-controlled teams */
	UPROPERTY()
	ATacticalAIController* AIController = nullptr;
	
	/** Shared tactical controller component managing action coordination */
	UPROPERTY()
	UTacticalControllerComponent* ControllerComponent = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteActionSignature, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalculateReachableTilesSignature, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalculatePathTilesSignature, bool, bSuccess);

/**
 * Central tactical manager that orchestrates grid-based combat actions.
 * Manages team controller assignments, action components (hover, select, target,
 * pathfinding, movement), and coordinates the execution of tactical abilities.
 */
UCLASS()
class SFAF_API ATacticalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Constructs the TacticalManager actor.
	 */
	ATacticalManager();

protected:
	/**
	 * Initializes action components and registers meshes on game start.
	 */
	virtual void BeginPlay() override;
	
public:	
	/**
	 * Updates per-frame hover and pathfinding state.
	 * @param DeltaTime Frame tick delta.
	 */
	virtual void Tick(float DeltaTime) override;

	/** Root scene component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> SceneRoot;
	
	// -----------------------------------------------------------------------
	// Meshes
	// -----------------------------------------------------------------------
	
	/** Instanced mesh visualizing the currently hovered tile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> HoverMesh;
	
	/** Instanced mesh visualizing the selected tile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> SelectMesh;
		
	/** Instanced mesh visualizing the target tile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> TargetMesh;
	
	/** Instanced mesh visualizing neighbor tiles */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> NeighborMesh;
	
	/** Instanced mesh visualizing reachable tiles */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> ReachableMesh;
	
	/** Instanced mesh visualizing the computed path tiles */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> PathMesh;

	// -----------------------------------------------------------------------
	// Runtime
	// -----------------------------------------------------------------------
	
	/** Initializes the tactical manager: finds the grid, creates action components, and registers meshes */
	void Initiate();
	
	/** Assigns the grid actor to be managed */
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	void SetGrid(AGridType* GridType);
	
	/** Returns the currently managed grid actor */
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	AGridType* GetGrid();
	
	/** The active grid actor reference */
	UPROPERTY(BlueprintReadOnly, Category="Runtime")
	AGridType* Grid;
		
	// -----------------------------------------------------------------------
	// Actions - Components
	// -----------------------------------------------------------------------
	
	/** Sets a specific player controller as the debug controller for testing */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetDebugController (ATacticalPlayerController* Controller);
		
	/** Maps team numbers to their controller bundles */
	UPROPERTY()
	TMap<int32,FControllers> TeamsControllers;
		
	/** Controller bundle used when debug mode is active */
	UPROPERTY()
	FControllers DebugController;
	
	/** Enables debug mode, bypassing team turn logic */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	bool bDebug = false;
	
	/** Component handling hover tile visuals and logic */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UHoverTile* HoverTile;
	
	/** Component handling tile selection visuals and logic */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USelectTile* SelectTile;
	
	/** Component handling target tile visuals and logic */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UTargetTile* TargetTile;
	
	/** Component handling neighbor tile discovery and visuals */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UNeighborTile* NeighborTile;
	
	/** Component handling reachable tile calculations and visuals */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UReachableTiles* ReachableTiles;
	
	/** Component handling adding and removing units from the grid */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UAddRemoveUnit* AddRemoveUnit;
	
	/** Component handling path tile calculations and visuals */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UPathTiles* PathTiles;

	/** Component handling combatant movement execution along a path */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UMoveCombatant* MoveCombatant;
	
	/** Maps action components to their corresponding visualization meshes */
	UPROPERTY()
	TMap<UBaseActionComponent*, UInstancedStaticMeshComponent*> ComponentMesh;	
	
	/** The currently active team number */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	int32 CurrentTeam = 1;
	
	/** Maps action components to their sequenced follow-up actions */
	UPROPERTY()
	TMap<UBaseActionComponent*, UBaseActionComponent*> SequencedActions;
	
	/** Z offset applied to action visualization meshes */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	float ZOffset = 0.01f;
	
	/** Enables continuous path recalculation as the cursor moves */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
    bool bScanPath = false;

	/** Whether to use the target tile (instead of hover) for path calculation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid|Visual", meta = (AllowPrivateAccess = "true"))
	bool bUseTarget = false;

	/** True while a movement ability is executing; blocks other actions */
	bool bIsExecutingAbility = false;
	
	// -----------------------------------------------------------------------
	// Actions - Functions
	// -----------------------------------------------------------------------
	
	/** Executes a given action component using the currently active team's controller data */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void ExecuteAction(UBaseActionComponent* ActionComponent);	
	
	/** Sets whether flying movement is considered for all pathfinding actions */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetConsiderFlying(bool bConsider);
		
	/** Returns the controller data for the currently active team */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool GetCurrentTeam(FControllers& OutControllers) const;
	
	/** Returns controller data for all registered teams that have valid controllers */
	TArray<FControllers> GetAllTeams() const;

	/** Updates the movement points available for reachability calculations */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetCurrentMovementPoints(int32 Points);
	
	/** Configures AddRemoveUnit with spawn parameters: unit tag, team, add/remove flag, and change-team flag */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StartAddOrRemoveUnit(FGameplayTag InUnit, int32 InTeam, bool bInAdd, bool bInChangeTeam) const;
	
	/**
	 * Computes reachable tiles from the SelectTile's source coordinate within the specified movement budget.
	 * Updates the ReachableMesh visual instances and returns the results.
	 * @param MovementPoints     The movement point budget for reachability.
	 * @param bFlying            Whether flying movement is considered.
	 * @param OutReachableTiles  All grid coordinates reachable within the budget.
	 * @param OutLocations       World-space locations for mesh placement.
	 * @return True if at least one reachable tile was found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool CalculateReachableTiles(
		int32 MovementPoints,
		bool bFlying,
		TArray<FGridCoord>& OutReachableTiles, TArray<FVector>& OutLocations
		);

	/**
	 * Computes the path from the selected tile to the hover or target tile.
	 * @param OutPathTiles   The ordered path coordinates.
	 * @param OutLocations   World-space locations for mesh placement.
	 * @return True if a valid path was found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool CalculatePathTiles(
		TArray<FGridCoord>& OutPathTiles, TArray<FVector>& OutLocations
	);
	
	/** Enables or disables continuous path scanning */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetScanPath(bool bNewScanPath);
	
	/** Returns whether continuous path scanning is active */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool GetScanPath() const;

	/** Enables or disables use of the target tile (instead of hover) for path calculation */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetUseTarget(bool bNewUseTarget);

	/** Returns whether target tile is used for path calculation */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool GetUseTarget() const;
		
	/** Applies the current AddRemoveUnit configuration to the selected tile */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool ApplyAddRemoveUnit();

	/** Executes a full unit move: validates, sorts path, and triggers movement */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool AbilityMoveUnit();

	/**
	 * Populates TeamsControllers for teams 0-7.
	 * Teams listed in PlayerTeams become player-controlled (ATacticalPlayerController).
	 * Teams not in the list become AI-controlled (ATacticalAIController).
	 */
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetupTeams(const TArray<int32>& PlayerTeams);

	/** Sets the per-tile movement duration on the MoveCombatant component */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetMoveDuration(float NewDuration);

	/** Called when a MoveCombatant movement sequence completes */
	UFUNCTION()
	void OnMoveComplete();

	/** Called when ExecuteAction finishes */
	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnExecuteActionSignature OnExecuteAction;

	/** Called when CalculateReachableTiles finishes */
	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnCalculateReachableTilesSignature OnCalculateReachableTiles;

	/** Called when CalculatePathTiles finishes */
	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnCalculatePathTilesSignature OnCalculatePathTiles;
};
