// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


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
