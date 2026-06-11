// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

/** Standard GAS macro that generates getter, setter, and initter functions for attributes */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute set defining movement-related attributes for combatants.
 * Includes current movement points, maximum movement points, and movement speed.
 */
UCLASS()
class SFAF_API UMovementAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    /** Default constructor */
    UMovementAttributeSet();

    /** Current available movement points */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MovementPoints;

    /** Maximum possible movement points */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MaxMovementPoints;

    /** Base movement speed for the combatant */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MovementSpeed;

    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementPoints);
    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxMovementPoints);
    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSpeed);
};
