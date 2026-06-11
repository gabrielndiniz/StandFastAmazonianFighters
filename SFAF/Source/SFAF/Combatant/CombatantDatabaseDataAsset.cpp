// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#include "Combatant/CombatantDatabaseDataAsset.h"

bool UCombatantDatabaseDataAsset::GetCombatantData(FGameplayTag Tag, FCombatantData& OutData) const
{
	if (const FCombatantData* Data = CombatantData.Find(Tag))
	{
		OutData = *Data;
		return true;
	}
	return false;
}

TArray<FGameplayTag> UCombatantDatabaseDataAsset::GetAllCombatantTypeTag()
{
	TArray<FGameplayTag> Result;
	Result.Reserve(CombatantData.Num());

	for (const TPair<FGameplayTag, FCombatantData>& Pair : CombatantData)
	{
		Result.Add(Pair.Key);
	}

	return Result;
}
