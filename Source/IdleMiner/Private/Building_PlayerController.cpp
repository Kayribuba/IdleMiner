// Fill out your copyright notice in the Description page of Project Settings.


#include "Building_PlayerController.h"
#include "DrawDebugHelpers.h"

void ABuilding_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}

void ABuilding_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;

	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		if (GWorld->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 50000.0f, ECC_Visibility))
		{
			AActor* HitActor = HitResult.GetActor();

			AGameManager::Instance->SendMouseTrace(HitActor, HitResult.Location, PlayerInput->IsPressed(EKeys::LeftMouseButton));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}
}