// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Core/Combat/Tactical/TacticalManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Grid/GridType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATacticalManager::ATacticalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	
	//Scene Components construct
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);	
	
	HoverMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("HoverMesh"));
	HoverMesh->SetupAttachment(SceneRoot);
	
	SelectMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMesh"));
	SelectMesh->SetupAttachment(SceneRoot);
	
	
	//Action Components construct
	
	HoverTile = CreateDefaultSubobject<UHoverTile>(TEXT("HoverTile"));
	
}

// Called when the game starts or when spawned
void ATacticalManager::BeginPlay()
{
	Super::BeginPlay();
	
	Initiate();
	
}

// Called every frame
void ATacticalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    ExecuteHoveredTile();

	
}

void ATacticalManager::Initiate()
{
	SetGrid(Cast<AGridType>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridType::StaticClass())));
		
	
	if (HoverTile && Grid && HoverMesh)
	{
		HoverTile->SetGrid(Grid);
		HoverTile->SetReady(true);
	}
}

void ATacticalManager::SetGrid(AGridType* GridType)
{
	Grid = GridType;
}

AGridType* ATacticalManager::GetGrid()
{
	return Grid;
}

void ATacticalManager::SetDebugController(ATacticalPlayerController* Controller)
{
	
	if (Controller)
	{
		bDebug = true;
		
		DebugController.PlayerController = Controller;
		DebugController.ControllerComponent = Controller->TacticalControllerComponent;
		DebugController.bIsPlayer = true;
	}
	else
	{
		bDebug = false;		
		DebugController.PlayerController = nullptr;
		DebugController.ControllerComponent = nullptr;
	}
}

bool ATacticalManager::GetCurrentTeam(FControllers& OutControllers) const
{
	if (bDebug)
	{
		OutControllers = DebugController;
		return true;
	}
	if (const FControllers* Found = TeamsControllers.Find(CurrentTeam))
	{
		OutControllers = *Found;
		return true;
	}
	
	return false;
}

TArray<FControllers> ATacticalManager::GetAllTeams() const
{
	TArray<FControllers> Result;
	
	for (const TPair<int32, FControllers>& Pair : TeamsControllers)
	{
		if (Pair.Value.PlayerController || Pair.Value.AIController)
		{
			Result.Add(Pair.Value);
		}
	}
	
	return Result;
}


void ATacticalManager::ExecuteHoveredTile()
{
	if (!HoverTile)
	{
		UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No HoverTile found."))
		return;
	}
	bool bChange;
	if (bDebug)
	{
		if (!DebugController.PlayerController)
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No DebugController found."))
			return;
		}
		bChange = HoverTile->Execute(
			DebugController.ControllerComponent->GetHoveredTile(),
			true,
			DebugController.ControllerComponent->GetHoveredTile());
	}
	else
	{
		if (TeamsControllers.IsEmpty())
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: TeamsControllers is empty."))
			return;
		}
		bChange = HoverTile->Execute(
			TeamsControllers.Find(CurrentTeam)->ControllerComponent->GetHoveredTile(),
			true,
			TeamsControllers.Find(CurrentTeam)->ControllerComponent->GetHoveredTile());
	}
	FGridTileStaticData* HoveredData = HoverTile->GetHoveredTileData();
	FTransform Transform = FTransform::Identity;
	if (bChange)
	{
		HoverMesh->ClearInstances();
		Transform.SetLocation(HoveredData->WorldLocation);
		HoverMesh->AddInstance(Transform,true);
	}

}
