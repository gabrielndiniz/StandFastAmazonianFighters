// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TeamSubsystem.generated.h"

class ACombatant_Base;

/** Stores runtime data for a single team: controller, display color, and registered combatants */
USTRUCT(BlueprintType)
struct FTeam
{
    GENERATED_BODY()

    /** The controller assigned to this team */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    TWeakObjectPtr<AController> Controller;

    /** Display color used by the team for UI and identification */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    FLinearColor Color = FLinearColor::White;

    /** Combatant actors currently assigned to this team */
    UPROPERTY()
    TArray<TWeakObjectPtr<ACombatant_Base>> Combatants;
};

/**
 * Global world subsystem managing combat team registration and queries.
 * Automatically created per World/Map and lives for the duration of the tactical match.
 * Responsible for:
 * - Registering/unregistering combatants to/from teams.
 * - Tracking team composition and ownership.
 */
UCLASS()
class SFAF_API UTeamSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // -----------------------------------------------------------------------
    // Team Registration API
    // -----------------------------------------------------------------------

    /** Registers a combatant actor into the specified team. Creates the team if it doesn't exist. */
    UFUNCTION(BlueprintCallable, Category = "Team")
    void RegisterUnitToTeam(ACombatant_Base* Unit, int32 TeamNumber);

    /** Removes a combatant from the specified team. Cleans up empty team entries. */
    UFUNCTION(BlueprintCallable, Category = "Team")
    void UnregisterUnitFromTeam(ACombatant_Base* Unit, int32 TeamNumber);

protected:
    // -----------------------------------------------------------------------
    // Runtime Team Registry
    // -----------------------------------------------------------------------

    /**
     * Maps team numbers to their runtime team data (controller, color, combatant list).
     * Example:
     *   Team 1 -> {Controller: PC, Combatants: [UnitA, UnitB]}
     *   Team 2 -> {Controller: AIC, Combatants: [UnitX, UnitY]}
     */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Team")
    TMap<int32, FTeam> TeamRegistry;
    
    /** The total number of distinct teams currently registered */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Team")
    int32 TeamQuantity = 0;
};
