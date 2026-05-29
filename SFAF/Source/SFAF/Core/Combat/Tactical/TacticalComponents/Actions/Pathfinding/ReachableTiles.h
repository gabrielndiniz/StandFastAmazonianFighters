// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "ReachableTiles.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UReachableTiles : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UReachableTiles();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetReachableTiles();
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetCurrentMovementPoints(int32 Points);
	
	UFUNCTION(BlueprintCallable, Category="Action")
	int32 GetCurrentMovementPoints();
		
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetConsiderFly(bool bConsider);
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> ReachableTiles;
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	int32 CurrentMovementPoints;	
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bConsiderFly = true;	
};
