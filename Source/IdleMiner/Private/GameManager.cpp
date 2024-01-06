// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Building_PlayerController.h"

AGameManager* AGameManager::Instance = nullptr;

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BuildingPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingPreview"));
	BuildingPreviewMesh->SetupAttachment(RootComponent);
	BuildingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	GridIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridIndicator"));
	GridIndicatorMesh->SetupAttachment(RootComponent);
	GridIndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GridIndicatorMesh->SetStaticMesh(DefaultSlotMesh.Object);

	Instance = this;

	//OldGameSlot = nullptr;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (UsableBuildings.Num() > 0)
	{
		CurrentBuilding = UsableBuildings[0];
		UStaticMesh* tempMesh = CurrentBuilding.GetDefaultObject()->Mesh->GetStaticMesh();
		BuildingPreviewMesh->SetStaticMesh(tempMesh);
		BuildingPreviewMesh->SetMaterial(0, GhostMaterial);
	}
	else CurrentBuilding = nullptr;

	GatherResources();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::SendMouseTrace(AActor* HitActor, FVector& Location, bool IsPressed)
{
	FVector TargetLocation = FVector(Location.X, Location.Y, GetActorLocation().Z);

	FVector SnappedLocation = TargetLocation;
	SnappedLocation.X = FMath::GridSnap(SnappedLocation.X, GridSize);
	SnappedLocation.Y = FMath::GridSnap(SnappedLocation.Y, GridSize);

	if (CurrentBuilding != nullptr)
	{
		BuildingPreviewMesh->SetWorldLocation(TargetLocation);
	}
	GridIndicatorMesh->SetWorldLocation(SnappedLocation);

	if (IsPressed)
	{
		FSGridPosition gridPos = FSGridPosition::GetPositionInGrid(GetActorLocation(), SnappedLocation, GridSize);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%ix%i"), gridPos.XPos, gridPos.YPos));

		bool canPlace = false;
		for (FSPlacedBuilding building : PlacedBuildings)
		{
			//building.Position
		}
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
