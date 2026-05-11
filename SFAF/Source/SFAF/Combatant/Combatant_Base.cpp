// Â© 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel NÃ³bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permissi


#include "Combatant/Combatant_Base.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

ACombatant_Base::ACombatant_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ACombatant_Base::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACombatant_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatant_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	SetOwner(NewController);
}

void ACombatant_Base::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void ACombatant_Base::InitAbilityActorInfo()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
