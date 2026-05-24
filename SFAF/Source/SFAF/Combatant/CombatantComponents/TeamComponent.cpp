#include "TeamComponent.h"

#include "Combatant/Combatant_Base.h"
#include "Core/Combat/Team/TeamSubsystem.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	RegisterToTeamSubsystem();
}

void UTeamComponent::RegisterToTeamSubsystem()
{
	if (!GetWorld())
	{
		return;
	}

	UTeamSubsystem* TeamSubsystem =
		GetWorld()->GetSubsystem<UTeamSubsystem>();

	if (!TeamSubsystem)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
	{
		return;
	}
	
	ACombatant_Base* Unit = Cast<ACombatant_Base>(OwnerActor);
	
	if (!Unit)
	{
		return;
	}

	TeamSubsystem->RegisterUnitToTeam(
		Unit,
		TeamNumber);
}
