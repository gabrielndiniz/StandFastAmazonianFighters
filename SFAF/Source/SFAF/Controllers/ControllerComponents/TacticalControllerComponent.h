// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid/GridCoord.h"
#include "TacticalControllerComponent.generated.h"

/**
 * Component that bridges controllers (player or AI) with tactical action components.
 * Stores source and target grid coordinates keyed by action component name,
 * and manages team assignment and turn state for the associated controller.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UTacticalControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Default constructor */
	UTacticalControllerComponent();

protected:
	/** Initializes the component when the game starts */
	virtual void BeginPlay() override;

public:	
	/** Updates the component every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// -----------------------------------------------------------------------
	// Action related
	// -----------------------------------------------------------------------	
	
	/** Stores a source or target coordinate for a named action component */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool AddCoordToComponent(FGridCoord Coord, FName Component, bool bIsTarget) ;
		
	/** Retrieves the stored source or target coordinate for a named action component */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GetCoordToComponent(FGridCoord& Coord, FName Component, bool bIsTarget);

	/** Maps action component names to their source grid coordinates */
	UPROPERTY()
	TMap<FName, FGridCoord> InSourceOfEachComponent;
	
	/** Maps action component names to their target grid coordinates */
	UPROPERTY()
	TMap<FName, FGridCoord> InTargetOfEachComponent;

	// -----------------------------------------------------------------------
	// Team
	// -----------------------------------------------------------------------	
		
	/** Adds or removes a team number from this controller's team list */
	UFUNCTION(BlueprintCallable, Category="Team")
	void SetTeamNumber(int32 NewTeamNumber, bool bAdd);
	
	/** Enables or disables debug mode for this controller */
	UFUNCTION(BlueprintCallable, Category="Team")
	void SetDebugMode(bool bSet);
	
	/** Returns whether debug mode is active */
	UFUNCTION(BlueprintCallable, Category="Team")
	bool GetDebugMode() const;
	
	/** Returns the list of team numbers assigned to this controller */
	UFUNCTION(BlueprintCallable, Category="Team")
	TArray<int32> GetTeamNumber();
	
	/** Returns whether it is currently this controller's turn */
	UFUNCTION(BlueprintCallable, Category="Team")
	bool IsMyTurn();
	
	/** The team numbers assigned to the owner of this component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	TArray<int32> TeamNumbers;
	
	/** Enables debug logging and visualization bypasses for this controller */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	bool bDebugMode = false;
	
	/** Flag indicating whether it is currently this controller's turn to act */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	bool bMyTurn = false;
};
