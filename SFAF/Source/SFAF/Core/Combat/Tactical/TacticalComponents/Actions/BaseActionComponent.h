// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Combat/Tactical/TacticalManager.h"
#include "Grid/GridType.h"
#include "BaseActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UBaseActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// -----------------------------------------------------------------------
	// API
	// -----------------------------------------------------------------------

	/** Returns whether the action is ready to be executed */
	UFUNCTION(BlueprintCallable, Category="Action")
	bool GetReady() const {return bReady; }
	
	/** Sets the ready state of the action */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetReady(bool bIsReady) { bReady = bIsReady;};
	
	/** Execute the action */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool Execute(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);

	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);
	
	
	
protected:
	// -----------------------------------------------------------------------
	// State
	// -----------------------------------------------------------------------

	/** Flag indicating if the action is ready */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bReady = false;		
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	ATacticalManager* TacticalManager;
	
	UPROPERTY(BlueprintReadOnly, Category="Action")
	AGridType* Grid;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord SourceCoord;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord TargetCoord;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bActionHasHit = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData SourceTileData;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData TargetTileData;

};
