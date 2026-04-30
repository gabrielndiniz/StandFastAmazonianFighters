// � 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N�bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridType.h"
#include "Grid/GridTacticalTypes.h"

// Sets default values
AGridType::AGridType()
{
	// Disable Tick for performance since it's not being used
	PrimaryActorTick.bCanEverTick = false;

	GridDataComponent = CreateDefaultSubobject<UGridDataComponent>(TEXT("GridDataComponent"));
	
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

void AGridType::InitializeCollision() const
{
	GridMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Block);
	TacticalObstacleMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalDoubleCostMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalTripleCostMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	TacticalFlyingOnlyMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
}

// Called when the game starts or when spawned
void AGridType::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCollision();
}

// Called every frame (disabled in constructor for performance)
void AGridType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// No per-frame updates needed
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

void AGridType::AddInstanceMesh(int TileType, FTransform Transform) const
{
	// Add instance based on tile type or tactical channel
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
		// Check if it's an EGridTacticalChannel value
		switch (static_cast<EGridTacticalChannel>(TileType))
		{
		case EGridTacticalChannel::Obstacle:
			if (TacticalObstacleMesh) TacticalObstacleMesh->AddInstance(Transform, true);
			break;
		case EGridTacticalChannel::DoubleCost:
			if (TacticalDoubleCostMesh) TacticalDoubleCostMesh->AddInstance(Transform, true);
			break;
		case EGridTacticalChannel::TripleCost:
			if (TacticalTripleCostMesh) TacticalTripleCostMesh->AddInstance(Transform, true);
			break;
		case EGridTacticalChannel::FlyingOnly:
			if (TacticalFlyingOnlyMesh) TacticalFlyingOnlyMesh->AddInstance(Transform, true);
			break;
		default:
			if (GridMesh)
				GridMesh->AddInstance(Transform, true);
			UE_LOG(LogTemp, Warning, TEXT("AddInstanceMesh: Unknown TileType %d"), TileType);
			break;
		}
		break;
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
