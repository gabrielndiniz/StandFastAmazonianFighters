#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TeamComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTeamComponent();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	int32 TeamNumber;

	void RegisterToTeamSubsystem();
};
