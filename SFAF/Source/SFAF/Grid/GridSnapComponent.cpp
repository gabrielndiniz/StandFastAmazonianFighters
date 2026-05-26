// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridSnapComponent.h"
#include "Grid/GridType.h"
#include "Grid/GridMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGridSnapComponent::UGridSnapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGridSnapComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetGrid(Cast<AGridType>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridType::StaticClass())));
}

void UGridSnapComponent::SetGrid(AGridType* Grid)
{
	GridOriginActor = Grid;
}

AGridType* UGridSnapComponent::GetGrid() const
{
	return GridOriginActor;
}

FVector UGridSnapComponent::GetSnappedLocation(FVector WorldLocation) const
{
	const bool bHasGrid =
		GridOriginActor != nullptr;

	const FVector GridOrigin =
		bHasGrid
		? GridOriginActor->GetActorLocation()
		: FVector::ZeroVector;

	const FGridCoord Coord =
		UGridMathLibrary::HexWorldToOffsetCoord(
			WorldLocation,
			GridOrigin,
			TileSize
		);

	FVector Snapped =
		UGridMathLibrary::HexOffsetToWorld(
			Coord,
			GridOrigin,
			TileSize,
			0.f
		);

	Snapped.Z = WorldLocation.Z+ZAlteration;

	return Snapped;
}

