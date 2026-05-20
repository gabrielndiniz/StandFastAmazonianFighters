// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Grid/GridMathLibrary.h"

#include "GridRuntimeStateComponent.h"

FVector UGridMathLibrary::HexOffsetToWorld(FGridCoord GridCoord, FVector GridOrigin, FVector TileSize,
                                           float ZCorrection)
{
	float WorldX = GridCoord.X * TileSize.X *	HEX_HORIZONTAL_SPACING;

	float WorldY = GridCoord.Y * TileSize.Y * HEX_VERTICAL_SPACING;

	// Offset odd columns
	if (GridCoord.X % 2 != 0)
	{
		WorldY += TileSize.Y;
	}

	float WorldZ = FMath::GridSnap(GridOrigin.Z, TileSize.Z);

	WorldZ += ZCorrection;

	return FVector(GridOrigin.X + WorldX,GridOrigin.Y + WorldY,WorldZ);
}

int32 UGridMathLibrary::FindNearestTileIndex(const TArray<FGridCoord>& Positions,
	const TMap<FGridCoord, FGridTileStaticData>& StaticTiles, const FGridCoord& Target, bool bConsiderFlying)
{
	const FGridTileStaticData* TargetTile =
		StaticTiles.Find(Target);

	if (!TargetTile)
	{
		return INDEX_NONE;
	}

	float BestDistance =
		TNumericLimits<float>::Max();

	int32 BestIndex = INDEX_NONE;

	for (int32 Index = 0; Index < Positions.Num(); ++Index)
	{
		const FGridCoord& Coord =
			Positions[Index];

		const FGridTileStaticData* TileData =
			StaticTiles.Find(Coord);

		if (!TileData)
		{
			continue;
		}

		const int32 CostMultiplier =
			UGridRuntimeStateComponent::GetTilePathCost(
				bConsiderFlying,
				TileData->TileTags
			);

		if (CostMultiplier >= 999 && !bConsiderFlying)
		{
			continue;
		}

		float Distance =
			FVector::Distance(
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
