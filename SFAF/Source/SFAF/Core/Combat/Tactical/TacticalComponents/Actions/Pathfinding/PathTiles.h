// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "PathTiles.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UPathTiles : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	UPathTiles();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
	virtual bool SetLocationsForMeshes_Implementation() override;
	
	/** The path tiles coordinates */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	TArray<FGridCoord> PathTiles;

	/** Total movement cost of the computed path */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	int32 TotalCost;
	
	/** Returns the path tiles coordinates computed by this component */
	UFUNCTION(BlueprintCallable, Category="Action")
	TArray<FGridCoord> GetPathTiles();

	/** Returns the total movement cost of the computed path */
	UFUNCTION(BlueprintCallable, Category = "Action")
	int32 GetTotalCost() const;
};
