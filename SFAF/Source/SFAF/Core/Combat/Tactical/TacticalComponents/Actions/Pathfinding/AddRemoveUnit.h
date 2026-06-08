// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Combatant/CombatantDatabaseDataAsset.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "AddRemoveUnit.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UAddRemoveUnit : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	
	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord) override;

	/** Spawn unit */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void AddUnit(FGridCoord Coord);
	
	/** Spawn unit */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void RemoveUnit(FGridCoord Coord);
	
	/** Change team of unit */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void ChangeUnitTeam(FGridCoord Coord);
	
	/** The unit to be summoned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	FGameplayTag CurrentUnit;

	/** Team of the unit */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int32 Team;	
	
	/** Choose Between add or remove the unit */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	bool bAdd = false;
	
	/** Change execution to change team */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	bool bChangeTeam = false;
	
	/** Change execution to change team */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	UCombatantDatabaseDataAsset* CombatantDatabase;
};
