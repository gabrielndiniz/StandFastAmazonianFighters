// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridCoord.h"
#include "Components/ActorComponent.h"
#include "GridSnapComponent.generated.h"

class AGridType;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UGridSnapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Sets default values for this component's properties */
	UGridSnapComponent();
	
	virtual void BeginPlay() override;

	/** Assigns the target grid actor for snapping calculations */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	void SetGrid(AGridType* Grid);
	
	/** Returns the currently assigned grid actor */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	AGridType* GetGrid() const;
	
	/** Snaps a world-space location to the nearest valid grid position */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	FVector GetSnappedLocation(FVector WorldLocation) const;

	/** Snaps a world-space location to the nearest valid grid position */
	UFUNCTION(CallInEditor, BlueprintPure, Category = "Grid")
	FGridCoord GetSnapCoord() const;
	
	/** Physical dimensions of a single tile used for snap calculations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize = FVector(400.f, 350.f, 50.f);

	/** Grid actor reference used as the snapping origin */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	AGridType* GridOriginActor;
	
	/** Vertical offset applied after snapping */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	float ZAlteration = 0.f;
	
	/** Coord equivalent from the Grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	mutable FGridCoord Coord;
};
