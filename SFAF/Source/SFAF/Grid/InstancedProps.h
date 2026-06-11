// © 2026 Gabriel Nobile Diniz. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GridMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GridSnapComponent.h"
#include "InstancedProps.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

/**
 * An instanced prop actor that snaps to the hex grid.
 * Used for decorative or interactive props that align with the tactical grid.
 * Can be tinted with a configurable color in the editor.
 */
UCLASS()
class SFAF_API AInstancedProps : public AActor
{
	GENERATED_BODY()
     
public: 
	/** Default constructor for the instanced props actor */
	AInstancedProps();
	
	/**
	 * Applies the configured TopColor to the preview mesh's dynamic material instance.
	 * The color is set via the GreenSubstitute parameter on the mesh material.
	 */
	void SetTopColor() const;

	/** Snaps to grid and applies color when spawned or edited */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	/** Initializes the prop when the game starts */
	virtual void BeginPlay() override;
	/** Updates the prop each frame */
	virtual void Tick(float DeltaTime) override;

private:
	/** Collision volume for spatial queries */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Volume;

	/** Editor preview mesh showing the prop's visual representation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

public:
	/** Component responsible for snapping this prop to the nearest grid tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGridSnapComponent> GridSnapComponent;
	
	/** Reference to the actor defining the grid origin for snapping */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	TObjectPtr<AActor> GridOriginActor;
	
	/** Color applied to the preview mesh via the GreenSubstitute material parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FLinearColor TopColor = FLinearColor::Green;
};
