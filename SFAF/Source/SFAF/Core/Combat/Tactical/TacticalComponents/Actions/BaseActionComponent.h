// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	
	/** Sets the grid for the action */
	UFUNCTION(BlueprintCallable, Category="Action")
	void SetGrid(AGridType* GridType) { Grid = GridType;};
	
	/** Execute the action */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool Execute(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);

	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord);
	
	/**Get the Data*/
	FGridTileStaticData GetTileData(bool bIsTarget);

	/** Returns both source and target tile data at once */
	UFUNCTION(BlueprintPure, Category = "Action")
	void GetTileData(FGridTileStaticData& OutSourceData, FGridTileStaticData& OutTargetData) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool SetLocationsForMeshes();

	virtual bool SetLocationsForMeshes_Implementation();
	
	/**Get the Locations for spawn Instanced Meshes*/
	UFUNCTION(BlueprintCallable, Category = "Action")
	TArray<FVector> GetLocationsForMeshes();
		
	/** Returns the source or target grid coordinate based on bIsTarget */
	UFUNCTION(BlueprintCallable, Category = "Action")
	const FGridCoord& GetCoord(bool bIsTarget) const { return bIsTarget ? TargetCoord : SourceCoord; }

protected:
	// -----------------------------------------------------------------------
	// State
	// -----------------------------------------------------------------------

	/** Flag indicating if the action is ready */
	UPROPERTY(BlueprintReadOnly, Category="Action")
	bool bReady = false;		
		
	/** Source grid coordinate for the action */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord SourceCoord;
	
	/** Target grid coordinate for the action */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord TargetCoord;
	
	/** Whether the action's hit condition was satisfied */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bActionHasHit = false;
	
	/** Static tile data for the source tile */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData SourceTileData;

	/** Static tile data for the target tile */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridTileStaticData TargetTileData;
	
	/** World-space positions where visual mesh instances are placed */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<FVector> LocationsForMeshes;
	
	// -----------------------------------------------------------------------
	// Dependencies
	// -----------------------------------------------------------------------

	/** Weak pointer to the grid actor this action operates on */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TObjectPtr<AGridType> Grid;

};
