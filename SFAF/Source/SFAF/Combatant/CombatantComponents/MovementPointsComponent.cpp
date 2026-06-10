// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "MovementPointsComponent.h"
#include "AbilitySystemComponent.h"
#include "Combatant/GAS/MovementAttributeSet.h"

UMovementPointsComponent::UMovementPointsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    MaxMovementPoints = 100;
    CurrentMovementPoints = 100;

    MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("MovementAttributeSet"));
}

void UMovementPointsComponent::BeginPlay()
{
    Super::BeginPlay();

    InitGAS();
}

void UMovementPointsComponent::InitGAS()
{
    AActor* Owner = GetOwner();
    if (!Owner || !Owner->Implements<UAbilitySystemInterface>())
    {
        CurrentMovementPoints = MaxMovementPoints;
        return;
    }

    AbilitySystemComponent = Cast<IAbilitySystemInterface>(Owner)->GetAbilitySystemComponent();
    if (!AbilitySystemComponent)
    {
        CurrentMovementPoints = MaxMovementPoints;
        return;
    }

    AbilitySystemComponent->AddAttributeSetSubobject(MovementAttributeSet.Get());

    if (AbilitySystemComponent->IsOwnerActorAuthoritative())
    {
        AbilitySystemComponent->SetNumericAttributeBase(
            UMovementAttributeSet::GetMovementPointsAttribute(), MaxMovementPoints);
        AbilitySystemComponent->SetNumericAttributeBase(
            UMovementAttributeSet::GetMaxMovementPointsAttribute(), MaxMovementPoints);
    }

    CurrentMovementPoints = FMath::RoundToInt(
        AbilitySystemComponent->GetNumericAttribute(UMovementAttributeSet::GetMovementPointsAttribute()));

    bGASReady = true;
}

int32 UMovementPointsComponent::GetMovementPoints() const
{
    if (bGASReady && AbilitySystemComponent)
    {
        return FMath::RoundToInt(
            AbilitySystemComponent->GetNumericAttribute(UMovementAttributeSet::GetMovementPointsAttribute()));
    }
    return CurrentMovementPoints;
}

int32 UMovementPointsComponent::GetMaxMovementPoints() const
{
    if (bGASReady && AbilitySystemComponent)
    {
        return FMath::RoundToInt(
            AbilitySystemComponent->GetNumericAttribute(UMovementAttributeSet::GetMaxMovementPointsAttribute()));
    }
    return MaxMovementPoints;
}

bool UMovementPointsComponent::HasEnoughMovementPoints(int32 Amount) const
{
    return GetMovementPoints() >= Amount;
}

void UMovementPointsComponent::UseMovementPoints(int32 Amount)
{
    int32 NewValue = FMath::Max(0, GetMovementPoints() - Amount);

    if (bGASReady && AbilitySystemComponent)
    {
        AbilitySystemComponent->SetNumericAttributeBase(
            UMovementAttributeSet::GetMovementPointsAttribute(), NewValue);
    }

    CurrentMovementPoints = NewValue;
}

void UMovementPointsComponent::RestoreMovementPoints()
{
    int32 NewValue = GetMaxMovementPoints();

    if (bGASReady && AbilitySystemComponent)
    {
        AbilitySystemComponent->SetNumericAttributeBase(
            UMovementAttributeSet::GetMovementPointsAttribute(), NewValue);
    }

    CurrentMovementPoints = NewValue;
}

float UMovementPointsComponent::GetMovementSpeed() const
{
    if (bGASReady && AbilitySystemComponent)
    {
        return AbilitySystemComponent->GetNumericAttribute(
            UMovementAttributeSet::GetMovementSpeedAttribute());
    }
    return 600.0f;
}

void UMovementPointsComponent::SetMaxMovementPoints(int32 NewMax)
{
    MaxMovementPoints = FMath::Max(0, NewMax);

    if (bGASReady && AbilitySystemComponent)
    {
        AbilitySystemComponent->SetNumericAttributeBase(
            UMovementAttributeSet::GetMaxMovementPointsAttribute(), MaxMovementPoints);
    }

    if (CurrentMovementPoints > MaxMovementPoints)
    {
        CurrentMovementPoints = MaxMovementPoints;
    }
}
