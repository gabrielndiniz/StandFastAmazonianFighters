// © 2026 Gabriel Nóbile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GridType.generated.h"

UCLASS()
class SFAF_API AGridType : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	float GridVerticalDistance = 1.f;

	float TraceRange = 1000.f;

	float TraceSphereRadius = 1.f;

	UInstancedStaticMeshComponent* GridMesh;

	UInstancedStaticMeshComponent* TacticalObstacleMesh;

	UInstancedStaticMeshComponent* TacticalDoubleCostMesh;

	UInstancedStaticMeshComponent* TacticalTripleCostMesh;

	UInstancedStaticMeshComponent* TacticalFlyingOnlyMesh;

public:

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetInstancedMeshes(UInstancedStaticMeshComponent* Grid, UInstancedStaticMeshComponent* Obstacle,
		UInstancedStaticMeshComponent* DoubleCost, UInstancedStaticMeshComponent* TripleCost,
		UInstancedStaticMeshComponent* FlyingOnly);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ClearInstancedMeshes();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddInstanceMesh(int TileType, FTransform Transform);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool RemoveInstanceMesh(int TileType, int Index);

	UFUNCTION(BlueprintCallable, Category = "Default")
	FHitResult HitTraceGround(FVector Location, TArray<AActor*> ActorsToIgnore);

	UFUNCTION(BlueprintPure, Category = "Default")
	float GetTraceSphereRange() const { return TraceSphereRadius; }

};

