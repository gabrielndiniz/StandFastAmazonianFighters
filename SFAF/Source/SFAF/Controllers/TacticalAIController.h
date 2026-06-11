// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TacticalAIController.generated.h"

class UTacticalControllerComponent;

/** AI controller for tactical combat. Uses the TacticalControllerComponent for automated decision-making. */
UCLASS()
class SFAF_API ATacticalAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	/** Default constructor */
	ATacticalAIController();

	/** Initializes the controller when the game starts */
	virtual void BeginPlay() override;
	
	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Component managing tactical action coordination for AI-driven units */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTacticalControllerComponent> TacticalControllerComponent;
};
