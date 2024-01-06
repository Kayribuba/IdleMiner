// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Building_PlayerController.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display"));
	DisplayMesh->SetupAttachment(RootComponent);
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OldGameSlot = nullptr;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (UsableBuildings.Num() > 0)
	{
		CurrentBuilding = UsableBuildings[0];
		UStaticMesh* tempMesh = CurrentBuilding.GetDefaultObject()->Mesh->GetStaticMesh();
		DisplayMesh->SetStaticMesh(tempMesh);
		DisplayMesh->SetMaterial(0, GhostMaterial);
	}
	else CurrentBuilding = nullptr;
	
	if (auto PlayerController = Cast<ABuilding_PlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManagerReference = this;
	}

	GatherResources();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::SendMouseTrace(AActor* HitActor, FVector& Location, bool IsPressed)
{
	if (CurrentBuilding != nullptr)
		DisplayMesh->SetWorldLocation(Location);

	if (auto HitSlot = Cast<AGameSlot>(HitActor))
	{
		if (OldGameSlot == nullptr)
		{
			if (HitSlot->GetState() == EGridState::GS_Default)
				HitSlot->SetState(GS_Highlighted);
		}
		else if (OldGameSlot != HitSlot)
		{
			if (HitSlot->GetState() == EGridState::GS_Default)
				HitSlot->SetState(GS_Highlighted);

			if (OldGameSlot->GetState() == EGridState::GS_Highlighted)
				OldGameSlot->SetState(GS_Default);
		}

		if (IsPressed)
		{
			if (HitSlot->GetState() != EGridState::GS_Occupied)
			{
				FActorSpawnParameters SpawnInfo;
				FVector SpawnLocation = HitSlot->GetActorLocation();
				FRotator SpawnRotation = FRotator(0, 0, 0);

				TSubclassOf<AActor> actor = CurrentBuilding;

				ABuildingBase* SpawnedRef = GetWorld()->SpawnActor<ABuildingBase>(CurrentBuilding, SpawnLocation, SpawnRotation, SpawnInfo);
				PlacedBuildings.Add(SpawnedRef);

				HitSlot->SetState(EGridState::GS_Occupied);
			}
		}

		OldGameSlot = HitSlot;
	}
	else if ((OldGameSlot != nullptr && OldGameSlot->GetState() == EGridState::GS_Highlighted))
	{
		OldGameSlot->SetState(GS_Default);
		OldGameSlot = nullptr;
	}

	//DrawDebugPoint(GetWorld(), Location, 100, FColor::Red);
}

void AGameManager::GatherResources()
{
	int CoinGain = PlacedBuildings.Num();
	Count_Coin += CoinGain;

	GEngine->AddOnScreenDebugMessage(-1, .8f, FColor::White, FString::Printf(TEXT("+%i coin. Total: %i"), CoinGain, Count_Coin));

	GWorld->GetTimerManager().SetTimer(GatherHandle, this, &AGameManager::GatherResources, ResourceGatherSpeed, false);
}
