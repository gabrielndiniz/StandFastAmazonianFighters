// © 2026 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "GridType.h"

// Sets default values
AGridType::AGridType()
{
	// Disable Tick for performance since it's not being used
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGridType::BeginPlay()
{
	Super::BeginPlay();
	// No additional initialization needed
}

// Called every frame (disabled in constructor for performance)
void AGridType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// No per-frame updates needed
}

void AGridType::SetInstancedMeshes(UInstancedStaticMeshComponent* Grid, UInstancedStaticMeshComponent* Obstacle,
	UInstancedStaticMeshComponent* DoubleCost, UInstancedStaticMeshComponent* TripleCost,
	UInstancedStaticMeshComponent* FlyingOnly)
{
	// Validate and assign mesh components with logging for null pointers
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInstancedMeshes: Grid mesh is null"));
	}
	GridMesh = Grid;

	if (!Obstacle)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInstancedMeshes: Obstacle mesh is null"));
	}
	TacticalObstacleMesh = Obstacle;

	if (!DoubleCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInstancedMeshes: DoubleCost mesh is null"));
	}
	TacticalDoubleCostMesh = DoubleCost;

	if (!TripleCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInstancedMeshes: TripleCost mesh is null"));
	}
	TacticalTripleCostMesh = TripleCost;

	if (!FlyingOnly)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInstancedMeshes: FlyingOnly mesh is null"));
	}
	TacticalFlyingOnlyMesh = FlyingOnly;
}

void AGridType::ClearInstancedMeshes()
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

void AGridType::AddInstanceMesh(int TileType, FTransform Transform)
{
	// Add instance based on tile type
	switch (TileType)
	{
	case 1:
		if (GridMesh)
			GridMesh->AddInstance(Transform, true);
		break;
	case 2:
		if (TacticalObstacleMesh)
			TacticalObstacleMesh->AddInstance(Transform, true);
		break;
	case 3:
		if (TacticalDoubleCostMesh)
			TacticalDoubleCostMesh->AddInstance(Transform, true);
		break;
	case 4:
		if (TacticalTripleCostMesh)
			TacticalTripleCostMesh->AddInstance(Transform, true);
		break;
	case 5:
		if (TacticalFlyingOnlyMesh)
			TacticalFlyingOnlyMesh->AddInstance(Transform, true);
		break;
	default:
		// For unknown tile types, add to the base grid as fallback
		if (GridMesh)
			GridMesh->AddInstance(Transform, true);
		UE_LOG(LogTemp, Warning, TEXT("AddInstanceMesh: Unknown TileType %d"), TileType);
		break;
	}
}

bool AGridType::RemoveInstanceMesh(int TileType, int Index)
{
	// Validate index
	if (Index < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Invalid negative index %d"), Index);
		return false;
	}

	// Remove instance based on tile type
	switch (TileType)
	{
	case 1:
		if (GridMesh && GridMesh->GetInstanceCount() > Index)
		{
			GridMesh->RemoveInstance(Index);
			return true;
		}
		else if (!GridMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: GridMesh is null"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for GridMesh (count: %d)"),
				Index, GridMesh->GetInstanceCount());
		}
		break;
	case 2:
		if (TacticalObstacleMesh && TacticalObstacleMesh->GetInstanceCount() > Index)
		{
			TacticalObstacleMesh->RemoveInstance(Index);
			return true;
		}
		else if (!TacticalObstacleMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: TacticalObstacleMesh is null"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for TacticalObstacleMesh (count: %d)"),
				Index, TacticalObstacleMesh->GetInstanceCount());
		}
		break;
	case 3:
		if (TacticalDoubleCostMesh && TacticalDoubleCostMesh->GetInstanceCount() > Index)
		{
			TacticalDoubleCostMesh->RemoveInstance(Index);
			return true;
		}
		else if (!TacticalDoubleCostMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: TacticalDoubleCostMesh is null"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for TacticalDoubleCostMesh (count: %d)"),
				Index, TacticalDoubleCostMesh->GetInstanceCount());
		}
		break;
	case 4:
		if (TacticalTripleCostMesh && TacticalTripleCostMesh->GetInstanceCount() > Index)
		{
			TacticalTripleCostMesh->RemoveInstance(Index);
			return true;
		}
		else if (!TacticalTripleCostMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: TacticalTripleCostMesh is null"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for TacticalTripleCostMesh (count: %d)"),
				Index, TacticalTripleCostMesh->GetInstanceCount());
		}
		break;
	case 5:
		if (TacticalFlyingOnlyMesh && TacticalFlyingOnlyMesh->GetInstanceCount() > Index)
		{
			TacticalFlyingOnlyMesh->RemoveInstance(Index);
			return true;
		}
		else if (!TacticalFlyingOnlyMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: TacticalFlyingOnlyMesh is null"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Index %d out of bounds for TacticalFlyingOnlyMesh (count: %d)"),
				Index, TacticalFlyingOnlyMesh->GetInstanceCount());
		}
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("RemoveInstanceMesh: Unknown TileType %d"), TileType);
		break;
	}
	return false;
}

FHitResult AGridType::HitTraceGround(FVector Location, TArray<AActor*> ActorsToIgnore)
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
