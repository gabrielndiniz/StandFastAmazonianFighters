// ? 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N?bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#include "Grid/InstancedProps.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Grid/GridMathLibrary.h"
#include "Grid/GridSnapComponent.h"
#include "Grid/GridType.h"

AInstancedProps::AInstancedProps()
{
	// Disable ticking for performance since this is primarily a static prop
	PrimaryActorTick.bCanEverTick = false;

	// Create and set root collision volume
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);

	// Create preview mesh for editor visualization
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(Volume);

	// Create grid snapping logic component
	GridSnapComponent = CreateDefaultSubobject<UGridSnapComponent>(TEXT("GridSnapComponent"));
}

void AInstancedProps::SetTopColor() const
{
	UMaterialInstanceDynamic* DynamicMaterial =
		PreviewMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(
			TEXT("GreenSubstitute"),
			TopColor
		);
	}
}

void AInstancedProps::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!GridSnapComponent || !PreviewMesh)
	{
		return;
	}

	// Link the grid origin actor to the snap component
	GridSnapComponent->GridOriginActor = Cast<AGridType>(GridOriginActor);

	// Calculate snapped location and offset the preview mesh accordingly
	const FVector Snapped =	GridSnapComponent->GetSnappedLocation(GetActorLocation());
	PreviewMesh->SetRelativeLocation(Snapped - GetActorLocation());
	
	SetTopColor();
}

void AInstancedProps::BeginPlay()
{
	Super::BeginPlay();
}

void AInstancedProps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
