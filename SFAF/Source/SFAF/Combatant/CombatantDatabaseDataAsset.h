// Copyright 2025 StandFast Games, LLC

#pragma once

#include "CoreMinimal.h"
#include "Combatant_Base.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CombatantDatabaseDataAsset.generated.h"

/**
 * Visual representation data for a combatant unit.
 * Defines the 3D model, animation blueprint, and UI icon.
 */
USTRUCT(BlueprintType)
struct FCombatantVisualData
{
	GENERATED_BODY()

	/** Skeletal mesh used for the unit's 3D model */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	/** Animation blueprint class for unit animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBlueprint;

	/** UI icon representing the unit in menus and HUD */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};

/** AI-specific configuration data for a combatant */
USTRUCT(BlueprintType)
struct FAIData
{
	GENERATED_BODY()
	
	/** Gameplay tag tracking the AI's last movement action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AILastMovementTags;
};

/** Full definition data for a combatant type, including class, roles, movement, visuals, and AI */
USTRUCT(BlueprintType)
struct FCombatantData
{
	GENERATED_BODY()

	/** The combatant subclass to spawn for this unit type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACombatant_Base> CombatantClass;
	
	/** Tags defining the unit's functional roles (e.g., Damage, Support, Tank) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer UnitRoles;

	/** Gameplay tag for movement type (e.g., Walking, Flying) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MovementTag;

	/** Visual representation data for this unit type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatantVisualData VisualData;
	
	/** Faction tag for team/alignment identification */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag FactionTag;
	
	/** AI-specific behavior data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAIData AIData;
	
	/** Whether the AI considers this unit a low-risk target (prefers attacking it) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLowRisk = false;
	
	/** Tags defining the unit's spells - maps to SpellDatabaseDataAsset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combatant|Spells")
	FGameplayTagContainer UnitSpells;
};

/**
 * Primary data asset containing all combatant definitions keyed by gameplay tag.
 * Maps unit type tags (e.g., Unit.Type.BulletAnt) to their full combatant data.
 */
UCLASS()
class SFAF_API UCombatantDatabaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Maps unit type tags to their complete combatant definitions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap <FGameplayTag, FCombatantData> CombatantData;
	
	/** Retrieves combatant data for a given unit type tag */
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	bool GetCombatantData(FGameplayTag Tag, FCombatantData& OutData) const;
	
	/** Returns all registered unit type tags in the database */
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	TArray<FGameplayTag> GetAllCombatantTypeTag();
};
