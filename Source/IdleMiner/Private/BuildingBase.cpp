// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"

// Sets default values
ABuildingBase::ABuildingBase():
	IsUpgraded(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(50, 50, 2));
	Box->SetCollisionResponseToAllChannels(ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Mesh->SetupAttachment(RootComponent);
}

void ABuildingBase::SetActorHidden(bool setTo)
{
	SetActorHiddenInGame(setTo);
	SetActorEnableCollision(!setTo);
	SetActorTickEnabled(!setTo);
}

void ABuildingBase::Upgrade()
{
	if (IsUpgraded) return;
	if (IsNotUpgradable) return;

	for (FSBuildingProcess process : UpgradeAddings)
	{
		GainsPerClock.Add(process);
	}

	IsUpgraded = true;
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

