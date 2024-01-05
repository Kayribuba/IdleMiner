// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCharacterController.h"

// Sets default values
AFloatingCharacterController::AFloatingCharacterController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloatingCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFloatingCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

