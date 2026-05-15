// � 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N�bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "AI/AIPawn.h"
#include "Logging/LogMacros.h"

// Define log category for this class
DEFINE_LOG_CATEGORY_STATIC(LogAIPawn, Log, All);

// Sets default values
AAIPawn::AAIPawn()
{
	// Disable Tick for performance since it's not being used for anything substantial
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	// No additional initialization needed
}

// Called every frame (disabled in constructor for performance)
void AAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// No per-frame updates needed
}

// Called to bind functionality to input
void AAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// No input bindings needed for AI pawn
}

void AAIPawn::SetLocationsAndSpells(const TArray<FIntPoint>& Points, int32 Spell)
{
	LocationsInRange = Points;
	SpellLength = Spell;
}

void AAIPawn::RestartSpellLoop()
{
	SpellLoopCounter = 0;
	SpellArrayIndex = 0;
	LocationsInRangeLoopCounter = 0;
	LocationsInRangeArrayIndex = 0;
}

FIntVector AAIPawn::LocationAndSpellLoop()
{
	// Increment counters
	LocationsInRangeLoopCounter++;
	LocationsInRangeArrayIndex++;

	// Check if locations array is empty
	if (LocationsInRange.IsEmpty())
	{
		UE_LOG(LogAIPawn, Verbose, TEXT("LocationAndSpellLoop: Locations array is empty"));
		return FIntVector(-1, -1, -1);
	}

	// Initialize counter if this is the first iteration
	if (SpellLoopCounter == 0)
	{
		SpellLoopCounter = 1;
	}

	// Check if we've reached the end of the locations array
	if (LocationsInRangeArrayIndex > LocationsInRange.Num())
	{
		// Move to the next spell
		SpellArrayIndex++;
		SpellLoopCounter++;
		LocationsInRangeArrayIndex = 1;
	}

	// Check if we've processed all spells
	if (SpellArrayIndex >= SpellLength)
	{
		UE_LOG(LogAIPawn, Verbose, TEXT("LocationAndSpellLoop: All spells processed, restarting loop"));
		RestartSpellLoop();
		return FIntVector(-999, -999, -999);
	}

	// Validate array index before access
	if (!LocationsInRange.IsValidIndex(LocationsInRangeArrayIndex - 1))
	{
		UE_LOG(LogAIPawn, Warning, TEXT("LocationAndSpellLoop: Invalid index %d for array of size %d"),
			LocationsInRangeArrayIndex - 1, LocationsInRange.Num());
		return FIntVector(-1, -1, -1);
	}

	// Return current location and spell index
	const FIntPoint& CurrentLocation = LocationsInRange[LocationsInRangeArrayIndex - 1];
	return FIntVector(CurrentLocation.X, CurrentLocation.Y, SpellArrayIndex);
}

FIntPoint AAIPawn::DebugSpellCounter() const
{
	return FIntPoint(SpellLoopCounter, LocationsInRangeLoopCounter);
}

void AAIPawn::SetCastLocations(const TArray<FIntPoint>& Points)
{
	CastLocation = Points;
}

void AAIPawn::RestartCastLocationLoop()
{
	CastLocationLoopCounter = 0;
	CastLocationArrayIndex = 0;
}

FIntPoint AAIPawn::CastLocationLoop()
{
	// Increment counters
	CastLocationLoopCounter++;
	CastLocationArrayIndex++;

	// Check if cast locations array is empty
	if (CastLocation.IsEmpty())
	{
		UE_LOG(LogAIPawn, Verbose, TEXT("CastLocationLoop: Cast locations array is empty"));
		return FIntPoint(-1, -1);
	}

	// Check if we've reached the end of the cast locations array
	if (CastLocationArrayIndex > CastLocation.Num())
	{
		UE_LOG(LogAIPawn, Verbose, TEXT("CastLocationLoop: All cast locations processed, restarting loop"));
		RestartCastLocationLoop();
		return FIntPoint(-999, -999); // Fixed typo: was -999.-999
	}

	// Validate array index before access
	if (!CastLocation.IsValidIndex(CastLocationArrayIndex - 1))
	{
		UE_LOG(LogAIPawn, Warning, TEXT("CastLocationLoop: Invalid index %d for array of size %d"),
			CastLocationArrayIndex - 1, CastLocation.Num());
		return FIntPoint(-1, -1);
	}

	// Return current cast location
	return CastLocation[CastLocationArrayIndex - 1];
}

