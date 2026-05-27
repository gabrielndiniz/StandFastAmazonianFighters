// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "TacticalPlayerController.h"
#include "Controllers/ControllerComponents/TacticalControllerComponent.h"
#include "Grid/GridType.h"
#include "Kismet/GameplayStatics.h"

ATacticalPlayerController::ATacticalPlayerController()
{
	TacticalControllerComponent = CreateDefaultSubobject<UTacticalControllerComponent>(TEXT("TacticalControllerComponent"));
}

void ATacticalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	Grid = Cast<AGridType>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridType::StaticClass()));
}

void ATacticalPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SetTileUnderCursor();
}


void ATacticalPlayerController::SetTileUnderCursor() const
{
	FHitResult HitResult;
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel4;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(CollisionChannel), true, HitResult);
				
	if (HitResult.bBlockingHit)
	{
		FVector Location = HitResult.Location;
		FGridCoord Coord;
		FGridTileStaticData TileData;
		if (Grid->FindNearestTileFromLocation(Location, Coord, TileData))
		{
			if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("HoverTile"), false))
			{
				UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
			}
			if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("HoverTile"), true))
			{
				UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
			}
		}
	}
}

bool ATacticalPlayerController::SelectTile() const
{
	if (!TacticalControllerComponent->GetDebugMode() && !TacticalControllerComponent->IsMyTurn()) {return false;}
	
	FGridCoord Coord;
	if (!TacticalControllerComponent->GetCoordToComponent(Coord, TEXT("HoverTile"), false))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to get coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->GetCoordToComponent(Coord, TEXT("HoverTile"), true))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to get coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("SelectTile"), false))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("SelectTile"), true))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("NeighborTile"), false))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("NeighborTile"), true))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	return true;
}


bool ATacticalPlayerController::TargetTile() const
{
	if (!TacticalControllerComponent->GetDebugMode() && !TacticalControllerComponent->IsMyTurn()) {return false;}
	
	FGridCoord Coord;
	if (!TacticalControllerComponent->GetCoordToComponent(Coord, TEXT("HoverTile"), false))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to get coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->GetCoordToComponent(Coord, TEXT("HoverTile"), true))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to get coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("TargetTile"), false))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	if (!TacticalControllerComponent->AddCoordToComponent(Coord, TEXT("TargetTile"), true))
	{
		UE_LOG(LogTemp, Warning, TEXT("TacticalPlayerController: Failed to add coord to component"));
		return false;
	}
	return true;
}
