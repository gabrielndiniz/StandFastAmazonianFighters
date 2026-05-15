// � 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N�bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

/**
 * AAIPawn - AI controlled pawn for tactical gameplay
 * Handles spell casting, location management, and impact calculations
 */
// ---------------------------------------------------------------------------
// Actor
// ---------------------------------------------------------------------------

UCLASS()
class SFAF_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIPawn();

protected:
	virtual void BeginPlay() override;

	// -----------------------------------------------------------------------
	// Internal State
	// -----------------------------------------------------------------------

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

	// -----------------------------------------------------------------------
	// Calculation Weights
	// -----------------------------------------------------------------------

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
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------------------------------------------------------
	// Spell & Location API
	// -----------------------------------------------------------------------

	/** Sets the locations in range and number of spells */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetLocationsAndSpells(const TArray<FIntPoint>& Points, int32 Spell);

	/** Resets all spell loop counters and indices to zero */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartSpellLoop();

	/** Iterates through locations and spells */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntVector LocationAndSpellLoop();

	/** Returns current spell loop counters for debugging */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	FIntPoint DebugSpellCounter() const;

	/** Sets the potential cast locations */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetCastLocations(const TArray<FIntPoint>& Points);

	/** Resets all cast location loop counters and indices to zero */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartCastLocationLoop();

	/** Iterates through cast locations */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntPoint CastLocationLoop();

	/** Returns current cast location loop counter for debugging */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	int32 DebugLocationCounter() const;

	// -----------------------------------------------------------------------
	// Impact API
	// -----------------------------------------------------------------------

	/** Calculates the impact score of a spell based on various parameters */
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

	/** Calculates the middle value between X and Y of an FIntPoint */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static float CalcMiddle(FIntPoint Value);

	/** Checks if a unit can be killed based on its HP and potential damage */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static bool CanUnitBeKilled(int32 HP, FIntPoint Damage);
};
