#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TeamComponent.generated.h"

/** Component that assigns a team affiliation to a combatant actor */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Sets default values for this component's properties */
	UTeamComponent();

protected:

	virtual void BeginPlay() override;

public:

	/** Numeric identifier for the team this component belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	int32 TeamNumber;

	/** Registers the owning actor with the global TeamSubsystem */
	void RegisterToTeamSubsystem();
};
