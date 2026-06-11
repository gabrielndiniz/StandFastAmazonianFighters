// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

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

	// Create grid snapping logic component
	GridSnapComponent = CreateDefaultSubobject<UGridSnapComponent>(TEXT("GridSnapComponent"));
	
	// Create and set root collision volume
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);

	// Create preview mesh for editor visualization
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(Volume);

}

void AInstancedProps::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!GridSnapComponent)
	{
		if (!GridSnapComponent)
		{
			// Try to find the component
			GridSnapComponent = FindComponentByClass<UGridSnapComponent>();
		}
		if (!GridSnapComponent)
		{
			// Log error to help debug if not found even so
			UE_LOG(LogTemp, Error, TEXT("GridModifier %s: GridSnapComponent is null in OnConstruction!"), *GetName());
			return;
		}
	}

	if (!PreviewMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("GridModifier %s: PreviewMesh is null in OnConstruction!"), *GetName());
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
