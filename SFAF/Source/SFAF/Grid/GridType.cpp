// � 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N�bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridType.h"
#include "Grid/GridTacticalTypes.h"
#include "Grid/GridModifier.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGridType::AGridType()
{
	// Disable Tick for performance since it's not being used
	PrimaryActorTick.bCanEverTick = false;

	GridDataComponent = CreateDefaultSubobject<UGridDataComponent>(TEXT("GridDataComponent"));
	GridRuntimeStateComponent = CreateDefaultSubobject<UGridRuntimeStateComponent>(TEXT("GridRuntimeStateComponent"));
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	GridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMesh"));
	GridMesh->SetupAttachment(SceneRoot);

	TacticalObstacleMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TacticalObstacleMesh"));
	TacticalObstacleMesh->SetupAttachment(SceneRoot);

	TacticalDoubleCostMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TacticalDoubleCostMesh"));
	TacticalDoubleCostMesh->SetupAttachment(SceneRoot);

	TacticalTripleCostMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TacticalTripleCostMesh"));
	TacticalTripleCostMesh->SetupAttachment(SceneRoot);

	TacticalFlyingOnlyMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TacticalFlyingOnlyMesh"));
	TacticalFlyingOnlyMesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AGridType::BeginPlay()
{
	Super::BeginPlay();
	
	TacticalModifiersMeshes.Add(FGameplayTag::RequestGameplayTag("Grid.Type.Obstacle"), TacticalObstacleMesh);
	TacticalModifiersMeshes.Add(FGameplayTag::RequestGameplayTag("Grid.Cost.Double"), TacticalDoubleCostMesh);
	TacticalModifiersMeshes.Add(FGameplayTag::RequestGameplayTag("Grid.Cost.Triple"), TacticalTripleCostMesh);
	TacticalModifiersMeshes.Add(FGameplayTag::RequestGameplayTag("Grid.Type.FlyingOnly"), TacticalFlyingOnlyMesh);

	
	InitializeCollision();
	
	ShowTacticalGrid(false);
}

// Called every frame (disabled in constructor for performance)
void AGridType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// No per-frame updates needed
}

void AGridType::InitializeCollision() const
{
	GridMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Block);
	TacticalObstacleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalDoubleCostMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalTripleCostMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalFlyingOnlyMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
}

void AGridType::ClearInstancedMeshes() const
{
	// Check each mesh individually and clear if not null
	if (GridMesh)
	{
		GridMesh->ClearInstances();
	}

	if (TacticalObstacleMesh)
	{
		TacticalObstacleMesh->ClearInstances();
	}

	if (TacticalDoubleCostMesh)
	{
		TacticalDoubleCostMesh->ClearInstances();
	}

	if (TacticalTripleCostMesh)
	{
		TacticalTripleCostMesh->ClearInstances();
	}

	if (TacticalFlyingOnlyMesh)
	{
		TacticalFlyingOnlyMesh->ClearInstances();
	}
}

UInstancedStaticMeshComponent* AGridType::SelectTacticMeshWithTag(FGameplayTag GridModifierTag) const
{
	if (const TObjectPtr<UInstancedStaticMeshComponent>* FoundMesh = TacticalModifiersMeshes.Find(GridModifierTag))
	{
		if (*FoundMesh)
		{
			return FoundMesh->Get();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("SelectTacticMeshWithTag: No mesh found for tag %s"),
		*GridModifierTag.ToString());
	return nullptr;
}

void AGridType::AddInstanceMesh(const FGameplayTag& TileTypeTag, const FGameplayTagContainer& TileTags, const FTransform& Transform) const
{
	// Sempre adiciona o mesh base
	if (GridMesh)
	{
		GridMesh->AddInstance(Transform, true);
	}

	// Verifica cada tag da tile e adiciona overlay tático correspondente
	for (const FGameplayTag& Tag : TileTags)
	{
		if (UInstancedStaticMeshComponent* TacticMesh = SelectTacticMeshWithTag(Tag))
		{
			TacticMesh->AddInstance(Transform, true);
		}
	}
}

bool AGridType::RemoveInstanceMesh(int TileType, int Index) const
{
	// Validate index
	if (Index < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Invalid negative index %d"), Index);
		return false;
	}

	// Helper lambda to remove instance if valid
	auto RemoveIfValid = [Index](UInstancedStaticMeshComponent* Mesh, const FString& MeshName) -> bool
	{
		if (Mesh && Mesh->GetInstanceCount() > Index)
		{
			Mesh->RemoveInstance(Index);
			return true;
		}
		else if (!Mesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: %s is null"), *MeshName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for %s (count: %d)"),
				Index, *MeshName, Mesh->GetInstanceCount());
		}
		return false;
	};

	// Remove instance based on tile type
	switch (TileType)
	{
	case 1:
		return RemoveIfValid(GridMesh, TEXT("GridMesh"));
	case 2:
		return RemoveIfValid(TacticalObstacleMesh, TEXT("TacticalObstacleMesh"));
	case 3:
		return RemoveIfValid(TacticalDoubleCostMesh, TEXT("TacticalDoubleCostMesh"));
	case 4:
		return RemoveIfValid(TacticalTripleCostMesh, TEXT("TacticalTripleCostMesh"));
	case 5:
		return RemoveIfValid(TacticalFlyingOnlyMesh, TEXT("TacticalFlyingOnlyMesh"));
	default:
		// Check if it's an EGridTacticalChannel value
		switch (static_cast<EGridTacticalChannel>(TileType))
		{
		case EGridTacticalChannel::Obstacle:
			return RemoveIfValid(TacticalObstacleMesh, TEXT("TacticalObstacleMesh"));
		case EGridTacticalChannel::DoubleCost:
			return RemoveIfValid(TacticalDoubleCostMesh, TEXT("TacticalDoubleCostMesh"));
		case EGridTacticalChannel::TripleCost:
			return RemoveIfValid(TacticalTripleCostMesh, TEXT("TacticalTripleCostMesh"));
		case EGridTacticalChannel::FlyingOnly:
			return RemoveIfValid(TacticalFlyingOnlyMesh, TEXT("TacticalFlyingOnlyMesh"));
		default:
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Unknown TileType %d"), TileType);
			break;
		}
		break;
	}
	return false;
}

