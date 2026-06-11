// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "MoveCombatant.generated.h"

class ACombatant_Base;
class UMovementPointsComponent;

/** Delegate broadcast when a move sequence completes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveCombatantComplete);

/**
 * Action component that executes combatant movement along a precomputed path.
 * Handles step-by-step tile traversal, movement point consumption, and occupancy updates.
 * Uses a timer-driven approach for smooth path progression.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UMoveCombatant : public UBaseActionComponent
{
    GENERATED_BODY()

public:
    /**
     * Constructs the MoveCombatant component.
     */
    UMoveCombatant();

    /**
     * Executes combatant movement along the stored path.
     * @param InSourceCoord The source grid coordinate.
     * @param bHasHit Whether a valid hit was detected.
     * @param InTargetCoord The target grid coordinate.
     * @return True if movement was successfully initiated.
     */
    virtual bool Execute_Implementation(
        const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord) override;

    /** Sets the ordered path tiles the combatant will follow */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovePath(const TArray<FGridCoord>& InSortedPath);

    /** Sets the time (in seconds) between each tile traversal step */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetTileMoveDuration(float NewDuration);

    /** Event fired when the entire movement sequence finishes */
    UPROPERTY(BlueprintAssignable, Category = "Movement")
    FMoveCombatantComplete OnMoveComplete;

protected:
    /** Returns the combatant occupying the tile at the given coordinate */
    ACombatant_Base* GetCombatantAtCoord(const FGridCoord& Coord) const;

    /** Initiates the movement sequence from the first path tile */
    void StartPathMovement();

    /** Advances the combatant to the next tile in the path */
    void AdvancePathTile();

    /** Teleports the combatant actor to the current path tile's world location */
    void MoveToCurrentTile();

    /** Finalizes movement: updates occupancy, deducts MP, and broadcasts completion */
    void FinishMovement();

    /** Ordered list of grid coordinates forming the movement path */
    UPROPERTY()
    TArray<FGridCoord> MovePath;

    /** World-space locations corresponding to each path tile */
    TArray<FVector> MovePathWorld;

    /** The combatant currently being moved */
    UPROPERTY()
    TWeakObjectPtr<ACombatant_Base> MovingCombatant;

    /** Movement points component of the moving combatant */
    TWeakObjectPtr<UMovementPointsComponent> MovingComp;

    /** Index of the current tile being traversed in the path */
    int32 CurrentPathIndex = 0;

    /** Total movement cost of the entire path */
    int32 MoveTotalCost = 0;

    /** Whether a movement sequence is currently in progress */
    bool bIsMoving = false;

    /** Duration in seconds for traversing each tile */
    UPROPERTY(EditAnywhere, Category = "Movement")
    float TileMoveDuration = 2.0f;

    /** Timer handle for step-by-step path advancement */
    FTimerHandle MovementTimerHandle;
};
