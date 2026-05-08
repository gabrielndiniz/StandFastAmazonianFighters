// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionBase.generated.h"

UCLASS()
class SFAF_API AActionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	bool GetReady() const {return bReady; }
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetReady(bool bIsReady) { bReady = bIsReady;};
	
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bReady = false;
	
	
	
};