FHitResult AGridType::HitTraceGround(FVector Location, TArray<AActor*> ActorsToIgnore) const
{
	FHitResult Hit;
	FVector StartLocation = Location + FVector(0, 0, TraceRange);
	FVector EndLocation = Location - FVector(0, 0, TraceRange);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// Perform line trace to find ground
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel2, // line of sight or Grid
		TraceParams
	);

	// Check if we hit something valid
	AActor* HitActor = Hit.GetActor();
	if (Hit.bBlockingHit && HitActor && !ActorsToIgnore.Contains(HitActor))
	{
		return Hit;
	}

	return FHitResult();
}

void AGridType::GenerateGrid() //TODO fix this accordingly
{
	if (!GridDataComponent || !GridDataComponent->GetGridDataAsset())
	{
		UE_LOG(LogTemp, Error, TEXT("GenerateGrid: GridDataComponent or GridDataAsset is null!"));
		return;
	}

	ClearInstancedMeshes();

	FIntPoint TileCount = GridDataComponent->GetNumberOfTileCount();
	FVector TileSize = GridDataComponent->GetTileSize();
	FVector StartLoc = GridDataComponent->GetInitialSpawnLocation();
	bool bSpawnAround = GridDataComponent->GetSpawnAroundGivenLocation();

	// Calculate bounds for modifier search
	FVector GridHalfSize = FVector(TileCount.X * TileSize.X * 0.5f, TileCount.Y * TileSize.Y * 0.5f, 500.f);
	FVector GridCenter = StartLoc;
	if (!bSpawnAround)
	{
		GridCenter += FVector(GridHalfSize.X, GridHalfSize.Y, 0.f);
	}

	// 1. Find all modifiers in the area
	TArray<AActor*> OverlappingModifiers;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	UKismetSystemLibrary::BoxOverlapActors(
		GetWorld(),
		GridCenter,
		GridHalfSize,
		ObjectTypes,
		AGridModifier::StaticClass(),
		TArray<AActor*>(),
		OverlappingModifiers
	);

	// 2. Loop through tiles
	for (int32 x = 0; x < TileCount.X; ++x)
	{
		for (int32 y = 0; y < TileCount.Y; ++y)
		{
			FVector Offset;
			if (bSpawnAround)
			{
				Offset = FVector(
					(x - TileCount.X * 0.5f) * TileSize.X + TileSize.X * 0.5f,
					(y - TileCount.Y * 0.5f) * TileSize.Y + TileSize.Y * 0.5f,
					0.f
				);
			}
			else
			{
				Offset = FVector(x * TileSize.X, y * TileSize.Y, 0.f);
			}

			FVector TileTargetLoc = StartLoc + Offset;
			FHitResult Hit = HitTraceGround(TileTargetLoc, TArray<AActor*>());

			if (Hit.bBlockingHit)
			{
				FTransform TileTransform(Hit.ImpactPoint + FVector(0.f, 0.f, GridVerticalDistance));
				
				// Add base mesh
				//AddInstanceMesh(1, TileTransform);

				// 3. Check for modifiers on this tile
				for (AActor* ModActor : OverlappingModifiers)
				{
					if (AGridModifier* Modifier = Cast<AGridModifier>(ModActor))
					{
						if (Modifier->AffectsPosition(TileTransform.GetLocation()))
						{
							
						}
					}
				}
			}
		}
	}
}

void AGridType::ShowTacticalGrid(bool bShow)
{
	TacticalObstacleMesh->SetVisibility(bShow);
	TacticalDoubleCostMesh->SetVisibility(bShow);
	TacticalTripleCostMesh->SetVisibility(bShow);
	TacticalFlyingOnlyMesh->SetVisibility(bShow);
}

FIntPoint AGridType::GetFirstTile() const
{
	return FirstTile;
}

FIntPoint AGridType::GetLastTile() const
{
	return LastTile;
}

