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
	
	/** Returns the list of reachable tile coordinates computed by this component */
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetReachableTiles();
	
	/** Sets the current movement points available for reachability calculations */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetCurrentMovementPoints(int32 Points);
	
	/** Returns the current movement points budget */
	UFUNCTION(BlueprintCallable, Category="Action")
	int32 GetCurrentMovementPoints();
		
	/** Sets whether flying movement is considered in reachability */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetConsiderFly(bool bConsider);
	
	/** Cached list of reachable tile coordinates */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> ReachableTiles;
	
	/** Current movement point budget for reachability calculations */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	int32 CurrentMovementPoints;	
	
	/** Whether to ignore ground-based movement restrictions */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bConsiderFly = true;	
};
