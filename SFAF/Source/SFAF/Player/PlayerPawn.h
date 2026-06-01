// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

/** Player-controlled pawn for navigating and interacting with the tactical grid */
UCLASS()
class SFAF_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Sets default values for this pawn's properties */
	APlayerPawn();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
