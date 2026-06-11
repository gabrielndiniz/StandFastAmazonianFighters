// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.generated.h"

/**
 * Represents a 2D coordinate on the hex grid (X, Y).
 * Used throughout the grid system to identify individual tile positions
 * and perform hex-based mathematical operations.
 */
USTRUCT(BlueprintType)
struct FGridCoord
{
	GENERATED_BODY()

	/** Default constructor. Initializes to (-1, -1) to indicate an invalid/unset coordinate. */
	FGridCoord() {}

	/**
	 * Constructs a grid coordinate with explicit X and Y values.
	 * @param InX The column index on the hex grid.
	 * @param InY The row index on the hex grid.
	 */
	FGridCoord(int32 InX, int32 InY) : X(InX), Y(InY) {}

	/** The column (X-axis) position on the grid. Default -1 indicates an unset state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 X = -1;

	/** The row (Y-axis) position on the grid. Default -1 indicates an unset state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Y = -1;

	/**
	 * Implicit conversion to FIntPoint for compatibility with Unreal utilities.
	 * @return The coordinate as an FIntPoint.
	 */
	operator FIntPoint() const { return FIntPoint(X, Y); }

	/** Returns true if both coordinates have identical X and Y values */
	FORCEINLINE bool operator==(const FGridCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	/** Returns true if X or Y differ between the two coordinates */
	FORCEINLINE bool operator!=(const FGridCoord& Other) const
	{
		return X != Other.X || Y != Other.Y;
	}
};

/** Hash function for FGridCoord, enabling its use as a TMap key or TSet element */
FORCEINLINE uint32 GetTypeHash(const FGridCoord& Coord)
{
	return HashCombine(GetTypeHash(Coord.X), GetTypeHash(Coord.Y));
}
