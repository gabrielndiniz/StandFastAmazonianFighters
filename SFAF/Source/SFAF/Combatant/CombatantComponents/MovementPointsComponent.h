// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "MovementPointsComponent.generated.h"

class UAbilitySystemComponent;
class UMovementAttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UMovementPointsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMovementPointsComponent();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    int32 GetMovementPoints() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    int32 GetMaxMovementPoints() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool HasEnoughMovementPoints(int32 Amount) const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void UseMovementPoints(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RestoreMovementPoints();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMaxMovementPoints(int32 NewMax);

    void InitGAS();

    UFUNCTION(BlueprintCallable, Category = "Movement|GAS")
    bool IsGASReady() const { return bGASReady; }

    UFUNCTION(BlueprintCallable, Category = "Movement|GAS")
    UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementSpeed() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    int32 CurrentMovementPoints;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    int32 MaxMovementPoints;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|GAS")
    TObjectPtr<UMovementAttributeSet> MovementAttributeSet;

    bool bGASReady = false;
};
