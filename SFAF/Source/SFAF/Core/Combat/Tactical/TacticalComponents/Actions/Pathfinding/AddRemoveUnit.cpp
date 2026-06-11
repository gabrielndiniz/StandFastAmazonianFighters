// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "AddRemoveUnit.h"
#include "Combatant/Combatant_Base.h"
#include "Combatant/CombatantComponents/TeamComponent.h"
#include "Core/Combat/Team/TeamSubsystem.h"


bool UAddRemoveUnit::Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit,
	const FGridCoord& InTargetCoord)
{
	// Route execution based on the configured mode
	if (bChangeTeam)
	{
		ChangeUnitTeam(InSourceCoord);
	}
	else
	{
		if (bAdd)
		{
			AddUnit(InSourceCoord);
		}
		else
		{
			RemoveUnit(InSourceCoord);
		}
	}
	
	return Super::Execute_Implementation(InSourceCoord, bHasHit, InTargetCoord);
}

void UAddRemoveUnit::AddUnit(FGridCoord Coord)
{
	// Validate dependencies
	if (!CombatantDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — CombatantDatabase is null"));
		return;
	}
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — Grid is null"));
		return;
	}
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — GetWorld() is null"));
		return;
	}

	// Look up combatant data from the database
	FCombatantData CombatantData;
	if (!CombatantDatabase->GetCombatantData(CurrentUnit, CombatantData))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — No data for tag %s"), *CurrentUnit.ToString());
		return;
	}
	if (!CombatantData.CombatantClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — CombatantClass is null for tag %s"), *CurrentUnit.ToString());
		return;
	}

	// Validate the target tile exists and is unoccupied
	FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(Coord);
	if (!TileStaticData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — No tile data at (%d,%d)"), Coord.X, Coord.Y);
		return;
	}
	
	if (TileStaticData->Occupancy.OccupyingUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — Tile (%d,%d) already occupied by %s"),
			Coord.X, Coord.Y, *GetNameSafe(TileStaticData->Occupancy.OccupyingUnit));
		return;
	}

	// Spawn the new combatant at the tile location
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACombatant_Base* SpawnedUnit = GetWorld()->SpawnActor<ACombatant_Base>(
		CombatantData.CombatantClass,
		TileStaticData->WorldLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (!SpawnedUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — SpawnActor failed at location %s"),
			*TileStaticData->WorldLocation.ToString());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — Spawned %s at (%d,%d)"), *SpawnedUnit->GetName(), Coord.X, Coord.Y);

	// Update tile occupancy and assign team
	FGridTileOccupancy Occupancy;
	Occupancy.OccupyingUnit = SpawnedUnit;
	TileStaticData->Occupancy = Occupancy;
	UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::AddUnit — Occupancy set for tile (%d,%d)"), Coord.X, Coord.Y);

	ChangeUnitTeam(Coord);
}

void UAddRemoveUnit::RemoveUnit(FGridCoord Coord)
{
	
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit: No Grid."));
		return;
	}

	FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(Coord);
	if (!TileStaticData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit: No TileStaticData."));
		return;
	}

	ACombatant_Base* Unit = TileStaticData->Occupancy.OccupyingUnit;
	
	UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::RemoveUnit — Found unit at tile: %s"), *GetNameSafe(Unit));
	
	if (!Unit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit: No unit to remove."));
		return;
	}

	int32 UnitTeam = 0;
	if (UTeamComponent* TC = Unit->FindComponentByClass<UTeamComponent>())
	{
		UnitTeam = TC->TeamNumber;
	}

	if (UTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UTeamSubsystem>())
	{
		TeamSubsystem->UnregisterUnitFromTeam(Unit, UnitTeam);
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::RemoveUnit — Unregistered from team %d"), UnitTeam);
	}

	Unit->DestroyUnit();
	UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::RemoveUnit — Unit destroyed"));

	TileStaticData->Occupancy.OccupyingUnit = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::RemoveUnit — Finished removing unit at (%d,%d)"), Coord.X, Coord.Y);
}

void UAddRemoveUnit::ChangeUnitTeam(FGridCoord Coord)
{
	if (!Grid || !GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::ChangeUnitTeam — No Grid Found"));
		return;
	}

	FGridTileStaticData* TileData = Grid->GetTileStaticData(Coord);
	if (!TileData || !TileData->Occupancy.OccupyingUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddRemoveUnit::ChangeUnitTeam — No Unit found"));
		return;
	}
	TileData->Occupancy.OccupyingUnit->SetTeam(Team);
}

