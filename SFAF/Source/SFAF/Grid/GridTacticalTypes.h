// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"

/** Enumeration of tactical modifier types that affect tile movement costs */
UENUM(BlueprintType)
enum class EGridTacticalChannel : uint8
{
	/** No tactical modifier */
	None,
	/** Tile is blocked / impassable */
	Obstacle,
	/** Tile costs 2 movement points to enter */
	DoubleCost,
	/** Tile costs 3 movement points to enter */
	TripleCost,
	/** Only flying units can occupy this tile */
	FlyingOnly
};
