// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridModifier.h"
#include "Grid/GridSnapComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridModifierTypes.h"
#include "GridType.h"


// Sets default values for this actor's properties
AGridModifier::AGridModifier()
{
	PrimaryActorTick.bCanEverTick = false;

	GridSnapComponent = CreateDefaultSubobject<UGridSnapComponent>(TEXT("GridSnapComponent"));
	if (GridSnapComponent)
	{
		GridSnapComponent->TileSize = FVector(400.f, 350.f, 50.f);
	}
	
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(Volume);
	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AGridModifier::OnConstruction(const FTransform& Transform)
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
}

void AGridModifier::SetPreviewVisible(bool bVisible)
{
	PreviewMesh->SetVisibility(bVisible);
}

void AGridModifier::ApplyToTile(FGameplayTag& InOutTileType, int32& InOutCost, FGameplayTagContainer& InOutTags) const
{
	// Apply cost modification
	if (CostMode == EGridModifierMode::Override)
	{
		InOutCost = CostValue;
	}
	else // Additive
	{
		InOutCost += CostValue;
	}

	// Apply tile type override if valid
	if (TileModificationTag.IsValid())
	{
		InOutTileType = TileModificationTag;
	}

	// Append additional tags
	if (!TileTags.IsEmpty())
	{
		InOutTags.AppendTags(TileTags);
	}

	// Note: Tactical visualization handling is managed by AGridType during generation
}

FGameplayTag AGridModifier::GetTileModificationTag() const
{
	return TileModificationTag;
}

FGameplayTagContainer AGridModifier::GetTileTags() const
{
	return TileTags;
}

bool AGridModifier::AffectsPosition(const FVector& WorldPos) const
{
	if (!Volume)
	{
		return false;
	}

	const FVector RelativePos = GetTransform().InverseTransformPosition(WorldPos);
	const FVector BoxExtent = Volume->GetUnscaledBoxExtent();

	return FMath::Abs(RelativePos.X) <= BoxExtent.X &&
		   FMath::Abs(RelativePos.Y) <= BoxExtent.Y &&
		   FMath::Abs(RelativePos.Z) <= BoxExtent.Z;
}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

