// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridModifier.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridModifierTypes.h"


// Sets default values
AGridModifier::AGridModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	SetRootComponent(Volume);

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(Volume);
	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AGridModifier::SetPreviewVisible(bool bVisible)
{
	PreviewMesh->SetVisibility(bVisible);
}

void AGridModifier::ApplyToTile(FGameplayTag& InOutTileType, int32& InOutCost, FGameplayTagContainer& InOutTags) const
{
	// Apply cost
	if (CostMode == EGridModifierMode::Override)
	{
		InOutCost = CostValue;
	}
	else // Additive
	{
		InOutCost += CostValue;
	}

	// Apply tile type override (if valid)
	if (TileModificationTag.IsValid())
	{
		InOutTileType = TileModificationTag;
	}

	// Apply additional tags
	if (!TileTags.IsEmpty())
	{
		InOutTags.AppendTags(TileTags);
	}

	// Note: TacticalType handling is currently done in GridType.cpp
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
	if (!Volume) return false;

	FVector RelativePos = GetTransform().InverseTransformPosition(WorldPos);
	FVector BoxExtent = Volume->GetUnscaledBoxExtent();

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

