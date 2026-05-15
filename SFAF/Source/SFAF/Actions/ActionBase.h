// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionBase.generated.h"

// ---------------------------------------------------------------------------
// Actor
// ---------------------------------------------------------------------------

UCLASS()
class SFAF_API AActionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns whether the action is ready to be executed */
	UFUNCTION(BlueprintCallable, Category="Action")
	bool GetReady() const {return bReady; }
	
	/** Sets the ready state of the action */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetReady(bool bIsReady) { bReady = bIsReady;};
	
protected:
	// -----------------------------------------------------------------------
	// State
	// -----------------------------------------------------------------------

	/** Flag indicating if the action is ready */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bReady = false;
};
