// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SpellAuxiliaryEffect.generated.h"

UCLASS(Abstract, Blueprintable)
class SFAF_API ASpellAuxiliaryEffect : public AActor
{
	GENERATED_BODY()

public:
	ASpellAuxiliaryEffect();

	/** 
	 * Type of auxiliary effect (e.g., Summon, Teleport).
	 * Used by the spell system to determine how to handle this effect.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FGameplayTag AuxiliaryTypeTag;

	/** Duration of the effect before it is automatically destroyed (0 for infinite/manual) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration = 0.0f;
};
