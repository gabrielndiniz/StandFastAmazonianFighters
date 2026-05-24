// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#include "TeamSubsystem.h"
#include "Combatant/Combatant_Base.h"

/**
 * Registers a combatant into the team registry.
 *
 * If the team does not exist yet, it will be created automatically.
 */
void UTeamSubsystem::RegisterUnitToTeam(ACombatant_Base* Unit, int32 TeamNumber)
{
	if (!Unit || !TeamNumber)
	{
		return;
	}

	FTeam& Team = TeamRegistry.FindOrAdd(TeamNumber);

	if (!Team.Combatants.Contains(Unit))
	{
		Team.Combatants.Add(Unit);
	}
}
