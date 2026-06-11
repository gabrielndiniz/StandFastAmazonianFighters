// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionBase.generated.h"

/**
 * Base actor class for action-related gameplay elements.
 * Provides a simple ready-state mechanism for controlling execution flow.
 */
UCLASS()
class SFAF_API AActionBase : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor */
	AActionBase();

protected:
	/** Initializes the action when the game starts */
	virtual void BeginPlay() override;

public:
	/** Updates the action every frame */
	virtual void Tick(float DeltaTime) override;
	
	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns whether this action is ready for execution */
	UFUNCTION(BlueprintCallable, Category="Action")
	bool GetReady() const {return bReady; }
	
	/** Sets the ready state of this action */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetReady(bool bIsReady) { bReady = bIsReady;};
	
protected:
	// -----------------------------------------------------------------------
	// State
	// -----------------------------------------------------------------------

	/** Whether this action is ready to be processed */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bReady = false;
};
