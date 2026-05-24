// © 2026 Gabriel Nobile Diniz. All Rights Reserved.This software and its content, including but not limited to code, art, assets, and documentation, are the exclusive property of Gabriel Nóbile Diniz. Unauthorized copying, distribution, adaptation, or other use is prohibited without explicit permission.For inquiries or permission requests, please contact hearnodarkness@gmail.com.


#include "Core/Combat/Tactical/TacticalManager.h"

// Sets default values
ATacticalManager::ATacticalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

}

// Called when the game starts or when spawned
void ATacticalManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATacticalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

