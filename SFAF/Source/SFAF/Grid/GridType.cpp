// � 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel N�bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "GridType.h"
#include "GridModifier.h"
#include "GridCoord.h"
#include "GridMathLibrary.h"
#include "GridTacticalTypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGridType::AGridType()
{
	// Disable Tick for performance since it's not being used
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	GridDataComponent = CreateDefaultSubobject<UGridDataComponent>(TEXT("GridDataComponent"));
	GridRuntimeStateComponent = CreateDefaultSubobject<UGridRuntimeStateComponent>(TEXT("GridRuntimeStateComponent"));
	GridPathFindingComponent = CreateDefaultSubobject<UGridPathfindingComponent>(TEXT("GridPathFindingComponent"));
	
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
	
	TacticalVisualTag = FGameplayTag::RequestGameplayTag(TEXT("Grid.Visual.Black"));
	
	StandardTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Cost.Simple")));
	StandardTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Visual.Green")));
	StandardTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Walkable")));
	StandardTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Size.Standard")));
	
	EnvironmentTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Cost.Simple")));
	EnvironmentTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Visual.Green")));
	EnvironmentTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Blocked")));
	EnvironmentTileTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Size.Standard")));
}


// Called when the game starts or when spawned
void AGridType::BeginPlay()
{
	Super::BeginPlay();
	
	GridRuntimeStateComponent->RegisterTacticalMesh(FGameplayTag::RequestGameplayTag("Grid.Type.Obstacle"), TacticalObstacleMesh);
	GridRuntimeStateComponent->RegisterTacticalMesh(FGameplayTag::RequestGameplayTag("Grid.Cost.Double"), TacticalDoubleCostMesh);
	GridRuntimeStateComponent->RegisterTacticalMesh(FGameplayTag::RequestGameplayTag("Grid.Cost.Triple"), TacticalTripleCostMesh);
	GridRuntimeStateComponent->RegisterTacticalMesh(FGameplayTag::RequestGameplayTag("Grid.Type.Blocked"), TacticalTripleCostMesh);
	GridRuntimeStateComponent->RegisterTacticalMesh(FGameplayTag::RequestGameplayTag("Grid.Type.FlyingOnly"), TacticalFlyingOnlyMesh);

	InitializeCollision();
	ShowTacticalGrid(false);
	
	GridLocation = GetActorLocation();
}

// Called every frame (disabled in constructor for performance)
void AGridType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	GridRuntimeStateComponent->ClearTacticalData();

	// Check the base mesh individually and clear if not null
	if (GridMesh)
	{
		GridMesh->ClearInstances();
	}
}

UInstancedStaticMeshComponent* AGridType::SelectTacticMeshWithTag(FGameplayTag GridModifierTag) const
{
	return GridRuntimeStateComponent->SelectTacticMeshWithTag(GridModifierTag);
}

bool AGridType::GetTileModifier(const FGridCoord& Coord, FGameplayTag& OutModifier) const
{
	return GridRuntimeStateComponent->GetTileModifier(Coord, OutModifier);
}

