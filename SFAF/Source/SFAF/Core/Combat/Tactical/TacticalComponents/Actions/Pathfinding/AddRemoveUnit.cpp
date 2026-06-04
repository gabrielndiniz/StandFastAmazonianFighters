// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "AddRemoveUnit.h"
#include "Combatant/Combatant_Base.h"
#include "Combatant/CombatantComponents/TeamComponent.h"
#include "Core/Combat/Team/TeamSubsystem.h"


bool UAddRemoveUnit::Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit,
	const FGridCoord& InTargetCoord)
{
	
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
	if (!CombatantDatabase || !Grid || !GetWorld())
		return;

	FCombatantData CombatantData;
	if (!CombatantDatabase->GetCombatantData(CurrentUnit, CombatantData) || !CombatantData.CombatantClass)
		return;

	FGridTileStaticData* TileStaticData = Grid->GetTileStaticData(Coord);
	if (!TileStaticData)
		return;
	
	if (TileStaticData->Occupancy.OccupyingUnit)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACombatant_Base* SpawnedUnit = GetWorld()->SpawnActor<ACombatant_Base>(
		CombatantData.CombatantClass,
		TileStaticData->WorldLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (!SpawnedUnit)
		return;

	FGridTileOccupancy Occupancy;
	Occupancy.OccupyingUnit = SpawnedUnit;
	TileStaticData->Occupancy = Occupancy;

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

