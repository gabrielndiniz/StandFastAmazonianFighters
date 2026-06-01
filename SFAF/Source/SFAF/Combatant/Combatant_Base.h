// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permissi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CombatantComponents/TeamComponent.h"
#include "Combatant_Base.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

// ---------------------------------------------------------------------------
// Actor
// ---------------------------------------------------------------------------

UCLASS()
class SFAF_API ACombatant_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACombatant_Base();

	// -----------------------------------------------------------------------
	// Interface
	// -----------------------------------------------------------------------

	/** Returns the ability system component for this unit */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns the gameplay tag identifying the unit type */
	UFUNCTION(BlueprintCallable, Category="Combatant|GAS")
	FGameplayTag GetUnitTypeTag() const { return UnitTypeTag; }
	
protected:

	/** Initializes the Ability System Component's actor info for this combatant */
	void InitAbilityActorInfo();

	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Ability System Component for GAS integration */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Attribute Set for unit statistics */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Component for Team Management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|Team")
	TObjectPtr<UTeamComponent> TeamComponent;
	
	// -----------------------------------------------------------------------
	// Data
	// -----------------------------------------------------------------------

	/** Gameplay tag representing the unit type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combatant|Unit")
	FGameplayTag UnitTypeTag;
	
	
};
