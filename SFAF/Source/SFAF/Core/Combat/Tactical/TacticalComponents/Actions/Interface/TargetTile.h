// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "TargetTile.generated.h"

/**
 * Action component that tracks the target tile for tactical actions.
 * Used to designate a destination for movement or ability targeting.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UTargetTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the TargetTile component.
	 */
	UTargetTile();

protected:
	/**
	 * Initializes the target tile component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Updates target tile state each frame.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
