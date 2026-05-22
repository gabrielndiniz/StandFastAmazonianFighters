// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridSnapComponent.generated.h"

class AGridType;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SFAF_API UGridSnapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridSnapComponent();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	void SetGrid(AGridType* Grid);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	AGridType* GetGrid() const;
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Grid")
	FVector GetSnappedLocation(FVector WorldLocation) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector TileSize = FVector(400.f, 350.f, 50.f);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	AGridType* GridOriginActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	float ZAlteration = 0.f;
};
