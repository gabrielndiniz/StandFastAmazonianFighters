// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"

#include "GridCoord.generated.h"

/**
 * Structure representing coordinates on the grid (X, Y).
 * Used for identifying tile positions and performing grid math.
 */
USTRUCT(BlueprintType)
struct FGridCoord
{
	GENERATED_BODY()

	/** Default constructor. Initializes to (0, 0). */
	FGridCoord() {}

	/** 
	 * Constructor with parameters.
	 * @param InX The X coordinate.
	 * @param InY The Y coordinate.
	 */
	FGridCoord(int32 InX, int32 InY) : X(InX), Y(InY) {}

	/** The X position on the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 X = -1;

	/** The Y position on the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Y = -1;

	/** Equality operator for FGridCoord. */
	FORCEINLINE bool operator==(const FGridCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	/** Inequality operator for FGridCoord. */
	FORCEINLINE bool operator!=(const FGridCoord& Other) const
	{
		return X != Other.X || Y != Other.Y;
	}
};

FORCEINLINE uint32 GetTypeHash(const FGridCoord& Coord)
{
	return HashCombine(
		GetTypeHash(Coord.X),
		GetTypeHash(Coord.Y)
	);
}

