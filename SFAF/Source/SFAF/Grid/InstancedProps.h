// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.

#pragma once

#include "CoreMinimal.h"
#include "GridMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GridSnapComponent.h"
#include "InstancedProps.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SFAF_API AInstancedProps : public AActor
{
	GENERATED_BODY()
    
public: 
	/** Default constructor */
	AInstancedProps();
	
	/** Applies the configured TopColor to the preview mesh material */
	void SetTopColor() const;

	/** Called when the actor is spawned or moved in the editor */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

private:
	/** Collision volume for the instanced prop */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Volume;

	/** Visual representation of the prop in the editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PreviewMesh;


public:

	
	/** Component responsible for snapping the prop to the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridSnapComponent> GridSnapComponent;
	
	/** Reference to the actor defining the grid origin for snapping */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	TObjectPtr<AActor> GridOriginActor;
	
	/** Component responsible for snapping the prop to the grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FLinearColor TopColor = FLinearColor::Green;
};
