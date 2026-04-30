// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGridTacticalChannel : uint8
{
    None,
    Obstacle,
    DoubleCost,
    TripleCost,
    FlyingOnly
};

class SFAF_API GridTacticalTypes
{
public:
	GridTacticalTypes();
	~GridTacticalTypes();
};
