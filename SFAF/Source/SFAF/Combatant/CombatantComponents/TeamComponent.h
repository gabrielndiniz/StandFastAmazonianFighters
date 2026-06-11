// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TeamComponent.generated.h"

/** Component that assigns a team affiliation to a combatant actor and registers it with the global TeamSubsystem */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UTeamComponent();

protected:
	/** Initializes team component state when the game starts */
	virtual void BeginPlay() override;

public:
	/** Numeric identifier for the team this component belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	int32 TeamNumber;

	/** Registers the owning actor with the global TeamSubsystem based on TeamNumber */
	void RegisterToTeamSubsystem();
};
