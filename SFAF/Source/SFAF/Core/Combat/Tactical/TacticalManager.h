// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/TacticalAIController.h"
#include "Controllers/TacticalPlayerController.h"
#include "Controllers/ControllerComponents/TacticalControllerComponent.h"
#include "GameFramework/Actor.h"
#include "TacticalComponents/Actions/BaseActionComponent.h"
#include "TacticalComponents/Actions/Interface/HoverTile.h"
#include "TacticalComponents/Actions/Interface/SelectTile.h"
#include "TacticalComponents/Actions/Interface/TargetTile.h"
#include "TacticalComponents/Actions/Pathfinding/NeighborTile.h"
#include "TacticalComponents/Actions/Pathfinding/ReachableTiles.h"
#include "TacticalManager.generated.h"

class AGridType;
/** Bundles references to a team's player controller, AI controller, and tactical component */
USTRUCT(BlueprintType)
struct FControllers
{
	GENERATED_BODY()
	
	/** Whether this team is controlled by a human player */
	bool bIsPlayer = false;
	
	/** Player controller for human-controlled teams */
	ATacticalPlayerController* PlayerController = nullptr;
	
	/** AI controller for AI-controlled teams */
	ATacticalAIController* AIController = nullptr;
	
	/** Shared tactical controller component for action management */
	UTacticalControllerComponent* ControllerComponent = nullptr;
};


UCLASS()
class SFAF_API ATacticalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacticalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	/** Root scene component for the manager actor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> SceneRoot;
	
	// -----------------------------------------------------------------------
	// Meshes
	// -----------------------------------------------------------------------
	
	/** Visual representation of the Hovered Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> HoverMesh;
	
	/** Visual representation of the selected Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> SelectMesh;
		
	/** Visual representation of the target Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> TargetMesh;
	
	/** Visual representation of the neighbor Tiles*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> NeighborMesh;
	
	/** Visual representation of the reachable Tiles*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> ReachableMesh;
	// -----------------------------------------------------------------------
	// Runtime
	// -----------------------------------------------------------------------
	
	/** Initializes the tactical manager, creating action components and registering meshes */
	void Initiate();
	
	/** Assigns the grid actor to be managed */
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	void SetGrid(AGridType* GridType);
	
	/** Returns the currently managed grid actor */
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	AGridType* GetGrid();
	
	/** Reference to the active grid actor */
	UPROPERTY(BlueprintReadOnly, Category="Runtime")
	AGridType* Grid;
		
	// -----------------------------------------------------------------------
	// Actions - Components
	// -----------------------------------------------------------------------
	
	
	/** Sets a specific controller as the debug controller for testing */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetDebugController (ATacticalPlayerController* Controller);
		
	/** Maps team numbers to their controller bundles */
	UPROPERTY()
	TMap<int32,FControllers> TeamsControllers;
		
	/** Controller bundle used for debug/testing purposes */
	UPROPERTY()
	FControllers DebugController;
	
	
	/** Enables debug mode for the tactical manager */
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
	
	/** Component handling neighbor tile discovery */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UNeighborTile* NeighborTile;
	
	/** Component handling reachable tile calculations */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UReachableTiles* ReachableTiles;
	
	/** Mapping the Actions with their Static Meshes*/
	UPROPERTY()
	TMap<UBaseActionComponent*, UInstancedStaticMeshComponent*> ComponentMesh;	
	
	/** Current Team*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	int32 CurrentTeam = 1;
	
	/** Mapping the Actions with sequences*/
	UPROPERTY()
	TMap<UBaseActionComponent*, UBaseActionComponent*> SequencedActions;
	
	// -----------------------------------------------------------------------
	// Actions - Functions
	// -----------------------------------------------------------------------
	
	/** Executes a given action component on the currently active team */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void ExecuteAction(UBaseActionComponent* ActionComponent);	
	
	/** Sets whether flying movement is considered for all pathfinding actions */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetConsiderFlying(bool bConsider);
		
	/** Returns the controller data for the currently active team */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool GetCurrentTeam(FControllers& OutControllers) const;
	
	/** Returns controller data for all registered teams */
	TArray<FControllers> GetAllTeams() const;

	/** Updates the current movement points for reachability calculations */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetCurrentMovementPoints(int32 Points);
};
