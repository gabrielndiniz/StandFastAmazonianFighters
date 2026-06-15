// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SpellCharacteristics.h"
#include "SpellAnimation.h"
#include "SpellAuxiliaryEffect.h"
#include "SpellFX.h"
#include "SpellDatabaseDataAsset.generated.h"

/**
 * Visuals and effects for a spell.
 */
USTRUCT(BlueprintType)
struct FSpellVisualData
{
	GENERATED_BODY()

	/** Animation to play when casting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	TSubclassOf<ASpellAnimation> CastAnimation;

	/** FX actor to spawn for the primary area (Visuals + Audio) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	TSubclassOf<ASpellFX> PrimaryFX;

	/** FX actor to spawn for the secondary area (Visuals + Audio) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	TSubclassOf<ASpellFX> SecondaryFX;

	/** Z-axis height offset for the effect spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	float HeightOffset = 0.0f;

	/** XY-plane offset from the tile center */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	FVector2D TileOffset = FVector2D::ZeroVector;
};

/**
 * Range and targeting constraints for a spell.
 */
USTRUCT(BlueprintType)
struct FSpellRangeData
{
	GENERATED_BODY()

	/** Minimum range of the spell in tiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	int32 MinRange = 1;

	/** Maximum range of the spell in tiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	int32 MaxRange = 5;

	/** If the spell requires a direct line of sight to the target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	bool bRequireLineOfSight = true;

	/** Requirements for the target tile (Empty, Occupied, Either) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	ETargetTileRequirement TargetRequirement = ETargetTileRequirement::Either;
};

/**
 * Area of effect definitions for a spell.
 */
USTRUCT(BlueprintType)
struct FSpellAreaData
{
	GENERATED_BODY()

	/** Radius/Size of the primary area (center of the spell) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float PrimaryArea = 0.0f;

	/** Radius/Size of the secondary area (affected by splash) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float SecondaryArea = 0.0f;
};

/**
 * Min and Max values for damage or healing.
 */
USTRUCT(BlueprintType)
struct FSpellDamageRange
{
	GENERATED_BODY()

	/** Minimum damage value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float MinDamage = 0.0f;

	/** Maximum damage value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float MaxDamage = 0.0f;
};

/**
 * Damage data for primary and secondary areas across different resources.
 */
USTRUCT(BlueprintType)
struct FSpellDamageData
{
	GENERATED_BODY()

	/** Health damage to primary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Primary")
	FSpellDamageRange PrimaryHealthDamage;

	/** Action Point damage to primary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Primary")
	FSpellDamageRange PrimaryActionPointDamage;

	/** Movement Point damage to primary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Primary")
	FSpellDamageRange PrimaryMovementPointDamage;

	/** Health damage to secondary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Secondary")
	FSpellDamageRange SecondaryHealthDamage;

	/** Action Point damage to secondary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Secondary")
	FSpellDamageRange SecondaryActionPointDamage;

	/** Movement Point damage to secondary area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage|Secondary")
	FSpellDamageRange SecondaryMovementPointDamage;
};

/**
 * Costs and timing constraints for a spell.
 */
USTRUCT(BlueprintType)
struct FSpellCostData
{
	GENERATED_BODY()

	/** Cost in Action Points (AP) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	float Cost_AP = 1.0f;

	/** Cost in Health Points (HP) - For blood magic etc. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	float Cost_HP = 0.0f;

	/** Maximum times this spell can be cast per turn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	int32 MaxCastsPerTurn = 1;

	/** Cooldown in turns after use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	int32 Cooldown = 0;

	/** If the combatant can move after casting this spell */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCanMoveAfter = false;
};

/**
 * Specialized effects for a spell, such as summoning or teleportation.
 */
USTRUCT(BlueprintType)
struct FSpellAuxiliaryData
{
	GENERATED_BODY()

	/** Auxiliary effect for the primary area of effect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auxiliary")
	TSubclassOf<ASpellAuxiliaryEffect> PrimaryAuxiliaryEffect;

	/** Auxiliary effect for the secondary area of effect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auxiliary")
	TSubclassOf<ASpellAuxiliaryEffect> SecondaryAuxiliaryEffect;
};

/**
 * Data structure defining a spell's properties and effects.
 */
USTRUCT(BlueprintType)
struct FSpellData
{
	GENERATED_BODY()

	/** Unique name of the spell */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FText SpellName;

	/** Detailed description of the spell's effect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell", meta = (MultiLine = true))
	FText SpellDescription;

	/** Icon for spell bar */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	TObjectPtr<UTexture2D> Icon;

	/** Range and targeting configuration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellRangeData RangeData;

	/** Area of effect configuration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellAreaData AreaData;

	/** Damage configuration for primary and secondary areas */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellDamageData DamageData;

	/** Cost and timing configuration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellCostData CostData;

	/** Visuals and animation configuration */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellVisualData VisualData;

	/** Specialized auxiliary effects (Summon, Teleport, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
	FSpellAuxiliaryData AuxiliaryData;
};

/**
 * Primary data asset containing all spell definitions keyed by gameplay tag.
 */
UCLASS()
class SFAF_API USpellDatabaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Maps spell type tags (e.g., Spell.Fireball) to their full definitions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spells")
	TMap<FGameplayTag, FSpellData> SpellData;
	
	/** Retrieves spell data for a given spell tag */
	UFUNCTION(BlueprintCallable, Category = "Spells")
	bool GetSpellData(FGameplayTag Tag, FSpellData& OutData) const;
};
