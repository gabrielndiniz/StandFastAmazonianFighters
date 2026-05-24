// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalManager.generated.h"

class AGridType;

UCLASS()
class SFAF_API ATacticalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacticalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> SceneRoot;
	
	
	/** Visual representation of the selected Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> SelectMesh;
	
	// -----------------------------------------------------------------------
	// Runtime
	// -----------------------------------------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	void SetGrid(AGridType* GridType);
	
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	AGridType* GetGrid();
	
	UPROPERTY(BlueprintReadOnly, Category="Runtime")
	AGridType* Grid;
	
	// -----------------------------------------------------------------------
	// Actions
	// -----------------------------------------------------------------------
	/** Current Team*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	int32 CurrentTeam = 1;
};
