// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

/**
 * AI-controlled pawn for tactical gameplay.
 * Handles spell casting iteration, location management, and impact calculations
 * for AI decision-making during combat.
 */
UCLASS()
class SFAF_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Default constructor */
	AAIPawn();

protected:
	/** Initializes the AI pawn when the game starts */
	virtual void BeginPlay() override;

	// -----------------------------------------------------------------------
	// Internal State
	// -----------------------------------------------------------------------

	/** Array of grid positions within spell range */
	TArray<FIntPoint> LocationsInRange;

	/** Total number of spells available */
	int32 SpellLength = 0;

	/** Iteration counter for the outer spell processing loop */
	int32 SpellLoopCounter = 0;

	/** Current index in the spell array */
	int32 SpellArrayIndex = 0;

	/** Iteration counter for the inner locations loop */
	int32 LocationsInRangeLoopCounter = 0;

	/** Current index in the locations array */
	int32 LocationsInRangeArrayIndex = 0;

	/** Array of valid cast target positions */
	TArray<FIntPoint> CastLocation;

	/** Iteration counter for the cast location loop */
	int32 CastLocationLoopCounter = 0;

	/** Current index in the cast location array */
	int32 CastLocationArrayIndex = 0;

	// -----------------------------------------------------------------------
	// Calculation Weights
	// -----------------------------------------------------------------------

	/** Weight multiplier for HP damage/healing in impact calculations */
	float HPValue = 1000.0f;

	/** Weight multiplier for MP damage/healing in impact calculations */
	float MPValue = 200.0f;

	/** Weight multiplier for AP damage/healing in impact calculations */
	float APValue = 200.0f;

	/** Score awarded for killing a unit in impact calculations */
	float KillValue = 2000.0f;

	/** Penalty weight for remaining cooldown turns */
	float CooldownValue = 10000.0f;

	/** Penalty weight for remaining countdown turns */
	float CountdownValue = 10000.0f;

	/** Divisor applied to cost values to normalize them against benefits */
	float CostAdjust = 100.0f;

public:
	/** Updates the AI pawn every frame */
	virtual void Tick(float DeltaTime) override;
	/** Sets up player input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// -----------------------------------------------------------------------
	// Spell & Location API
	// -----------------------------------------------------------------------

	/** Sets the array of positions in spell range and the number of spells */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetLocationsAndSpells(const TArray<FIntPoint>& Points, int32 Spell);

	/** Resets all spell and location loop counters to zero */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartSpellLoop();

	/** Advances the spell/location iterator and returns the current (Location.X, Location.Y, SpellIndex) */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntVector LocationAndSpellLoop();

	/** Returns the current spell and location loop counters for debugging */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	FIntPoint DebugSpellCounter() const;

	/** Sets the array of potential cast target positions */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void SetCastLocations(const TArray<FIntPoint>& Points);

	/** Resets all cast location loop counters to zero */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	void RestartCastLocationLoop();

	/** Advances the cast location iterator and returns the current position */
	UFUNCTION(BlueprintCallable, Category = "AI|Spells")
	FIntPoint CastLocationLoop();

	/** Returns the current cast location loop counter for debugging */
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	int32 DebugLocationCounter() const;

	// -----------------------------------------------------------------------
	// Impact API
	// -----------------------------------------------------------------------

	/**
	 * Calculates the impact score of a spell based on damage, costs, risks, and ally/enemy status.
	 * Higher scores indicate more beneficial actions for the AI to prioritize.
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

	/** Calculates the midpoint between X and Y of an FIntPoint */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static float CalcMiddle(FIntPoint Value);

	/** Returns true if the given HP can be killed by the given damage range (HP <= Damage.Y) */
	UFUNCTION(BlueprintPure, Category = "AI|Utility")
	static bool CanUnitBeKilled(int32 HP, FIntPoint Damage);
};
