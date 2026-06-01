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
	
	SelectMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SelectMesh"));
	SelectMesh->SetupAttachment(SceneRoot);
	
	TargetMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(SceneRoot);
	
	NeighborMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("NeighborMesh"));
	NeighborMesh->SetupAttachment(SceneRoot);
	
	ReachableMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ReachableMesh"));
	ReachableMesh->SetupAttachment(SceneRoot);
	
	//Action Components construct
	
	HoverTile = CreateDefaultSubobject<UHoverTile>(TEXT("HoverTile"));
	
	SelectTile = CreateDefaultSubobject<USelectTile>(TEXT("SelectTile"));
	
	TargetTile = CreateDefaultSubobject<UTargetTile>(TEXT("TargetTile"));
	
	NeighborTile = CreateDefaultSubobject<UNeighborTile>(TEXT("NeighborTile"));
	
	ReachableTiles = CreateDefaultSubobject<UReachableTiles>(TEXT("ReachableTiles"));
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

    ExecuteAction(HoverTile);

	
}

void ATacticalManager::Initiate()
{
	SetGrid(Cast<AGridType>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridType::StaticClass())));
			
	if (HoverTile && Grid && HoverMesh)
	{
		HoverTile->SetGrid(Grid);
		HoverTile->SetReady(true);
		ComponentMesh.Add(HoverTile,HoverMesh);
	}	
	if (SelectTile && Grid && SelectMesh)
	{
		SelectTile->SetGrid(Grid);
		SelectTile->SetReady(true);
		ComponentMesh.Add(SelectTile,SelectMesh);
	}
	if (TargetTile && Grid && TargetMesh)
	{
		TargetTile->SetGrid(Grid);
		TargetTile->SetReady(true);
		ComponentMesh.Add(TargetTile,TargetMesh);
		if (NeighborTile && Grid && NeighborMesh)
		{
			NeighborTile->SetGrid(Grid);
			NeighborTile->SetReady(true);
			ComponentMesh.Add(NeighborTile,NeighborMesh);
			SequencedActions.Add(SelectTile,NeighborTile);
		}
	}
	if (ReachableTiles && Grid && ReachableMesh)
	{
		ReachableTiles->SetGrid(Grid);
		ReachableTiles->SetReady(true);
		ComponentMesh.Add(ReachableTiles, ReachableMesh);
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

void ATacticalManager::SetCurrentMovementPoints(int32 Points)
{
	ReachableTiles->SetCurrentMovementPoints(Points);
}


void ATacticalManager::ExecuteAction(UBaseActionComponent* ActionComponent)
{
	if (!ActionComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No %s found."),  *GetNameSafe(ActionComponent))
		return;
	}
	bool bChange;
	FGridCoord SourceCoord;
	FGridCoord TargetCoord;
	if (bDebug)
	{
		if (!DebugController.PlayerController)
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No DebugController found."))
			return;
		}
		if (!DebugController.ControllerComponent->GetCoordToComponent(SourceCoord, 
			ActionComponent->GetFName(), false)
			|| !DebugController.ControllerComponent->GetCoordToComponent(TargetCoord, 
			ActionComponent->GetFName(), true))
		{
			//UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No Coords found."))
			return;
		}
		bChange = ActionComponent->Execute(
			SourceCoord,
			true,
			TargetCoord);
	}
	else
	{
		if (TeamsControllers.IsEmpty())
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: TeamsControllers is empty."))
			return;
		}
		if (!TeamsControllers[CurrentTeam].PlayerController && !TeamsControllers[CurrentTeam].AIController)
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No TeamController found."))
			return;
		}
		if (!TeamsControllers[CurrentTeam].ControllerComponent->GetCoordToComponent(SourceCoord, 
			ActionComponent->GetFName(), false)
			|| !TeamsControllers[CurrentTeam].ControllerComponent->GetCoordToComponent(TargetCoord, 
			ActionComponent->GetFName(), true))
		{
			UE_LOG(LogTemp,Warning,TEXT("Tactical Manager: No Coords found considering %s."), 
				*GetNameSafe(ActionComponent))
			return;
		}
		bChange = ActionComponent->Execute(
			SourceCoord,
			true,
			TargetCoord);
	}
	FGridTileStaticData SourceData = ActionComponent->GetTileData(false);
	FGridTileStaticData TargetData = ActionComponent->GetTileData(true);
	if (bChange)
	{
		UInstancedStaticMeshComponent** MeshPtr  = ComponentMesh.Find(ActionComponent);
		if (UInstancedStaticMeshComponent* Mesh = *MeshPtr)
		{			
			Mesh->ClearInstances();
			for (const FVector& Location : ActionComponent->GetLocationsForMeshes())
			{
				FTransform Transform = FTransform::Identity;
				Transform.SetLocation(Location);
				Mesh->AddInstance(Transform,true);
			}					
		}
	}
	if (SequencedActions.Contains(ActionComponent))
	{
		ExecuteAction(*SequencedActions.Find(ActionComponent));
	}
	
}

void ATacticalManager::SetConsiderFlying(bool bConsider)
{
	NeighborTile->SetConsiderFly(bConsider);
	ReachableTiles->SetConsiderFly(bConsider);
}

bool ATacticalManager::CalculateReachableTiles(
	int32 InMovementPoints,
	bool bFlying,
	TArray<FGridCoord>& OutReachableTiles, TArray<FVector>& OutLocations)
{
	OutReachableTiles.Empty();
	OutLocations.Empty();

	if (!Grid || !Grid->GridRuntimeStateComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalManager::CalculateReachableTiles - Missing Grid or GridRuntimeStateComponent."));
		return false;
	}

	if (!SelectTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalManager::CalculateReachableTiles - Missing SelectTile."));
		return false;
	}

	//UGridRuntimeStateComponent* RuntimeState = Grid->GridRuntimeStateComponent;
	
	const FGridCoord& SourceCoord = SelectTile->GetCoord(false);
	
	ReachableTiles->SetConsiderFly(bFlying);
	
	ReachableTiles->SetCurrentMovementPoints(InMovementPoints);
	
	ReachableTiles->Execute(SourceCoord, true, SourceCoord);
			
	OutReachableTiles = ReachableTiles->GetReachableTiles();
		
	OutLocations = ReachableTiles->GetLocationsForMeshes();

	if (ReachableMesh)
	{
		ReachableMesh->ClearInstances();

		for (const FVector& Location : OutLocations)
		{
			FTransform Transform = FTransform::Identity;
			Transform.SetLocation(Location);
			ReachableMesh->AddInstance(Transform, true);
		}
	}

	return !OutReachableTiles.IsEmpty();
}
