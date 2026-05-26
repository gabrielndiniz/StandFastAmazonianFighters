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
			TacticalControllerComponent->SetHoveredTile(Coord);
		}
	}
}
