// © 2024 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "GridType.h"

// Sets default values
AGridType::AGridType()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridType::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridType::SetInstancedMeshes(UInstancedStaticMeshComponent* Grid, UInstancedStaticMeshComponent* Obstacle,
	UInstancedStaticMeshComponent* DoubleCost, UInstancedStaticMeshComponent* TripleCost,
	UInstancedStaticMeshComponent* FlyingOnly)
{
	GridMesh=Grid;
	TacticalObstacleMesh=Obstacle;
	TacticalDoubleCostMesh=DoubleCost;
	TacticalTripleCostMesh=TripleCost;
	TacticalFlyingOnlyMesh=FlyingOnly;
}

void AGridType::ClearInstancedMeshes()
{
	if (GridMesh != nullptr || TacticalObstacleMesh != nullptr || TacticalDoubleCostMesh != nullptr ||
		TacticalTripleCostMesh != nullptr || TacticalFlyingOnlyMesh != nullptr) {return;}
	GridMesh->ClearInstances();
	TacticalObstacleMesh->ClearInstances();
	TacticalDoubleCostMesh->ClearInstances();
	TacticalTripleCostMesh->ClearInstances();
	TacticalFlyingOnlyMesh->ClearInstances();
}

void AGridType::AddInstanceMesh(int TileType, FTransform Transform)
{
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
		break;
	}
}

bool AGridType::RemoveInstanceMesh(int TileType, int Index)
{
	if (Index < 0)
		return false;

	switch (TileType)
	{
	case 1:
		if (GridMesh && GridMesh->GetInstanceCount() > Index)
		{
			GridMesh->RemoveInstance(Index);
			return true;
		}
		break;
	case 2:
		if (TacticalObstacleMesh && TacticalObstacleMesh->GetInstanceCount() > Index)
		{
			TacticalObstacleMesh->RemoveInstance(Index);
			return true;
		}
		break;
	case 3:
		if (TacticalDoubleCostMesh && TacticalDoubleCostMesh->GetInstanceCount() > Index)
		{
			TacticalDoubleCostMesh->RemoveInstance(Index);
			return true;
		}
		break;
	case 4:
		if (TacticalTripleCostMesh && TacticalTripleCostMesh->GetInstanceCount() > Index)
		{
			TacticalTripleCostMesh->RemoveInstance(Index);
			return true;
		}
		break;
	case 5:
		if (TacticalFlyingOnlyMesh && TacticalFlyingOnlyMesh->GetInstanceCount() > Index)
		{
			TacticalFlyingOnlyMesh->RemoveInstance(Index);
			return true;
		}
		break;
	default:
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

	GetWorld()->LineTraceSingleByChannel(
			Hit,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_GameTraceChannel7, //blockgrid
			TraceParams
		);

	AActor* HitActor = Hit.GetActor(); //Return null if you need ignore
	if (Hit.bBlockingHit && HitActor)
	{
		//return Hit; //you can test this logic
		return FHitResult();
	}
	
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_GameTraceChannel2, //line of sight or Grid
		TraceParams
	);

	// Double check
	HitActor = Hit.GetActor();//Return the correct grid if you need ignore
	if (Hit.bBlockingHit && HitActor && !ActorsToIgnore.Contains(HitActor))
	{
		return Hit;
	}

	return FHitResult();
}
