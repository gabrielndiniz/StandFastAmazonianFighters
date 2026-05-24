// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Combat/Team/TeamSubsystem.h"
#include "TacticalControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UTacticalControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTacticalControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Set Team Number.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Set Team Number"))
	void SetTeamNumber(int32 NewTeamNumber);
	
	/** Return Team Number.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Set Team Number"))
	int32 GetTeamNumber();
	
	/** The Team Number of the owner.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	int32 TeamNumber = 0;
};
