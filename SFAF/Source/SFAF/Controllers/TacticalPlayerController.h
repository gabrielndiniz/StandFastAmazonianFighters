// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TacticalPlayerController.generated.h"

class UTacticalControllerComponent;

/**
 * 
 */
UCLASS()
class SFAF_API ATacticalPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
	ATacticalPlayerController();
	
	virtual void BeginPlay() override;
	// -----------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTacticalControllerComponent> TacticalControllerComponent;
};