void AGridType::AddInstanceMesh(const FGameplayTagContainer& TileTags, const FTransform& Transform)
{
	if (GridMesh)
	{
		GridMesh->AddInstance(Transform, true);
	}

	for (const FGameplayTag& Tag : TileTags)
	{
		if (UInstancedStaticMeshComponent* TacticalMesh = SelectTacticMeshWithTag(Tag))
		{
			TacticalMesh->AddInstance(Transform, true);
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

bool AGridType::AddGridTileInstance(int32 TileIndex, const FTransform& TileTransform, FGridCoord TilePosition,
                                    bool bCheckForEquivalents, const FGameplayTagContainer& TileTags, 
                                    ACombatant_Base* UnitOnTile)
{	
	if (!CanAddTile(TileTags)) { return false; }
	
	// Register the last position
	GridRuntimeStateComponent->SetLastTile(TilePosition);
	
	// Only needed if it is possible to spawn a tile after play has started.
	if (bCheckForEquivalents)
	{
		GridRuntimeStateComponent->RemoveTile(TilePosition);
	}
	
	// Prepare tile data for registration
	FGridTileStaticData TileStaticData;
	TileStaticData.WorldLocation = TileTransform.GetLocation();
	TileStaticData.InstanceIndex = TileIndex;
	TileStaticData.TileTags = TileTags;

	FGridTileOccupancy Occupancy;
	Occupancy.OccupyingUnit = UnitOnTile;
	TileStaticData.Occupancy = Occupancy;
	
	AddInstanceMesh(TileTags, TileTransform);
	GridRuntimeStateComponent->RegisterTile(TilePosition, TileStaticData);
	
	return true;
	
}

bool AGridType::CanAddTile(const FGameplayTagContainer& TileTags)
{
	// A tile is only necessary if it can be walked upon or flown over
	if (TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.Walkable"))) ||
		TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.FlyingOnly"))))
	{
		return true;
	}
	
	return false;
}

bool AGridType::GenerateGrid(const FVector Location)
{
	if (!GridDataComponent || !GridDataComponent->GetGridDataAsset())
	{
		UE_LOG(LogTemp, Error, TEXT("GenerateGrid: GridDataComponent or GridDataAsset is null!"));
		return false;
	}
	
	
	// Destroy existing tiles before generating new ones
	DestroyGridTiles();
	
	GridDataComponent->SetInitialSpawnLocation(Location);
		
	// Applying visual data to the meshes
	
	// Set base grid mesh visual properties
	FGridVisualData VisualData = GridDataComponent->GetVisualData();
	
	GridMesh->SetStaticMesh(VisualData.Plane);
	GridMesh->SetMaterial(0, VisualData.PlaneBorderMaterialInstance);
	
	// Set tactical visualization properties
	FGridVisualData TacticalVisualData; 
	GridDataComponent->GetVisualDataByTag(TacticalVisualTag, TacticalVisualData);
	
	TacticalObstacleMesh->SetStaticMesh(TacticalVisualData.Solid);
	TacticalObstacleMesh->SetMaterial(0, TacticalVisualData.SolidWithMoldureMaterialInstance);
	
	TacticalDoubleCostMesh->SetStaticMesh(TacticalVisualData.Solid);
	TacticalDoubleCostMesh->SetMaterial(0, TacticalVisualData.PlaneMaterialInstance);
	
	TacticalFlyingOnlyMesh->SetStaticMesh(TacticalVisualData.Plane);
	TacticalFlyingOnlyMesh->SetMaterial(0, TacticalVisualData.PlaneBorderMaterialInstance);
	
	// Prepare for grid generation loop
	const FGridCoord TileCount = GridDataComponent->GetNumberOfTileCount();
	const FVector TileSize = GridDataComponent->GetTileSize();
	
	int32 TileIndex = 0;
	FTransform TileTransform = FTransform::Identity;
	FGridCoord TilePosition = FGridCoord(0, 0);
	bool bTileNeeded = false;
	float ZCorrection = 0.1f;
	
	// For Ground Tracing
	FGameplayTagContainer TileTags = StandardTileTags;
	FGameplayTag ModifierTag;
	bool bGridModifier;
	float ZScale = 1.f;
	bool bSpawnOnEnvironment = GridDataComponent->GetSpawnOnEnvironment();
		
	TMap<FGridCoord, AGridModifier*> ModifiersByCoord;
	TArray<AActor*> FoundModifiers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridModifier::StaticClass(), FoundModifiers);

	for (AActor* Actor : FoundModifiers)
	{
		if (AGridModifier* Modifier = Cast<AGridModifier>(Actor))
		{
			FGridCoord SnapCoord = Modifier->GetSnappedCoord();
			if (SnapCoord.X != -1)
			{
				ModifiersByCoord.Add(SnapCoord, Modifier);
			}
		}
	}
	
	// Loop through the grid dimensions
	for (int32 x = 0; x < TileCount.X - 1; ++x)
	{
		for (int32 y = 0; y < TileCount.Y - 1; ++y)
		{
			TilePosition = FGridCoord(x, y);
			
			FVector TileLocation;
			
			TileLocation = UGridMathLibrary::HexOffsetToWorld(TilePosition, GridLocation, TileSize, ZCorrection);


			// These variables are only used if we have to spawn grid after play has started.
			constexpr bool bCheckForEquivalents = false;
			ACombatant_Base* UnitOnTile = nullptr;

			
			
			// TODO: Grid centering logic improvement needed. 
			// Currently uses bSpawnAroundGivenLocation to decide tracing behavior.

			if (bSpawnOnEnvironment)
			{
				// Trace Ground for Z location and Grid Modifiers
				
				TileLocation = UGridMathLibrary::HexOffsetToWorld(
					TilePosition, GridLocation, TileSize, ZCorrection);

				TileTags = StandardTileTags;
				bGridModifier = false;
				

				// Lookup direto — sem trace
				if (AGridModifier** FoundModifier = ModifiersByCoord.Find(TilePosition))
				{
					AGridModifier* Modifier = *FoundModifier;
					TileTags = Modifier->GetTileTags();
					ModifierTag = Modifier->GetTileModificationTag();
            
					// Z vem da posição snappada do modificador
					TileLocation.Z = Modifier->GetSnappedWorldLocation().Z;
					bGridModifier = true;
				}

				TileTransform.SetLocation(TileLocation);
				bTileNeeded = AddGridTileInstance(TileIndex, TileTransform, TilePosition,
					false, TileTags, nullptr);

				if (bGridModifier && bTileNeeded)
				{
					GridRuntimeStateComponent->AddTacticalModifierPosition(TilePosition, ModifierTag);
					if (UInstancedStaticMeshComponent* ModMesh = SelectTacticMeshWithTag(ModifierTag))
					{
						ModMesh->AddInstance(TileTransform, true);
					}
				}

				if (bTileNeeded)
				{
					TileIndex++;
					if (GridRuntimeStateComponent->GetFirstTile().X == TNumericLimits<int32>::Max())
					{
						GridRuntimeStateComponent->SetFirstTile(TilePosition);
					}
					GridRuntimeStateComponent->SetLastTile(TilePosition);
				}
			}
			else
			{
				TileTransform.SetLocation(TileLocation);
				bTileNeeded = AddGridTileInstance(TileIndex, TileTransform, TilePosition, bCheckForEquivalents,
					TileTags, UnitOnTile);
				
				if (bTileNeeded)
				{
					TileIndex++;
				}
			}
			
			if (GridRuntimeStateComponent->GetFirstTile().X == TNumericLimits<int32>::Max() && bTileNeeded)
			{
				GridRuntimeStateComponent->SetFirstTile(TilePosition);
			}
		}
	}
	
	if (GridRuntimeStateComponent->GetFirstTile().X == TNumericLimits<int32>::Max())
	{
		return false;
	}
	
	// Grid generation complete
	bReady = true;
	return true;
}

void AGridType::ShowTacticalGrid(bool bShow)
{
	TacticalObstacleMesh->SetVisibility(bShow);
	TacticalDoubleCostMesh->SetVisibility(bShow);
	TacticalTripleCostMesh->SetVisibility(bShow);
	TacticalFlyingOnlyMesh->SetVisibility(bShow);
}

void AGridType::DestroyGridTiles()
{
	ClearInstancedMeshes();

	if (GridRuntimeStateComponent)
	{
		GridRuntimeStateComponent->ClearAllTiles();
	}
	if (GridPathFindingComponent)
	{
		GridPathFindingComponent->ClearPathfindingCache();
	}
}

FGridCoord AGridType::GetFirstTile() const
{
	return GridRuntimeStateComponent->GetFirstTile();
}

FGridCoord AGridType::GetLastTile() const
{
	return GridRuntimeStateComponent->GetLastTile();
}

bool AGridType::GetIsReady() const
{
	return bReady;
}

bool AGridType::FindNearestTileFromLocation(const FVector& WorldLocation, FGridCoord& OutCoord,
	FGridTileStaticData& OutTileData) const
{
	if (!GridRuntimeStateComponent)
	{
		return false;
	}

	return UGridMathLibrary::FindNearestTileFromWorldPosition(
		WorldLocation,
		GridRuntimeStateComponent->StaticTiles,
		OutCoord,
		OutTileData
	);
}

FGridTileStaticData* AGridType::GetTileStaticData(FGridCoord Coord)
{
	if (!GridRuntimeStateComponent)
	{
		FGridTileStaticData* Result = nullptr;
		return Result;
	}
	return GridRuntimeStateComponent->GetMutableStaticTile(Coord);
}

int32 AGridType::GetTileCost(FGridCoord Coord)
{
	if (!GridRuntimeStateComponent)
	{
		return 999;
	}
	if (FGridTileStaticData* TileDataPtr = GetTileStaticData(Coord))
	{
		return GridRuntimeStateComponent->GetTilePathCost(
			TileDataPtr->TileTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Grid.Type.FlyingOnly"))),
			TileDataPtr->TileTags);
	}
	return 999;
}

bool AGridType::GetNeighbors(FGridCoord Coord, TArray<FGridCoord>& Neighbors, const bool bConsiderFly)
{
	if (!GridRuntimeStateComponent || !GridPathFindingComponent) { return false; }
	
	return GridPathFindingComponent->GetNeighborsCoords(Coord, Neighbors, GridRuntimeStateComponent, bConsiderFly);
}

bool AGridType::GetReachableCoords(FGridCoord Coord, TArray<FGridCoord>& ReachableCoords, bool bIsFlying, int32 MovementPoints)
{
	if (!GridRuntimeStateComponent || !GridPathFindingComponent) { return false; }
	
	return GridPathFindingComponent->GetAllReachableCoords(
		Coord, 
		ReachableCoords, 
		MovementPoints, 
		bIsFlying,
		GridRuntimeStateComponent);
}

