// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Grid/GridType.h"
#include "TacticalPlayerController.generated.h"

class UTacticalControllerComponent;

/**
 * 
 */
/** Player controller for tactical combat. Handles grid interaction and input-driven actions. */
UCLASS()
class SFAF_API ATacticalPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
	ATacticalPlayerController();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	
	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Component managing tactical actions (select, hover, target, pathfinding) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTacticalControllerComponent> TacticalControllerComponent;
	
	// -----------------------------------------------------------------------
	// Data
	// -----------------------------------------------------------------------
	
	/** Reference to the active grid actor */
	UPROPERTY()
	AGridType* Grid;
	
	
	// -----------------------------------------------------------------------
	// Action
	// -----------------------------------------------------------------------
	/** Calculates which tile is currently under the mouse cursor and pushes it to the tactical component */
	void SetTileUnderCursor() const;
	
	/** Handles tile selection logic, returns true if a valid tile was selected */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool SelectTile() const;
	
	/** Handles target confirmation on the currently selected tile */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool TargetTile() const;
	
	/** Whether AddCoordToComponent should be called inside SelectTile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bUseAddRemoveUnit = false;
	
	/** Sets whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetUseAddRemoveUnit(bool bIn) { bUseAddRemoveUnit = bIn; }
	
	/** Return whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetUseAddRemoveUnit() { return bUseAddRemoveUnit; }	
	
	/** Whether AddCoordToComponent should be called inside SelectTile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bPathTileHover = false;
	
	/** Sets whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetPathTileHover(bool bIn) { bPathTileHover = bIn; }
	
	/** Return whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetPathTileHover() { return bPathTileHover; }
	
	/** Whether AddCoordToComponent should be called inside SelectTile */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	mutable bool bPathTileTarget = false;
	
	/** Sets whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetPathTileTarget(bool bIn) { bPathTileTarget = bIn; }
	
	/** Return whether AddCoordToComponent should be used during tile selection */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetPathTileTarget() { return bPathTileTarget; }
};
