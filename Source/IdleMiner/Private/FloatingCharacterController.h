// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FloatingCharacterController.generated.h"

UCLASS()
class AFloatingCharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	AFloatingCharacterController();

	UPROPERTY(EditAnywhere, Category = "FCC")
	UCameraComponent* Camera;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCC")
	float CameraSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCC")
	float CameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCC")
	float MaxCameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCC")
	float MinCameraDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCC")
	bool HasteIsPressed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveNorth(float Amount);
	void MoveEast(float Amount);

	void ZoomIn(float Amount);

	void HastePressed();
	void HasteReleased();
};
