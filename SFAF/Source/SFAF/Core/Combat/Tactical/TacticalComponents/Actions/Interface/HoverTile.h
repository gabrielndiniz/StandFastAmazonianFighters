// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Core/Combat/Tactical/TacticalComponents/Actions/BaseActionComponent.h"
#include "HoverTile.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SFAF_API UHoverTile : public UBaseActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHoverTile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual bool Execute_Implementation(const FGridCoord& InSourceCoord, bool bHasHit, 
		const FGridCoord& InTargetCoord) override;
	
	
	/** Set hovered tile*/
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetHoveredTile(FGridCoord NewHoveredTile);
	
	/** Get hovered tile data*/
	FGridTileStaticData* GetHoveredTileData() const;
		
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	FGridCoord HoveredTile;
	
	FGridTileStaticData* HoveredTileData;
	
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	bool bChange = true;
};
