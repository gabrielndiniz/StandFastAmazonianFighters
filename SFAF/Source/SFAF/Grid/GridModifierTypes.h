// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridModifierTypes.generated.h"

/** Defines how a grid modifier applies its cost value to a tile */
UENUM(BlueprintType)
enum class EGridModifierMode : uint8
{
	/** Replace the tile's existing cost with the modifier's cost */
	Override UMETA(DisplayName = "Override"),
	/** Add the modifier's cost to the tile's existing cost */
	Additive UMETA(DisplayName = "Additive")
};
