// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"

// Sets default values for this component's properties
UBaseActionComponent::UBaseActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...	

}


// Called every frame
void UBaseActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBaseActionComponent::Execute_Implementation(
	const FGridCoord& InSourceCoord,
	bool bHasHit,
	const FGridCoord& InTargetCoord)
{
	// -----------------------------------------------------------------------
	// Validation
	// -----------------------------------------------------------------------

	if (!bReady)
	{
		return false;
	}


	// -----------------------------------------------------------------------
	// Retrieve Tile Data
	// -----------------------------------------------------------------------

	const FGridTileStaticData* SourceTile =
		Grid->GridRuntimeStateComponent->GetStaticTile(InSourceCoord);

	const FGridTileStaticData* TargetTile =
		Grid->GridRuntimeStateComponent->GetStaticTile(InTargetCoord);

	if (!SourceTile || !TargetTile)
	{
		return false;
	}

	// -----------------------------------------------------------------------
	// Cache Runtime Data
	// -----------------------------------------------------------------------

	SourceTileData = *SourceTile;
	TargetTileData = *TargetTile;

	SourceCoord = InSourceCoord;
	TargetCoord = InTargetCoord;

	bActionHasHit = bHasHit;

	return true;
}

