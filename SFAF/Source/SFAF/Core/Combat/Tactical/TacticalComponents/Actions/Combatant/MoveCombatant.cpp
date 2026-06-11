// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "MoveCombatant.h"
#include "Combatant/Combatant_Base.h"
#include "Combatant/CombatantComponents/MovementPointsComponent.h"
#include "Grid/GridType.h"
#include "Grid/GridRuntimeStateComponent.h"

UMoveCombatant::UMoveCombatant()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UMoveCombatant::Execute_Implementation(
    const FGridCoord& InSourceCoord, bool bHasHit, const FGridCoord& InTargetCoord)
{
    // Validate prerequisites: ready state, grid reference, tile existence
    if (!bReady || !Grid || !Grid->GridRuntimeStateComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — bReady=%d, Grid=%s, RuntimeState=%s"),
            bReady, *GetNameSafe(Grid), *GetNameSafe(Grid ? Grid->GridRuntimeStateComponent : nullptr));
        return false;
    }

    if (!Grid->GridRuntimeStateComponent->HasTile(InSourceCoord) ||
        !Grid->GridRuntimeStateComponent->HasTile(InTargetCoord))
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — Invalid coords. Source(%d,%d) exists=%d, Target(%d,%d) exists=%d"),
            InSourceCoord.X, InSourceCoord.Y, Grid->GridRuntimeStateComponent->HasTile(InSourceCoord),
            InTargetCoord.X, InTargetCoord.Y, Grid->GridRuntimeStateComponent->HasTile(InTargetCoord));
        return false;
    }

    // No movement needed if source equals target
    if (InSourceCoord == InTargetCoord)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — Source == Target (%d,%d)"), InSourceCoord.X, InSourceCoord.Y);
        return false;
    }

    if (MovePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — MovePath is empty. Call SetMovePath first."));
        return false;
    }

    SourceCoord = InSourceCoord;
    TargetCoord = InTargetCoord;
    bActionHasHit = bHasHit;

    const FGridTileStaticData* SrcTileData = Grid->GridRuntimeStateComponent->GetStaticTile(InSourceCoord);
    const FGridTileStaticData* TgtTileData = Grid->GridRuntimeStateComponent->GetStaticTile(InTargetCoord);
    if (SrcTileData) SourceTileData = *SrcTileData;
    if (TgtTileData) TargetTileData = *TgtTileData;

    // Validate that a combatant exists at the source tile
    ACombatant_Base* Combatant = GetCombatantAtCoord(InSourceCoord);
    if (!Combatant)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — No combatant at source tile (%d,%d)"), InSourceCoord.X, InSourceCoord.Y);
        return false;
    }

    UMovementPointsComponent* MovementComp = Combatant->FindComponentByClass<UMovementPointsComponent>();
    if (!MovementComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — Combatant %s has no MovementPointsComponent"), *Combatant->GetName());
        return false;
    }

    // Validate target tile is not already occupied
    if (Grid->GridRuntimeStateComponent->GetTileOccupancy(InTargetCoord).OccupyingUnit)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — Target tile (%d,%d) is occupied by %s"),
            InTargetCoord.X, InTargetCoord.Y,
            *GetNameSafe(Grid->GridRuntimeStateComponent->GetTileOccupancy(InTargetCoord).OccupyingUnit));
        return false;
    }

    // Calculate total movement cost for the path
    MoveTotalCost = 0;
    for (int32 i = 1; i < MovePath.Num(); ++i)
    {
        int32 Cost = 1;
        Grid->GridRuntimeStateComponent->GetTileCost(MovePath[i], Cost);
        MoveTotalCost += Cost;
    }

    // Check if the combatant has enough movement points
    if (!MovementComp->HasEnoughMovementPoints(MoveTotalCost))
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::Execute — Not enough MP. Has %d, needs %d"),
            MovementComp->GetMovementPoints(), MoveTotalCost);
        return false;
    }

    MovingCombatant = Combatant;
    MovingComp = MovementComp;

    // Convert path coordinates to world-space locations
    MovePathWorld.Empty();
    for (const FGridCoord& Coord : MovePath)
    {
        const FGridTileStaticData* TileData = Grid->GridRuntimeStateComponent->GetStaticTile(Coord);
        if (TileData)
            MovePathWorld.Add(TileData->WorldLocation);
    }

    // Begin step-by-step movement along the path
    CurrentPathIndex = 0;
    StartPathMovement();

    SetLocationsForMeshes();
    return true;
}

void UMoveCombatant::SetMovePath(const TArray<FGridCoord>& InSortedPath)
{
    MovePath = InSortedPath;
    UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::SetMovePath — Path set with %d tiles"), MovePath.Num());
}

