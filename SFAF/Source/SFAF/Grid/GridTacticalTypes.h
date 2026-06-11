// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Enumeration of tactical modifier types that affect tile movement costs */
UENUM(BlueprintType)
enum class EGridTacticalChannel : uint8
{
	/** No tactical modifier applied */
	None,
	/** Tile is blocked and impassable to ground units */
	Obstacle,
	/** Tile costs 2 movement points to enter */
	DoubleCost,
	/** Tile costs 3 movement points to enter */
	TripleCost,
	/** Only flying units can traverse this tile */
	FlyingOnly
};
