// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "NeighborTile.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UNeighborTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNeighborTile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetNeighbors();
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetConsiderFly(bool bConsider);
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> NeighborsCoords;
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bConsiderFly = true;	
};