int32 AAIPawn::DebugLocationCounter() const
{
	return CastLocationLoopCounter;
}

float AAIPawn::CalculateSpellImpact(
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
	int32 ControlledUnitHP)
{
	// Early exit if spell hurts the caster (not allowed in this game)
	if (bHurtItself)
	{
		UE_LOG(LogAIPawn, Verbose, TEXT("CalculateSpellImpact: Spell hurts caster, returning minimum score"));
		return -9999999.0f;
	}

	/*
	 * Note: In this game it should not be possible to hurt itself
	 * But if it was possible:
	 * TotalHPCost = TotalHPCost + CalculateMiddleScore(HPDamage*2,true);
	 * Also need to check if it would kill itself with CanUnitBeKilled and give score to it
	 */

	 // Calculate risk and ally multipliers
	const float TargetRisk = bLowRisk ? 1.0f : 0.5f;
	const float AllyMultiplier = bIsUnityAlly ? 1.0f : -1.0f;

	// Calculate damage scores with proper operator precedence
	// HP score includes kill value if damage can kill the unit
	const float HPDamageValue = CalcMiddle(HPDamage);
	const bool CanKillTarget = CanUnitBeKilled(UnitHP, HPDamage);
	const float KillBonus = CanKillTarget ? KillValue : 0.0f;
	const float TotalHPScore = (HPDamageValue * HPValue + KillBonus) * AllyMultiplier;

	// MP and AP scores
	const float TotalMPScore = CalcMiddle(MPDamage) * MPValue * AllyMultiplier;
	const float TotalAPScore = CalcMiddle(APDamage) * APValue * AllyMultiplier;

	// Fixed operator precedence issues in MaxCastScore and CooldownScore
	const float MaxCastScore = (MaxCastPerTurn < 999 ? CountdownValue : 0.0f) * AllyMultiplier;
	const float CooldownScore = (CooldownTurnCount > 0 ? CooldownValue : 0.0f) * AllyMultiplier;

	// Calculate costs
	const bool SpellKillsSelf = CanUnitBeKilled(ControlledUnitHP, FIntPoint(HPCost, HPCost));
	const float SelfKillPenalty = SpellKillsSelf ? KillValue : 0.0f;
	const float TotalHPCost = (HPCost + SelfKillPenalty) * HPValue / CostAdjust;
	const float TotalMPCost = MPCost * MPValue / CostAdjust;
	const float TotalAPCost = APCost * APValue / CostAdjust;

	// Calculate final result
	const float TotalBenefit = TotalHPScore + TotalMPScore + TotalAPScore - MaxCastScore - CooldownScore;
	const float TotalCost = TotalHPCost + TotalMPCost + TotalAPCost;
	const float Result = TotalBenefit * TargetRisk - TotalCost;

	UE_LOG(LogAIPawn, VeryVerbose, TEXT("CalculateSpellImpact: Benefit=%.2f, Cost=%.2f, Result=%.2f"),
		TotalBenefit * TargetRisk, TotalCost, Result);

	return Result;
}

float AAIPawn::CalcMiddle(FIntPoint Value)
{
	// Calculate the average of X and Y values
	return (static_cast<float>(Value.X) + static_cast<float>(Value.Y)) * 0.5f;
}

bool AAIPawn::CanUnitBeKilled(int32 HP, FIntPoint Damage)
{
	return HP <= Damage.Y;
}

