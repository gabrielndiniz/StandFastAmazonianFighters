// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grid/GridCoord.h"
#include "TacticalControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UTacticalControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTacticalControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// -----------------------------------------------------------------------
	// Action related
	// -----------------------------------------------------------------------	
	
	/** Set the tile under the cursor or to be considered */
	UFUNCTION(BlueprintCallable, Category="Action", meta=(ToolTip="Set tile under cursor or to be considered"))
	FGridCoord GetHoveredTile();
	
	UFUNCTION(BlueprintCallable, Category="Action", meta=(ToolTip="Set tile under cursor or to be considered"))
	void SetHoveredTile(FGridCoord Tile);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Grid|State")
	FGridCoord HoveredTile;
	
	// -----------------------------------------------------------------------
	// Team
	// -----------------------------------------------------------------------	
	
	
	/** Set Team Number.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Set Team Number"))
	void SetTeamNumber(int32 NewTeamNumber, bool bAdd);
	
	/** Set debug mode.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Set DebugMode"))
	void SetDebugMode(bool bSet);
	
	/** Get debug mode.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Set DebugMode"))
	bool GetDebugMode() const;
	
	/** Return Team Number.*/
	UFUNCTION(BlueprintCallable, Category="Team", meta=(ToolTip="Get Team Numbers"))
	TArray<int32> GetTeamNumber();
	
	/** The Team Number of the owner.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	TArray<int32> TeamNumbers;
	
	/** This will be used for debug purpose*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	bool bDebugMode = false;
	
	/** This will be used for debug purpose*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	bool bMyTurn = false;
	
};
