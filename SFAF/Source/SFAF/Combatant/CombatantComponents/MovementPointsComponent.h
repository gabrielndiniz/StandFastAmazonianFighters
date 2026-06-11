// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "MovementPointsComponent.generated.h"

class UAbilitySystemComponent;
class UMovementAttributeSet;

/**
 * Component managing a combatant's movement points.
 * Integrates with GAS through UMovementAttributeSet for attribute-based tracking.
 * Falls back to raw int32 values when GAS is not available.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UMovementPointsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Default constructor */
    UMovementPointsComponent();

    /** Returns the current movement points */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    int32 GetMovementPoints() const;

    /** Returns the maximum movement points */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    int32 GetMaxMovementPoints() const;

    /** Returns true if the combatant has at least the given amount of movement points */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool HasEnoughMovementPoints(int32 Amount) const;

    /** Consumes the given amount of movement points (clamped to zero) */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void UseMovementPoints(int32 Amount);

    /** Restores movement points to the maximum value */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RestoreMovementPoints();

    /** Overrides the maximum movement points value */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMaxMovementPoints(int32 NewMax);

    /** Initializes the GAS attribute set and registers it with the owner's ASC */
    void InitGAS();

    /** Returns true if GAS initialization has completed */
    UFUNCTION(BlueprintCallable, Category = "Movement|GAS")
    bool IsGASReady() const { return bGASReady; }

    /** Returns the owner's Ability System Component */
    UFUNCTION(BlueprintCallable, Category = "Movement|GAS")
    UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

    /** Returns the movement speed attribute value (default 600.0) */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementSpeed() const;

protected:
    /** Initializes component state when the game starts */
    virtual void BeginPlay() override;

    /** Current movement points (backup value when GAS is not ready) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    int32 CurrentMovementPoints;

    /** Maximum movement points configurable in defaults */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    int32 MaxMovementPoints;

    /** The owner's Ability System Component (set during InitGAS) */
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    /** The movement attribute set subobject for GAS integration */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|GAS")
    TObjectPtr<UMovementAttributeSet> MovementAttributeSet;

    /** Whether GAS initialization has completed */
    bool bGASReady = false;
};
