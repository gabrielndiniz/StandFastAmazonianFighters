// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "HoverTile.generated.h"

/**
 * Action component that tracks the tile currently under the cursor (hovered).
 * Updated each frame by the player controller to provide real-time hover feedback.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UHoverTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the HoverTile component.
	 */
	UHoverTile();

protected:
	/**
	 * Initializes the hover tile component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Updates the hovered tile based on cursor position each frame.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
