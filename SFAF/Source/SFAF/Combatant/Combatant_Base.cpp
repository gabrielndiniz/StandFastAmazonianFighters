// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "Combatant/Combatant_Base.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CombatantComponents/TeamComponent.h"
#include "Core/Combat/Team/TeamSubsystem.h"

ACombatant_Base::ACombatant_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("TeamComponent"));
}

UAbilitySystemComponent* ACombatant_Base::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACombatant_Base::InitAbilityActorInfo()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ACombatant_Base::DestroyUnit()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearActorInfo();
	}

	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors);
	for (AActor* Child : ChildActors)
	{
		if (Child)
		{
			Child->Destroy();
		}
	}

	Destroy();
}

int32 ACombatant_Base::GetTeam() const
{
	if (TeamComponent)
	{
		return TeamComponent->TeamNumber;
	}
	return 0;
}

void ACombatant_Base::SetTeam(int32 NewTeam)
{
	if (!TeamComponent)
	{
		return;
	}

	UTeamSubsystem* TeamSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UTeamSubsystem>() : nullptr;

	int32 OldTeam = TeamComponent->TeamNumber;
	TeamComponent->TeamNumber = NewTeam;

	if (TeamSubsystem)
	{
		if (OldTeam != 0)
		{
			TeamSubsystem->UnregisterUnitFromTeam(this, OldTeam);
		}
		TeamSubsystem->RegisterUnitToTeam(this, NewTeam);
	}

	OnTeamChanged.Broadcast(NewTeam);
}
