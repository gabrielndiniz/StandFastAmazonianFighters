// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Grid/GridType.h"
#include "TacticalPlayerController.generated.h"

class UTacticalControllerComponent;

/**
 * Player controller for tactical combat. Handles grid interaction via mouse input,
 * determines which tile is under the cursor, and feeds coordinates to the
 * TacticalControllerComponent for action processing.
 */
UCLASS()
class SFAF_API ATacticalPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
	/** Default constructor */
	ATacticalPlayerController();
	
	/** Initializes the controller when the game starts */
	virtual void BeginPlay() override;
	/** Updates the controller every frame */
	virtual void Tick(float DeltaSeconds) override;
	/** Sets up input bindings for the controller */
	virtual void SetupInputComponent() override;
	
	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Component managing tactical action coordination (hover, select, target, pathfinding) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTacticalControllerComponent> TacticalControllerComponent;
	
	// -----------------------------------------------------------------------
	// Data
	// -----------------------------------------------------------------------
	
	/** Reference to the active grid actor for tile queries */
	UPROPERTY()
	AGridType* Grid;
	
	// -----------------------------------------------------------------------
	// Action
	// -----------------------------------------------------------------------

	/** Traces under the cursor and pushes the hovered tile coordinates to the tactical component */
	void SetTileUnderCursor() const;
	
	/** Selects the currently hovered tile and updates all related action components */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool SelectTile() const;
	
	/** Confirms the currently hovered tile as the target */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool TargetTile() const;
	
	/** When true, SelectTile also updates the AddRemoveUnit action component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bUseAddRemoveUnit = false;
	
	/** Enables or disables AddRemoveUnit coordination during selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetUseAddRemoveUnit(bool bIn) { bUseAddRemoveUnit = bIn; }
	
	/** Returns whether AddRemoveUnit is coordinated during selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetUseAddRemoveUnit() { return bUseAddRemoveUnit; }	
	
	/** When true, SelectTile also updates the PathTiles target coordinate */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bPathTileHover = false;
	
	/** Enables or disables path tile hover coordination */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetPathTileHover(bool bIn) { bPathTileHover = bIn; }
	
	/** Returns whether path tile hover is active */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetPathTileHover() { return bPathTileHover; }
	
	/** When true, TargetTile also updates the PathTiles target coordinate */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bPathTileTarget = false;
	
	/** Enables or disables path tile target coordination */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetPathTileTarget(bool bIn) { bPathTileTarget = bIn; }
	
	/** Returns whether path tile target is active */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetPathTileTarget() { return bPathTileTarget; }

	/** Left-click handler: activates path hover mode */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnLeftClick();

	/** Right-click handler: deactivates path hover mode */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnRightClick();
};
