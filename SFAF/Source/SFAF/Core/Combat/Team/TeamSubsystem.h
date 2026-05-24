// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "TeamSubsystem.generated.h"

class ACombatant_Base;

/**
 * Stores the data for a single team.
 *
 * Used by the team registry to group combatants, team number, and color.
 */
USTRUCT(BlueprintType)
struct FTeam
{
    GENERATED_BODY()

    /** Identify the controller of the team. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    TWeakObjectPtr<AController> Controller;

    /** Display color used by the team. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    FLinearColor Color = FLinearColor::White;

    /** Combatants currently assigned to this team. */
    UPROPERTY()
    TArray<TWeakObjectPtr<ACombatant_Base>> Combatants;
};

/**
 * Global combat team registry subsystem.
 *
 * Responsible for:
 * - Registering combatants into teams.
 * - Querying friendly/enemy relations.
 * - Managing tactical team ownership.
 *
 * Lifetime:
 * - Automatically created per World/Map.
 * - Exists during the entire tactical match.
 */
UCLASS()
class SFAF_API UTeamSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:

    // -----------------------------------------------------------------------
    // Team Registration API
    // -----------------------------------------------------------------------

    /**
     * Registers a combatant into a team.
     *
     * @param Actor
     * Combatant actor being registered.
     *
     * @param TeamTag
     * Gameplay tag representing the combatant team.
     */
    UFUNCTION(BlueprintCallable, Category = "Team")
    void RegisterUnitToTeam(ACombatant_Base* Unit, int32 TeamNumber);

protected:

    // -----------------------------------------------------------------------
    // Runtime Team Registry
    // -----------------------------------------------------------------------

    /**
     * Stores all registered combatants grouped by team tag.
     *
     * Example:
     * Team 1
     *   -> Unit A
     *   -> Unit B
     *
     * Team 2
     *   -> Unit X
     *   -> Unit Y
     */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Team")
    TMap<int32, FTeam> TeamRegistry;
    
    /** The quantity of teams on the game*/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Team")
    int32 TeamQuantity = 0;
};
