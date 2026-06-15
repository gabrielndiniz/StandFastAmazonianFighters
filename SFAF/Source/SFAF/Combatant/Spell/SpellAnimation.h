// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Combatant/GAS/BaseAnimation.h"
#include "GameFramework/Actor.h"
#include "SpellAnimation.generated.h"

UCLASS(Abstract, Blueprintable)
class SFAF_API ASpellAnimation : public AActor
{
	GENERATED_BODY()

public:
	ASpellAnimation();

	/** The animation asset to play (Montage, Sequence, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell")
	TObjectPtr<UAnimationAsset> AnimationAsset;
};
