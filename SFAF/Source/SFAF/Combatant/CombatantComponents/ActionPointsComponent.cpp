// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "ActionPointsComponent.h"

UActionPointsComponent::UActionPointsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxActionPoints = 6.0f;
	ActionPoints = MaxActionPoints;
}

void UActionPointsComponent::SetMaxActionPoints(float NewMaxAP)
{
	if (NewMaxAP < 0.0f) return;
	MaxActionPoints = NewMaxAP;
	ActionPoints = FMath::Min(ActionPoints, MaxActionPoints);
}

bool UActionPointsComponent::ModifyActionPoints(float Delta)
{
	if (Delta == 0.0f) return true;

	const float NewAP = ActionPoints + Delta;
	
	// If Delta is negative (taking AP), return false if we don't have enough
	if (Delta < 0.0f && NewAP < -0.001f) // small epsilon
	{
		return false;
	}

	ActionPoints = FMath::Clamp(NewAP, 0.0f, MaxActionPoints);
	return true;
}

