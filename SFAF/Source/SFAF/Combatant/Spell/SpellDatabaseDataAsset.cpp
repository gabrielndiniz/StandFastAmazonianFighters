// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "SpellDatabaseDataAsset.h"

bool USpellDatabaseDataAsset::GetSpellData(FGameplayTag Tag, FSpellData& OutData) const
{
	if (const FSpellData* DataPtr = SpellData.Find(Tag))
	{
		OutData = *DataPtr;
		return true;
	}
	return false;
}
