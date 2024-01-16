// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
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
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed));
}

// Called to bind functionality to input
void AFloatingCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveNorth", this, &AFloatingCharacterController::MoveNorth);
	PlayerInputComponent->BindAxis("MoveEast", this, &AFloatingCharacterController::MoveEast);

	PlayerInputComponent->BindAxis("CloseDistance", this, &AFloatingCharacterController::ZoomIn);

	PlayerInputComponent->BindAction("Haste", IE_Pressed, this, &AFloatingCharacterController::HastePressed);
	PlayerInputComponent->BindAction("Haste", IE_Released, this, &AFloatingCharacterController::HasteReleased);

	PlayerInputComponent->BindAction("DrillSelect", IE_Pressed, this, &AFloatingCharacterController::SelectDrill);
	PlayerInputComponent->BindAction("FactorySelect", IE_Pressed, this, &AFloatingCharacterController::SelectFactory);
	PlayerInputComponent->BindAction("StoreSelect", IE_Pressed, this, &AFloatingCharacterController::SelectStore);
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
	if (Amount == 0) return;

	CameraDistance = FMath::Clamp(GetActorLocation().operator+(FVector(0, 0, Amount * 50)).Z, MaxCameraDistance, MinCameraDistance);
	
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CameraDistance), false);
	

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), GetActorLocation().Z));

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%f"), CameraSpeed));

	CameraSpeed = CameraDistance;
	GetCharacterMovement()->MaxWalkSpeed = CameraSpeed;


}


void AFloatingCharacterController::HastePressed()
{
	CameraSpeed *= 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = CameraSpeed;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed));

}

void AFloatingCharacterController::HasteReleased()
{
	CameraSpeed /= 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = CameraSpeed;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed));

}

void AFloatingCharacterController::SelectDrill()
{
	AGameManager::Instance->ChangeSelectedBuilding(EBuilding::B_DrillBasic);
}

void AFloatingCharacterController::SelectFactory()
{
	AGameManager::Instance->ChangeSelectedBuilding(EBuilding::B_FactoryCopperWire);
}

void AFloatingCharacterController::SelectStore()
{
	AGameManager::Instance->ChangeSelectedBuilding(EBuilding::B_StoreHardware);
}

