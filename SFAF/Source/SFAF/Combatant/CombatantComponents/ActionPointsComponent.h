// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionPointsComponent.generated.h"

/**
 * Component managing the action points (AP) of a combatant.
 * Handles current and maximum AP with modification logic.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UActionPointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionPointsComponent();

	/** Gets current action points */
	UFUNCTION(BlueprintCallable, Category = "Combatant|ActionPoints")
	float GetActionPoints() const { return ActionPoints; }

	/** Gets maximum action points */
	UFUNCTION(BlueprintCallable, Category = "Combatant|ActionPoints")
	float GetMaxActionPoints() const { return MaxActionPoints; }

	/** Sets maximum action points */
	UFUNCTION(BlueprintCallable, Category = "Combatant|ActionPoints")
	void SetMaxActionPoints(float NewMaxAP);

	/** 
	 * Modifies current action points by Delta.
	 * Returns true if the modification was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combatant|ActionPoints")
	bool ModifyActionPoints(float Delta);

protected:
	/** Current action points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combatant|ActionPoints", meta = (AllowPrivateAccess = "true"))
	float ActionPoints;

	/** Maximum action points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combatant|ActionPoints", meta = (AllowPrivateAccess = "true"))
	float MaxActionPoints;
};
