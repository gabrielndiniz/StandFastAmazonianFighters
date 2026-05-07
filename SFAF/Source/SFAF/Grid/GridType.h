// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridDataComponent.h"
#include "GridRuntimeStateComponent.h"
#include "GameFramework/Actor.h"
#include "GridTacticalTypes.h"
#include "GridType.generated.h"

UCLASS()
class SFAF_API AGridType : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridType();
	void InitializeCollision() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridDataComponent> GridDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridRuntimeStateComponent> GridRuntimeStateComponent;
	
private:

	float GridVerticalDistance = 1.f;

	float TraceRange = 1000.f;

	float TraceSphereRadius = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> GridMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalObstacleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalDoubleCostMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalTripleCostMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> TacticalFlyingOnlyMesh;
	
	//TODO: Mapped State to tile pathfind

public:

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ClearInstancedMeshes() const;

	UFUNCTION(BlueprintCallable, Category = "Default")
	void AddInstanceMesh(int TileType, FTransform Transform) const;

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool RemoveInstanceMesh(int TileType, int Index) const;

	UFUNCTION(BlueprintCallable, Category = "Default")
	FHitResult HitTraceGround(FVector Location, TArray<AActor*> ActorsToIgnore) const;

	UFUNCTION(BlueprintPure, Category = "Default")
	float GetTraceSphereRange() const { return TraceSphereRadius; }

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void GenerateGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void ShowTacticalGrid(bool bShow);
};
