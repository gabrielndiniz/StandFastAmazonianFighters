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

/**
 * Visual Data
 */
USTRUCT(BlueprintType)
struct FCombatantVisualData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};

/**
 * AI Data
 */
USTRUCT(BlueprintType)
struct FAIData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AILastMovementTags;
};

/**
 * General combatant definition data
 */
USTRUCT(BlueprintType)
struct FCombatantData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ACombatant_Base> CombatantClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer UnitRoles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MovementTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCombatantVisualData VisualData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag FactionTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAIData AIData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLowRisk = false;
};
UCLASS()
class SFAF_API UCombatantDatabaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/*
	Example:
	Unit.Type.BulletAnt
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap <FGameplayTag, FCombatantData> CombatantData;
	
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	bool GetCombatantData(FGameplayTag Tag, FCombatantData& OutData) const;
	
	UFUNCTION(BlueprintCallable, Category = "Combatant")
	TArray<FGameplayTag> GetAllCombatantTypeTag();
	
	
};
	
