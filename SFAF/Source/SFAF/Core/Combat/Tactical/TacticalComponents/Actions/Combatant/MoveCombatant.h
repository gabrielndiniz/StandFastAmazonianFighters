// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "MoveCombatant.generated.h"

class ACombatant_Base;
class UMovementPointsComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveCombatantComplete);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UMoveCombatant : public UBaseActionComponent
{
    GENERATED_BODY()

public:
    UMoveCombatant();

    virtual bool Execute_Implementation(
        const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovePath(const TArray<FGridCoord>& InSortedPath);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetTileMoveDuration(float NewDuration);

    UPROPERTY(BlueprintAssignable, Category = "Movement")
    FMoveCombatantComplete OnMoveComplete;

protected:
    ACombatant_Base* GetCombatantAtCoord(const FGridCoord& Coord) const;

    void StartPathMovement();
    void AdvancePathTile();
    void MoveToCurrentTile();
    void FinishMovement();

    UPROPERTY()
    TArray<FGridCoord> MovePath;

    TArray<FVector> MovePathWorld;

    UPROPERTY()
    TWeakObjectPtr<ACombatant_Base> MovingCombatant;

    TWeakObjectPtr<UMovementPointsComponent> MovingComp;

    int32 CurrentPathIndex = 0;
    int32 MoveTotalCost = 0;
    bool bIsMoving = false;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float TileMoveDuration = 2.0f;

    FTimerHandle MovementTimerHandle;
};
