// © 2024 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "AIPawn.h"

// Sets default values
AAIPawn::AAIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FIntVector AAIPawn::LocationAndSpellLoop()
{
	LocationsInRangeLoopCounter++;
	LocationsInRangeArrayIndex++;

	if (LocationsInRange.IsEmpty()) {return FIntVector(-1,-1,-1);}

	if (SpellLoopCounter ==0) {SpellLoopCounter = 1;}

	if (LocationsInRangeArrayIndex > LocationsInRange.Num())
	{
		SpellArrayIndex = SpellArrayIndex+1;
		SpellLoopCounter = SpellLoopCounter+1;
		LocationsInRangeArrayIndex = 1;
	}
	
	if (SpellArrayIndex >= SpellLenght)
	{
		RestartSpellLoop();
		return FIntVector(-999,-999,-999);
	}
	
	return FIntVector(LocationsInRange[LocationsInRangeArrayIndex-1].X,LocationsInRange[LocationsInRangeArrayIndex-1].Y,SpellArrayIndex);
}

FIntPoint AAIPawn::CastLocationLoop()
{
	CastLocationLoopCounter++;
	CastLocationArrayIndex++; 

	if (CastLocation.IsEmpty()) {return FIntPoint(-1,-1);}

	if (CastLocationArrayIndex > CastLocation.Num())
	{
		RestartCastLocationLoop();
		return FIntPoint(-999.-999);
	}

	return CastLocation[CastLocationArrayIndex-1];
}

float AAIPawn::CalculateSpellImpact(bool bHurtItself, bool bLowRisk, FIntPoint HPDamage, FIntPoint MPDamage, FIntPoint APDamage,
                                    int APCost, int HPCost, int MPCost, int MaxCastPerTurn,
                                    int CooldownTurnCount, int UnitHP, bool bIsUnityAlly, int ControlledUnitHP)
{
	if (bHurtItself) {return -9999999.0f;}	
	/*In this game it should be not possible to hurt itself
	 * But if it was possible:
	 * TotalHPCost = TotalHPCost + CalculateMiddleScore(HPDamage*2,true);
	 * Also need to check if it would kill itself with bUnitCanBeKilled and give score to it
	 */
	const float TargetRisk = bLowRisk ? 0.5f : 1.f;
	const float AllyMultiplier = bIsUnityAlly ? -1.f : 1.f;
	int TotalHPScore = (CalcMiddle(HPDamage) * HPValue + bUnitCanBeKilled(UnitHP,HPDamage) ? KillValue : 0) * AllyMultiplier;
	int TotalMPScore = CalcMiddle(MPDamage) * MPValue * AllyMultiplier;
	int TotalAPScore = CalcMiddle(APDamage) * APValue * AllyMultiplier;
	int MaxCastScore = MaxCastPerTurn<999 ? CountdownValue : 0 * AllyMultiplier;
	int CooldownScore = CooldownTurnCount>0 ? CooldownValue : 0 * AllyMultiplier;
	int TotalHPCost = (HPCost + (bUnitCanBeKilled(ControlledUnitHP, FIntPoint(HPCost, HPCost)) ? KillValue : 0)) * HPValue;
	int TotalMPCost = MPCost * MPValue;
	int TotalAPCost = APCost * APValue;

	float Result = (TotalHPScore + TotalMPScore + TotalAPScore + MaxCastScore + CooldownScore) * TargetRisk - (TotalHPCost + TotalMPCost + TotalAPCost);
	return Result; //Piece by piece
}

float AAIPawn::CalcMiddle(FIntPoint Value)
{
	return (Value.X+Value.Y)*0.5;	
}

