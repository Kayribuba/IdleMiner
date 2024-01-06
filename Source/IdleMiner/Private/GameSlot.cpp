// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSlot.h"

// Sets default values
AGameSlot::AGameSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(50, 50, 2));
	Box->SetCollisionResponseToAllChannels(ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);
	Plane->SetStaticMesh(DefaultSlotMesh.Object);

	SetState(GS_Default);
}

void AGameSlot::SetState(EGridState NewState)
{
	GridState = NewState;

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> DefaultSlotMaterial(TEXT("'/Game/Materials/MI_GameSlot_Normal.MI_GameSlot_Normal'"));
	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> DefaultSlotMaterial_H(TEXT("'/Game/Materials/MI_GameSlot_Highlighted.MI_GameSlot_Highlighted'"));
	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> DefaultSlotMaterial_O(TEXT("'/Game/Materials/MI_GameSlot_Highlighted.MI_GameSlot_Highlighted'"));

	switch (NewState)
	{
	case GS_Default:
		Plane->SetMaterial(0, NormalMaterial);
		break;
	case GS_Highlighted:
		Plane->SetMaterial(0, HighlightedMaterial);
		break;
	case GS_Occupied:
		Plane->SetMaterial(0, OccupiedMaterial);
		break;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("%i"), (int)NewState));
}

EGridState AGameSlot::GetState()
{
	return GridState;
}
