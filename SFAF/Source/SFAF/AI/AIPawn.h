// © 2024 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

/**
 * AAIPawn - AI controlled pawn for tactical gameplay
 * Handles spell casting, location management, and impact calculations
 */
UCLASS()
class SFAF_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Loop variables for spell and location management
	/** Array of locations within range for spell casting */
	TArray<FIntPoint> LocationsInRange;
	/** Number of spells available */
	int32 SpellLength = 0;
	/** Counter for spell loop iterations */
	int32 SpellLoopCounter = 0;
	/** Current index in spell array */
	int32 SpellArrayIndex = 0;
	/** Counter for locations in range loop iterations */
	int32 LocationsInRangeLoopCounter = 0;
	/** Current index in locations array */
	int32 LocationsInRangeArrayIndex = 0;

	/** Array of potential cast locations */
	TArray<FIntPoint> CastLocation;
	/** Counter for cast location loop iterations */
	int32 CastLocationLoopCounter = 0;
	/** Current index in cast location array */
	int32 CastLocationArrayIndex = 0;

	// Spell impact calculation weights
	/** Weight value for HP in impact calculations */
	float HPValue = 1000.0f;
	/** Weight value for MP in impact calculations */
	float MPValue = 200.0f;
	/** Weight value for AP in impact calculations */
	float APValue = 200.0f;
	/** Weight value for kills in impact calculations */
	float KillValue = 2000.0f;
	/** Weight value for cooldown in impact calculations */
	float CooldownValue = 10000.0f;
	/** Weight value for countdown in impact calculations */
	float CountdownValue = 10000.0f;
	/** Weight value for paying cost in impact calculations */
	float CostAdjust = 100.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//------------------------------------------------------------------
	// Spell and Location Loop Functions
	//------------------------------------------------------------------

	/**
	 * Sets the locations in range and number of spells
	 * @param Points - Array of locations within range
	 * @param Spell - Number of spells available
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetLocationsAndSpells(const TArray<FIntPoint>& Points, int32 Spell)
	{
		LocationsInRange = Points;
		SpellLength = Spell;
	}

	/**
	 * Resets all spell loop counters and indices to zero
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartSpellLoop()
	{
		SpellLoopCounter = 0;
		SpellArrayIndex = 0;
		LocationsInRangeLoopCounter = 0;
		LocationsInRangeArrayIndex = 0;
	}

	/**
	 * Iterates through locations and spells
	 * @return FIntVector where X,Y are location coordinates and Z is the spell index
	 *         Returns (-1,-1,-1) if locations array is empty
	 *         Returns (-999,-999,-999) when iteration is complete
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntVector LocationAndSpellLoop();

	/**
	 * Returns current spell loop counters for debugging
	 * @return FIntPoint with X = SpellLoopCounter, Y = LocationsInRangeLoopCounter
	 */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	FIntPoint DebugSpellCounter() const
	{
		return FIntPoint(SpellLoopCounter, LocationsInRangeLoopCounter);
	}

	/**
	 * Sets the potential cast locations
	 * @param Points - Array of potential cast locations
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetCastLocations(const TArray<FIntPoint>& Points)
	{
		CastLocation = Points;
	}

	/**
	 * Resets all cast location loop counters and indices to zero
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartCastLocationLoop()
	{
		CastLocationLoopCounter = 0;
		CastLocationArrayIndex = 0;
	}

	/**
	 * Iterates through cast locations
	 * @return FIntPoint with the current cast location
	 *         Returns (-1,-1) if locations array is empty
	 *         Returns (-999,-999) when iteration is complete
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntPoint CastLocationLoop();

	/**
	 * Returns current cast location loop counter for debugging
	 * @return Current value of CastLocationLoopCounter
	 */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	int32 DebugLocationCounter() const
	{
		return CastLocationLoopCounter;
	}

	//------------------------------------------------------------------
	// Spell Impact Calculation Functions
	//------------------------------------------------------------------

	/**
	 * Calculates the impact score of a spell based on various parameters
	 * @param bHurtItself - Whether the spell can hurt the caster
	 * @param bLowRisk - Whether the spell is low risk
	 * @param HPDamage - Range of potential HP damage
	 * @param MPDamage - Range of potential MP damage
	 * @param APDamage - Range of potential AP damage
	 * @param APCost - AP cost to cast the spell
	 * @param HPCost - HP cost to cast the spell
	 * @param MPCost - MP cost to cast the spell
	 * @param MaxCastPerTurn - Maximum casts per turn
	 * @param CooldownTurnCount - Cooldown in turns
	 * @param UnitHP - Target unit's current HP
	 * @param bIsUnityAlly - Whether the target is an ally
	 * @param ControlledUnitHP - Caster's current HP
	 * @return Impact score (higher is better)
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	float CalculateSpellImpact(
		bool bHurtItself,
		bool bLowRisk,
		FIntPoint HPDamage,
		FIntPoint MPDamage,
		FIntPoint APDamage,
		int32 APCost,
		int32 HPCost,
		int32 MPCost,
		int32 MaxCastPerTurn,
		int32 CooldownTurnCount,
		int32 UnitHP,
		bool bIsUnityAlly,
		int32 ControlledUnitHP
	);

	/**
	 * Calculates the middle value between X and Y of an FIntPoint
	 * @param Value - The FIntPoint containing min (X) and max (Y) values
	 * @return The average of X and Y
	 */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static float CalcMiddle(FIntPoint Value);

	/**
	 * Checks if a unit can be killed based on its HP and potential damage
	 * @param HP - Current HP of the unit
	 * @param Damage - Potential damage range (using Y/max value for calculation)
	 * @return True if the unit can be killed, false otherwise
	 */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static bool CanUnitBeKilled(int32 HP, FIntPoint Damage)
	{
		return HP <= Damage.Y;
	}
};
