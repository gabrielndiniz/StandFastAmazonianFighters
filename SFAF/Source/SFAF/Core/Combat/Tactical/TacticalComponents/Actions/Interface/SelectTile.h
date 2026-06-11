// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "SelectTile.generated.h"

/**
 * Action component that tracks the currently selected tile.
 * Provides a bChange flag to detect when the selection state has changed,
 * enabling other components to react to new selections.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API USelectTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructs the SelectTile component.
	 */
	USelectTile();

protected:
	/**
	 * Initializes the select tile component.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Monitors selection state changes each frame.
	 * @param DeltaTime Frame tick delta.
	 * @param TickType The kind of tick this frame.
	 * @param ThisTickFunction The tick function handling this tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Tracks whether the selected tile has changed since the last evaluation */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bChange = true;
};
