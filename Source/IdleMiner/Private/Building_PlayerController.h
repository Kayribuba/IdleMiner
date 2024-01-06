// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BuildingBase.h"
#include "GameManager.h"
#include "Building_PlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ABuilding_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};