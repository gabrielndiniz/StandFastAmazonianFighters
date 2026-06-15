// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthPointsComponent.generated.h"

/**
 * Component managing the health of a combatant.
 * Handles current and maximum health points with modification logic.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UHealthPointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthPointsComponent();

	/** Gets current health */
	UFUNCTION(BlueprintCallable, Category = "Combatant|Health")
	float GetHealth() const { return Health; }

	/** Gets maximum health */
	UFUNCTION(BlueprintCallable, Category = "Combatant|Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** Sets maximum health, optionally adjusting current health proportionally */
	UFUNCTION(BlueprintCallable, Category = "Combatant|Health")
	void SetMaxHealth(float NewMaxHealth, bool bAdjustCurrentHealth = true);

	/** 
	 * Modifies current health by Delta.
	 * Returns true if the modification was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combatant|Health")
	bool ModifyHealth(float Delta);

protected:
	/** Current health points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combatant|Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	/** Maximum health points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combatant|Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
};
