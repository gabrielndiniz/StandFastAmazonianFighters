// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAnimation.generated.h"

/** Standard GAS macro that generates getter, setter, and initter functions for attributes */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute set for animation-related modifiers.
 * Currently a placeholder for future animation-driven attributes (e.g., AttackSpeed).
 */
UCLASS()
class SFAF_API UBaseAnimation : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Global animation speed multiplier for the combatant */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Animation")
	FGameplayAttributeData PlayRate;
	ATTRIBUTE_ACCESSORS(UBaseAnimation, PlayRate);
};
