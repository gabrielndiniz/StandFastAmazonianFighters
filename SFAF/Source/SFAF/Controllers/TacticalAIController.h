// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TacticalAIController.generated.h"

class UTacticalControllerComponent;

/** AI controller for tactical combat. Uses the tactical component for automated decision-making. */
UCLASS()
class SFAF_API ATacticalAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATacticalAIController();

	virtual void BeginPlay() override;
	
	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	/** Component managing tactical actions for AI-driven units */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTacticalControllerComponent> TacticalControllerComponent;
	
	
};
