// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCharacterController.h"

// Sets default values
AFloatingCharacterController::AFloatingCharacterController() :
	CameraSpeed(500.0f),
	CameraDistance(500.0f),
	MaxCameraDistance(500.0f),
	MinCameraDistance(1500.0f)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AddLocalRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Camera->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AFloatingCharacterController::BeginPlay()
{
	Super::BeginPlay();

	

	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->MaxWalkSpeed = CameraSpeed;
	SetActorLocation(FVector(0.0f, 0.0f, CameraDistance), false);

	GetCharacterMovement()->MaxAcceleration = 100000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 10000.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 100000.0f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 5000;
	
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

	PlayerInputComponent->BindAxis("MoveNorth", this, &AFloatingCharacterController::MoveNorth);
	PlayerInputComponent->BindAxis("MoveEast", this, &AFloatingCharacterController::MoveEast);

	PlayerInputComponent->BindAxis("CloseDistance", this, &AFloatingCharacterController::ZoomIn);

	PlayerInputComponent->BindAction("Haste", IE_Released, this, &AFloatingCharacterController::HastePressed);
	PlayerInputComponent->BindAction("Haste", IE_Released, this, &AFloatingCharacterController::HasteReleased);
}

void AFloatingCharacterController::MoveNorth(float Amount)
{
	AddMovementInput(FVector(1, 0, 0), Amount);
}

void AFloatingCharacterController::MoveEast(float Amount)
{
	AddMovementInput(FVector(0, 1, 0), Amount);
}

void AFloatingCharacterController::ZoomIn(float Amount)
{
	SetActorLocation(GetActorLocation().operator+(FVector(0,0, Amount*50)), false);
	
	//FMath::Clamp(GetActorLocation().ZAxisVector, MinCameraDistance, MaxCameraDistance);
	//FMath::Clamp(MyFloat, 0.0f, 100.0f)
}


void AFloatingCharacterController::HastePressed()
{
	CameraSpeed *= 1.5f;
}

void AFloatingCharacterController::HasteReleased()
{
	CameraSpeed /= 1.5f;
}

