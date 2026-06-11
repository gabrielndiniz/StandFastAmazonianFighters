// Copyright 2025 StandFast Games, LLC

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
	/** Default constructor */
	APlayerPawn();

protected:
	/** Initializes the pawn when the game starts */
	virtual void BeginPlay() override;

public:
	/** Updates the pawn every frame */
	virtual void Tick(float DeltaTime) override;
	/** Sets up player input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
