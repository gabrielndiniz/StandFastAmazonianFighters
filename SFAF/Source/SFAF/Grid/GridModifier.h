// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GridModifierTypes.h"
#include "GameFramework/Actor.h"
#include "GridTacticalTypes.h"
#include "GridModifier.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SFAF_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Volume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTag TileTypeTag;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 CostValue = 0;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 Priority = 0;

	bool AffectsPosition(const FVector& WorldPos) const;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	FGameplayTagContainer AdditionalTags;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetPreviewVisible(bool bVisible);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void ApplyToTile(FGameplayTag& InOutTileType, int32& InOutCost, FGameplayTagContainer& InOutTags) const;
	
	UPROPERTY(EditAnywhere)
	EGridModifierMode CostMode = EGridModifierMode::Override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
