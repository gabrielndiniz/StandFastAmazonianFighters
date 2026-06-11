// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CombatantComponents/TeamComponent.h"
#include "Combatant_Base.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/** Delegate broadcast when a combatant's team assignment changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamChanged, int32, NewTeam);

/**
 * Base combatant class for all tactical units.
 * Implements the Ability System Interface for GAS integration.
 * Manages team affiliation and provides the core unit API.
 */
UCLASS()
class SFAF_API ACombatant_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Default constructor */
	ACombatant_Base();

	// -----------------------------------------------------------------------
	// Interface
	// -----------------------------------------------------------------------

	/** Returns the Ability System Component for GAS integration */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns the gameplay tag identifying this unit's type (e.g., Unit.Type.BulletAnt) */
	UFUNCTION(BlueprintCallable, Category="Combatant|GAS")
	FGameplayTag GetUnitTypeTag() const { return UnitTypeTag; }

	/** Cleans up abilities and child actors, then destroys the unit */
	UFUNCTION(BlueprintCallable, Category="Combatant")
	void DestroyUnit();

	/** Returns the current team number */
	UFUNCTION(BlueprintCallable, Category="Combatant|Team")
	int32 GetTeam() const;

	/** Sets the team number and handles team subsystem registration */
	UFUNCTION(BlueprintCallable, Category="Combatant|Team")
	void SetTeam(int32 NewTeam);

	/** Event broadcast whenever this unit's team changes */
	UPROPERTY(BlueprintAssignable, Category="Combatant|Team")
	FOnTeamChanged OnTeamChanged;
	
protected:
	/** Initializes the Ability System Component's owner and avatar actor info */
	void InitAbilityActorInfo();

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Ability System Component for Gameplay Ability System integration */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Attribute Set containing unit base stats */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Component for team affiliation management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|Team")
	TObjectPtr<UTeamComponent> TeamComponent;
	
	// -----------------------------------------------------------------------
	// Data
	// -----------------------------------------------------------------------

	/** Gameplay tag representing the specific unit type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combatant|Unit")
	FGameplayTag UnitTypeTag;
};
