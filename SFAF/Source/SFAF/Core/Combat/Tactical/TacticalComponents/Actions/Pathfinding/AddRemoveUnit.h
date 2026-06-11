// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combatant/CombatantDatabaseDataAsset.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "AddRemoveUnit.generated.h"

/**
 * Action component that spawns or removes combatant units on the grid.
 * Supports adding new units, removing existing ones, and changing unit teams.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UAddRemoveUnit : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * Executes the add or remove unit action on the target tile.
	 * @param InSourceCoord The source grid coordinate.
	 * @param bHasHit Whether a valid hit was detected.
	 * @param InTargetCoord The target grid coordinate.
	 * @return True if the unit was successfully added or removed.
	 */
	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord) override;

	/** Spawns a new unit at the given grid coordinate */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void AddUnit(FGridCoord Coord);
	
	/** Removes the unit occupying the given grid coordinate */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void RemoveUnit(FGridCoord Coord);
	
	/** Changes the team of the unit at the given grid coordinate */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void ChangeUnitTeam(FGridCoord Coord);
	
	/** The unit type tag identifying which combatant to spawn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	FGameplayTag CurrentUnit;

	/** Target team number for spawning or team-changing */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	int32 Team;	
	
	/** If true, adds a unit; if false, removes a unit */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	bool bAdd = false;
	
	/** If true, executes a team-change operation instead of add/remove */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	bool bChangeTeam = false;
	
	/** Reference to the combatant database for unit type lookups */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	UCombatantDatabaseDataAsset* CombatantDatabase;
};
