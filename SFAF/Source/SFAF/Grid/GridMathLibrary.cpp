// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#include "Grid/GridMathLibrary.h"
#include "GridRuntimeStateComponent.h"

FVector UGridMathLibrary::HexOffsetToWorld(FGridCoord GridCoord, FVector GridOrigin, FVector TileSize, float ZCorrection)
{
    const float WorldX = GridCoord.X * TileSize.X * HEX_HORIZONTAL_SPACING;
    float WorldY = GridCoord.Y * TileSize.Y * HEX_VERTICAL_SPACING;

    if (!IsItEven(GridCoord.X))
    {
        WorldY += TileSize.Y;
    }

    float WorldZ = FMath::GridSnap(GridOrigin.Z, TileSize.Z);
    WorldZ += ZCorrection;

    return FVector(GridOrigin.X + WorldX, GridOrigin.Y + WorldY, WorldZ);
}

FVector UGridMathLibrary::HexFindNearestTilePositionOnXYPlane(FVector Position, const FVector TileSize)
{
    const FGridCoord Coord = HexWorldToOffsetCoord(Position, FVector::ZeroVector, TileSize);

    return HexOffsetToWorld(Coord, FVector::ZeroVector, TileSize, Position.Z);
}

FGridCoord UGridMathLibrary::HexWorldToOffsetCoord(
    const FVector& WorldPos,
    const FVector& GridOrigin,
    const FVector& TileSize)
{
    const float LocalX = WorldPos.X - GridOrigin.X;
    const float LocalY = WorldPos.Y - GridOrigin.Y;

    const int32 X = FMath::RoundToInt(LocalX / (TileSize.X * HEX_HORIZONTAL_SPACING));
    float YBase = LocalY / (TileSize.Y * HEX_VERTICAL_SPACING);

    if (!IsItEven(X))
    {
        YBase -= 1.0f;
    }

    const int32 Y = FMath::RoundToInt(YBase);
    return FGridCoord(X, Y);
}

int32 UGridMathLibrary::FindNearestTileIndex(
    const TArray<FGridCoord>& Positions,
    const TMap<FGridCoord, FGridTileStaticData>& StaticTiles,
    const FGridCoord& Target,
    bool bConsiderFlying)
{
    const FGridTileStaticData* TargetTile = StaticTiles.Find(Target);
    if (!TargetTile)
    {
        return INDEX_NONE;
    }

    float BestDistance = TNumericLimits<float>::Max();
    int32 BestIndex = INDEX_NONE;

    for (int32 Index = 0; Index < Positions.Num(); ++Index)
    {
        const FGridCoord& Coord = Positions[Index];

        const FGridTileStaticData* TileData = StaticTiles.Find(Coord);
        if (!TileData)
        {
            continue;
        }

        const int32 CostMultiplier = UGridRuntimeStateComponent::GetTilePathCost(
            bConsiderFlying,
            TileData->TileTags
        );

        if (CostMultiplier >= 999 && !bConsiderFlying)
        {
            continue;
        }

        float Distance = FVector::Distance(
            TargetTile->WorldLocation,
            TileData->WorldLocation
        );

        if (!bConsiderFlying)
        {
            Distance *= CostMultiplier;
        }

        if (Distance < BestDistance)
        {
            BestDistance = Distance;
            BestIndex = Index;
        }
    }

    return BestIndex;
}

FVector UGridMathLibrary::HexSnapWorldToGrid(
    const FVector& WorldPosition,
    const FVector& GridOrigin,
    const FVector& TileSize)
{
    const FGridCoord Coord =
        HexWorldToOffsetCoord(
            WorldPosition,
            GridOrigin,
            TileSize
        );

    return HexOffsetToWorld(
        Coord,
        GridOrigin,
        TileSize,
        WorldPosition.Z - GridOrigin.Z
    );
}

bool UGridMathLibrary::FindNearestTileFromWorldPosition(const FVector& WorldPosition,
    const TMap<FGridCoord, FGridTileStaticData>& StaticTiles, FGridCoord& OutCoord,
    FGridTileStaticData& OutTileData)
{
    float BestDistanceSq = TNumericLimits<float>::Max();

    bool bFoundTile = false;

    for (const TPair<FGridCoord, FGridTileStaticData>& Pair : StaticTiles)
    {
        const float DistanceSq =
            FVector::DistSquared(
                WorldPosition,
                Pair.Value.WorldLocation
            );

        if (DistanceSq < BestDistanceSq)
        {
            BestDistanceSq = DistanceSq;

            OutCoord = Pair.Key;
            OutTileData = Pair.Value;

            bFoundTile = true;
        }
    }

    return bFoundTile;
}

bool UGridMathLibrary::IsItEven(int32 Number)
{
    return (Number % 2 == 0);
}

bool UGridMathLibrary::GetHexNeighborTiles(const FGridCoord Coord,
    TArray<FGridCoord>& NeighborsCoords)
{
    NeighborsCoords.Empty();
    
    FGridCoord Result;
    int32 i = 0;
    int32 j = 0;
    //Same collum first
    for (i = 0 ; i <= 1 ; ++i)
    {                 
        Result.X=i*4-2+Coord.X;
        Result.Y=Coord.Y;
        NeighborsCoords.Add(Result);        
    }
    
    if (IsItEven(Coord.X))
    {
        for (i = 0 ; i <= 1 ; ++i)
        {
            Result.X=Coord.X-1+i*2;
            for (j = 0 ; j <= 1 ; ++j)
            {
                Result.Y=Coord.Y-j;
                NeighborsCoords.Add(Result); 
            }
        }
    }
    else
    {
        for (i = 0 ; i <= 1 ; ++i)
        {
            Result.X=Coord.X-1+i*2;
            for (j = 0 ; j <= 1 ; ++j)
            {
                Result.Y=Coord.Y+j;
                NeighborsCoords.Add(Result); 
            }
        }
    }
    
    return !NeighborsCoords.IsEmpty();
}

bool UGridMathLibrary::IsInTheSameLine(FGridCoord Coord1, FGridCoord Coord2)
{
    if (Coord1.X == Coord2.X)
    {
        return true;
    }
    if (Coord1.Y == Coord2.Y)
    {
        return IsItEven(Coord1.X) == IsItEven(Coord2.X);
    }
    
    return false;
}
