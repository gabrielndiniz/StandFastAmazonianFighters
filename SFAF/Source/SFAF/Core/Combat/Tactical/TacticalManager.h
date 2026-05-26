// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/TacticalAIController.h"
#include "Controllers/TacticalPlayerController.h"
#include "Controllers/ControllerComponents/TacticalControllerComponent.h"
#include "GameFramework/Actor.h"
#include "TacticalComponents/Actions/BaseActionComponent.h"
#include "TacticalComponents/Actions/Interface/HoverTile.h"
#include "TacticalComponents/Actions/Interface/SelectTile.h"
#include "TacticalComponents/Actions/Interface/TargetTile.h"
#include "TacticalManager.generated.h"

class AGridType;
/** Team Struct*/
USTRUCT(BlueprintType)
struct FControllers
{
	GENERATED_BODY()
	
	bool bIsPlayer = false;
	
	ATacticalPlayerController* PlayerController = nullptr;
	
	ATacticalAIController* AIController = nullptr;
	
	UTacticalControllerComponent* ControllerComponent = nullptr;
};


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
	
	// -----------------------------------------------------------------------
	// Meshes
	// -----------------------------------------------------------------------
	
	/** Visual representation of the Hovered Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> HoverMesh;
	
	/** Visual representation of the selected Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> SelectMesh;
		
	/** Visual representation of the selected Tile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	TObjectPtr<UInstancedStaticMeshComponent> TargetMesh;
	
	// -----------------------------------------------------------------------
	// Runtime
	// -----------------------------------------------------------------------
	
	void Initiate();
	
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	void SetGrid(AGridType* GridType);
	
	UFUNCTION(BlueprintCallable, Category = "Runtime")
	AGridType* GetGrid();
	
	UPROPERTY(BlueprintReadOnly, Category="Runtime")
	AGridType* Grid;
		
	// -----------------------------------------------------------------------
	// Actions - Components
	// -----------------------------------------------------------------------
	
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SetDebugController (ATacticalPlayerController* Controller);
		
	UPROPERTY()
	TMap<int32,FControllers> TeamsControllers;
		
	UPROPERTY()
	FControllers DebugController;
	
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	bool bDebug = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UHoverTile* HoverTile;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USelectTile* SelectTile;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UTargetTile* TargetTile;
	
	UPROPERTY()
	TMap<UBaseActionComponent*, UInstancedStaticMeshComponent*> ComponentMesh;
	
	// -----------------------------------------------------------------------
	// Actions - Functions
	// -----------------------------------------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void ExecuteAction(UBaseActionComponent* ActionComponent);	
		
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool GetCurrentTeam(FControllers& OutControllers) const;
	TArray<FControllers> GetAllTeams() const;

	/** Current Team*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	int32 CurrentTeam = 1;
};
