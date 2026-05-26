// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Controllers/ControllerComponents/TacticalControllerComponent.h"

// Sets default values for this component's properties
UTacticalControllerComponent::UTacticalControllerComponent()
{
	// Set this component to be initialized when the game starts and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTacticalControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTacticalControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTacticalControllerComponent::AddCoordToComponent(const FGridCoord Coord, FName Component,
                                                       bool bIsTarget) 
{
	if (bIsTarget)
	{
		InTargetOfEachComponent.Add(Component, Coord);
		return InTargetOfEachComponent.Contains(Component);
	}
	else
	{
		InSourceOfEachComponent.Add(Component, Coord);
		return InSourceOfEachComponent.Contains(Component);
	}
}

bool UTacticalControllerComponent::GetCoordToComponent(FGridCoord& Coord, const FName Component, const bool bIsTarget)
{
	if (!Component.IsValid()) return false;

	if (bIsTarget)
	{
		if (FGridCoord* FoundCoord = InTargetOfEachComponent.Find(Component))
		{
			Coord = *FoundCoord; 
			return true;
		}
	}
	else
	{
		if (FGridCoord* FoundCoord = InSourceOfEachComponent.Find(Component))
		{
			Coord = *FoundCoord; 
			return true;
		}
	}

	return false;
}


void UTacticalControllerComponent::SetTeamNumber(int32 NewTeamNumber, bool bAdd)
{
	if (bAdd)
	{
		TeamNumbers.Add(NewTeamNumber);
	}
	else
	{
		TeamNumbers.Remove(NewTeamNumber);
	}
}

void UTacticalControllerComponent::SetDebugMode(bool bSet)
{
	bDebugMode = bSet;
}

bool UTacticalControllerComponent::GetDebugMode() const
{
	return bDebugMode;
}

TArray<int32> UTacticalControllerComponent::GetTeamNumber()
{
	return TeamNumbers;
}

bool UTacticalControllerComponent::IsMyTurn()
{
	return bMyTurn;
}