void UMoveCombatant::SetTileMoveDuration(float NewDuration)
{
    TileMoveDuration = NewDuration;
    UE_LOG(LogTemp, Verbose, TEXT("MoveCombatant::SetTileMoveDuration — set to %.1f"), TileMoveDuration);
}

void UMoveCombatant::StartPathMovement()
{
    if (MovePathWorld.Num() <= 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::StartPathMovement — No tiles to traverse (path has %d tiles)"), MovePathWorld.Num());
        FinishMovement();
        return;
    }

    CurrentPathIndex = 1;
    bIsMoving = true;

    MoveToCurrentTile();

    UE_LOG(LogTemp, Warning, TEXT("MoveCombatant — Movement started. Moving to tile %d/%d every %.1f seconds"),
        CurrentPathIndex, MovePathWorld.Num() - 1, TileMoveDuration);

    GetWorld()->GetTimerManager().SetTimer(
        MovementTimerHandle,
        this,
        &UMoveCombatant::AdvancePathTile,
        TileMoveDuration,
        false
    );
}

void UMoveCombatant::AdvancePathTile()
{
    CurrentPathIndex++;

    if (CurrentPathIndex >= MovePathWorld.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::AdvancePathTile — Reached final tile. Finishing movement."));
        FinishMovement();
        return;
    }

    MoveToCurrentTile();

    UE_LOG(LogTemp, Verbose, TEXT("MoveCombatant::AdvancePathTile — Moved to tile %d/%d"),
        CurrentPathIndex, MovePathWorld.Num() - 1);

    GetWorld()->GetTimerManager().SetTimer(
        MovementTimerHandle,
        this,
        &UMoveCombatant::AdvancePathTile,
        TileMoveDuration,
        false
    );
}

void UMoveCombatant::MoveToCurrentTile()
{
    if (!MovingCombatant.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::MoveToCurrentTile — MovingCombatant is null"));
        return;
    }

    if (!MovePathWorld.IsValidIndex(CurrentPathIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::MoveToCurrentTile — Invalid path index %d (path has %d)"),
            CurrentPathIndex, MovePathWorld.Num());
        return;
    }

    MovingCombatant->SetActorLocation(MovePathWorld[CurrentPathIndex]);
}

void UMoveCombatant::FinishMovement()
{
    bIsMoving = false;

    GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

    if (!MovingCombatant.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::FinishMovement — MovingCombatant is null"));
        OnMoveComplete.Broadcast();
        return;
    }

    if (!MovingComp.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::FinishMovement — MovingComp is null for %s"), *MovingCombatant->GetName());
        OnMoveComplete.Broadcast();
        return;
    }

    if (!Grid)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::FinishMovement — Grid is null"));
        OnMoveComplete.Broadcast();
        return;
    }

    FGridTileOccupancy EmptyOccupancy;
    Grid->GridRuntimeStateComponent->SetTileOccupancy(SourceCoord, EmptyOccupancy);

    FGridTileOccupancy TargetOccupancy;
    TargetOccupancy.OccupyingUnit = MovingCombatant.Get();
    Grid->GridRuntimeStateComponent->SetTileOccupancy(TargetCoord, TargetOccupancy);

    UE_LOG(LogTemp, Warning, TEXT("MoveCombatant — %s finished moving from (%d,%d) to (%d,%d). Cost: %d"),
        *MovingCombatant->GetName(), SourceCoord.X, SourceCoord.Y, TargetCoord.X, TargetCoord.Y, MoveTotalCost);

    MovingComp->UseMovementPoints(MoveTotalCost);

    MovingCombatant = nullptr;
    MovingComp = nullptr;

    OnMoveComplete.Broadcast();
}

ACombatant_Base* UMoveCombatant::GetCombatantAtCoord(const FGridCoord& Coord) const
{
    if (!Grid)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::GetCombatantAtCoord — Grid is null"));
        return nullptr;
    }

    if (!Grid->GridRuntimeStateComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::GetCombatantAtCoord — GridRuntimeStateComponent is null"));
        return nullptr;
    }

    const FGridTileStaticData* TileData = Grid->GridRuntimeStateComponent->GetStaticTile(Coord);
    if (!TileData)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::GetCombatantAtCoord — No tile data at (%d,%d)"), Coord.X, Coord.Y);
        return nullptr;
    }

    if (!TileData->Occupancy.OccupyingUnit)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveCombatant::GetCombatantAtCoord — Tile (%d,%d) has no occupying unit"), Coord.X, Coord.Y);
        return nullptr;
    }

    return TileData->Occupancy.OccupyingUnit;
}
