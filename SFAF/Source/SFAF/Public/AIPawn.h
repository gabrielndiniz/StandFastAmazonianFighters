// © 2024 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

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

	//Loop variables
	TArray<FIntPoint> LocationsInRange;
	int SpellLenght = 0;
	int SpellLoopCounter = 0;
	int SpellArrayIndex = 0;
	int LocationsInRangeLoopCounter = 0;
	int LocationsInRangeArrayIndex = 0;

	TArray<FIntPoint> CastLocation;
	int CastLocationLoopCounter = 0;
	int CastLocationArrayIndex = 0;

	//Calculate Spell Cost Variables, each of them give weight for calculate the impact score, according to its origin.
	float HPValue = 1000;
	float MPValue = 200;
	float APValue = 200;
	float KillValue = 2000;
	float CooldownValue = 100000;
	float CountdownValue = 100000;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Loop Functions
	UFUNCTION(BlueprintCallable)
	void SetLocationsAndSpells (TArray<FIntPoint> Points, int Spell) {LocationsInRange = Points; SpellLenght = Spell;}

	void RestartSpellLoop() {SpellLoopCounter = 0; SpellArrayIndex = 0; LocationsInRangeLoopCounter = 0;
		LocationsInRangeArrayIndex = 0;}
	
	UFUNCTION(BlueprintCallable) //It will return the Location and the third value is the Spell Index
	FIntVector LocationAndSpellLoop ();

	UFUNCTION(BlueprintPure) //Debug Counters if needed
	FIntPoint DebugSpellCounter () const {return FIntPoint(SpellLoopCounter,LocationsInRangeLoopCounter);}

	UFUNCTION(BlueprintCallable)
	void SetCastLocations (TArray<FIntPoint> Points) {CastLocation = Points;}

	void RestartCastLocationLoop() {CastLocationLoopCounter = 0; CastLocationArrayIndex = 0;}

	UFUNCTION(BlueprintCallable) //It will return the Location In Loop
	FIntPoint CastLocationLoop();

	UFUNCTION(BlueprintPure) //Debug Counters if needed
	int DebugLocationCounter () const {return CastLocationLoopCounter;}


	//Calculate Spell Impact Functions
	UFUNCTION(BlueprintCallable)
	float CalculateSpellImpact	(bool bHurtItself, bool bLowRisk, FIntPoint HPDamage, FIntPoint MPDamage, FIntPoint
		                           APDamage, int APCost, int HPCost, int MPCost, int MaxCastPerTurn, int CooldownTurnCount, int UnitHP, bool bIsUnityAlly, int ControlledUnitHP);

	static float CalcMiddle (FIntPoint Value);

	static bool bUnitCanBeKilled(int HP, FIntPoint Damage)	{return HP <= Damage.Y;};
};
