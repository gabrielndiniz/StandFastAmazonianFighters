// Â© 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel NÃ³bile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Combatant_Base.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CombatantDatabaseDataAsset.generated.h"

/**
 * Defines the States of Units
 * TODO: Unit Stats, Unit Spells States (maybe another Data Asset)
 */

/** Visual representation data for a combatant unit */
USTRUCT(BlueprintType)
struct FCombatantVisualData
{
	GENERATED_BODY()

	/** Skeletal mesh used for the 3D model of the unit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	/** Animation blueprint class for unit animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBlueprint;

	/** UI icon representing the unit */
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

/** General combatant definition data stored per unit type */
USTRUCT(BlueprintType)
struct FCombatantData
{
	GENERATED_BODY()

	/** The combatant class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACombatant_Base> CombatantClass;
	
	/** Tags defining the unit's roles (e.g., Damage, Support) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer UnitRoles;

	/** Gameplay tag for movement type (e.g., Walking, Flying) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MovementTag;

	/** Visual representation of the unit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatantVisualData VisualData;
	
	/** Faction tag for team/alignment identification */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag FactionTag;
	
	/** AI-specific behavior data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAIData AIData;
	
	/** Whether the AI considers this unit a low-risk target */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLowRisk = false;
};
/** Primary data asset containing all combatant definitions keyed by gameplay tag */
UCLASS()
class SFAF_API UCombatantDatabaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/*
	Maps unit type tags (e.g., Unit.Type.BulletAnt) to their full combatant data definitions
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap <FGameplayTag, FCombatantData> CombatantData;
	
	/** Retrieves combatant data for a given unit type tag */
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	bool GetCombatantData(FGameplayTag Tag, FCombatantData& OutData) const;
	
	/** Returns all registered combatant type tags in the database */
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	TArray<FGameplayTag> GetAllCombatantTypeTag();
	
	
};
	
