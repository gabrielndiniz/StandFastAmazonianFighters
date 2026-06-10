// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

// Standard Macro from GAS
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SFAF_API UMovementAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UMovementAttributeSet();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MovementPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MaxMovementPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FGameplayAttributeData MovementSpeed;

    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementPoints);
    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MaxMovementPoints);
    ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSpeed);
};
