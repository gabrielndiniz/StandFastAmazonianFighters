// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"


#include "SpellCharacteristics.generated.h"

/** Targeting system choice */
UENUM(BlueprintType)
enum class ETargetTileRequirement : uint8
{
	/** Required an empty tile. It is not a null value */
	Empty UMETA (DisplayName = "Empty"),
	
	/** Required an occupied tile. */
	Occupied UMETA (DisplayName = "Occupied"),
	
	/** Both of them */
	Either UMETA (DisplayName = "Either")
};
