// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "HealthPointsComponent.h"

UHealthPointsComponent::UHealthPointsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void UHealthPointsComponent::SetMaxHealth(float NewMaxHealth, bool bAdjustCurrentHealth)
{
	if (NewMaxHealth <= 0.0f) return;

	if (bAdjustCurrentHealth)
	{
		const float Ratio = NewMaxHealth / MaxHealth;
		Health *= Ratio;
	}

	MaxHealth = NewMaxHealth;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

bool UHealthPointsComponent::ModifyHealth(float Delta)
{
	if (Delta == 0.0f) return true;

	const float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	// Example of a reason to return false: character is already dead and trying to take more damage (if desired)
	// Or if Delta is negative and we are invulnerable, etc.
	// For now, we just apply it and return true if it changed.
	
	Health = NewHealth;
	return true;
}

