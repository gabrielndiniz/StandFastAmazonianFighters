// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permissi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Combatant_Base.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SFAF_API ACombatant_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACombatant_Base();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Combatant|GAS")
	FGameplayTag GetUnitTypeTag() const { return UnitTypeTag; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combatant|GAS")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combatant|Unit")
	FGameplayTag UnitTypeTag;
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void InitAbilityActorInfo();
};
