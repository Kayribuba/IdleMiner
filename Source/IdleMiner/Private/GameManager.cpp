// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Building_PlayerController.h"

AGameManager* AGameManager::Instance = nullptr;

// Sets default values
AGameManager::AGameManager() : 
	WasM0Pressed(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResourceCounts.Empty();
	ResourceCounts.Add(EResource::R_Coin, 0);
	ResourceCounts.Add(EResource::R_Copper, 0);
	ResourceCounts.Add(EResource::R_Iron, 0);
	ResourceCounts.Add(EResource::R_Gold, 0);


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BuildingPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingPreview"));
	BuildingPreviewMesh->SetupAttachment(RootComponent);
	BuildingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	GridIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridIndicator"));
	GridIndicatorMesh->SetupAttachment(RootComponent);
	GridIndicatorMesh->SetStaticMesh(DefaultSlotMesh.Object);
	GridIndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

		GridIndicatorMesh->SetMaterial(0, GhostMaterial);
	}
	else CurrentBuilding = nullptr;

	GatherResources();

	ResourceCounts[EResource::R_Copper] = 10;
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

	if (IsPressed && WasM0Pressed == false)
	{
		FSGridPosition gridPos = FSGridPosition::GetPositionInGrid(GetActorLocation(), SnappedLocation, GridSize);

		bool canPlace = true;
		for (FSPlacedBuilding building : PlacedBuildings)
		{
			if (FSGridPosition::Compare(building.Position, gridPos))
			{
				canPlace = false;
				break;
			}
		}

		if (canPlace)//check build cost if still can place
		{
			for (FSBuildingProcess process : CurrentBuilding.GetDefaultObject()->BuildCosts)
			{
				if (DoesHaveResources(process) == false)
				{
					canPlace = false;
					break;
				}
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%ix%i %s"), gridPos.XPos, gridPos.YPos, canPlace ? TEXT("NOT Occupied") : TEXT("IS Occupied")));

		if (canPlace)
		{
			for (FSBuildingProcess process : CurrentBuilding.GetDefaultObject()->BuildCosts)
			{
				TryRemoveResources(process);
			}

			FActorSpawnParameters SpawnInfo;
			FVector SpawnLocation = SnappedLocation;
			FRotator SpawnRotation = FRotator(0, 0, 0);

			ABuildingBase* SpawnedRef = GetWorld()->SpawnActor<ABuildingBase>(CurrentBuilding, SpawnLocation, SpawnRotation, SpawnInfo);
			PlacedBuildings.Add(FSPlacedBuilding(SpawnedRef, gridPos));
		}
	}

	WasM0Pressed = IsPressed;

	//DrawDebugPoint(GetWorld(), Location, 100, FColor::Red);
}

void AGameManager::GatherResources()
{
	/*int CoinGain = PlacedBuildings.Num();
	Count_Coin += CoinGain;*/

	for (FSPlacedBuilding placedB : PlacedBuildings)
	{
		FString debugMessage = TEXT("");
		ABuildingBase* building = placedB.Building;

		bool doesHaveAllResources = true;
		for (FSBuildingProcess process : building->NeedsPerClock)
		{
			if (DoesHaveResources(process) == false)
			{
				doesHaveAllResources = false;
				break;
			}
		}

		if (doesHaveAllResources == false) continue;

		for (FSBuildingProcess process : building->NeedsPerClock)
		{
			TryRemoveResources(process);
		}

		for (FSBuildingProcess process : building->GainsPerClock)
		{
			AddResources(process);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, .8f, FColor::White, TEXT("Gathering resources..."));
	GWorld->GetTimerManager().SetTimer(GatherHandle, this, &AGameManager::GatherResources, ResourceGatherSpeed, false);
}

bool AGameManager::DoesHaveResources(FSBuildingProcess process)
{
	if (process.Type == EResource::R_None) return true;
	if (ResourceCounts.Contains(process.Type) == false) return false;

	return ResourceCounts[process.Type] >= process.Count;
}

bool AGameManager::TryRemoveResources(FSBuildingProcess process)
{
	if (process.Type == EResource::R_None) return true;
	if (DoesHaveResources(process) == false) return false;

	ResourceCounts[process.Type] -= process.Count;
	return true;
}

void AGameManager::AddResources(FSBuildingProcess process)
{
	if (ResourceCounts.Contains(process.Type))
	{
		ResourceCounts[process.Type] += process.Count;
	}
}