// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Core/Combat/Tactical/TacticalManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Grid/GridType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATacticalManager::ATacticalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	SelectMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMesh"));
	SelectMesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ATacticalManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetGrid(Cast<AGridType>(UGameplayStatics::GetActorOfClass(GetWorld(),AGridType::StaticClass())));
}

// Called every frame
void ATacticalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacticalManager::SetGrid(AGridType* GridType)
{
	Grid = GridType;
}

AGridType* ATacticalManager::GetGrid()
{
	return Grid;
}

