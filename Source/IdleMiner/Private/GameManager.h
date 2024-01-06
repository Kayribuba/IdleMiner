// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.h"
//#include "GameSlot.h"
#include "GameManager.generated.h"


USTRUCT(Blueprintable)
struct FSGridPosition
{
	GENERATED_USTRUCT_BODY();

	FSGridPosition() {}
	FSGridPosition(int xPos, int yPos) : XPos(xPos), YPos(yPos) {}

	UPROPERTY(EditAnywhere)
	int XPos;

	UPROPERTY(EditAnywhere)
	int YPos;

	FVector ToFVector()
	{
		return FVector(XPos, YPos, 0);
	}


	static FSGridPosition GetPositionInGrid(FVector origin, FVector location, float GridSize)
	{
		FVector targetVector = location - origin;
		targetVector.X = FMath::GridSnap(targetVector.X, GridSize);
		targetVector.Y = FMath::GridSnap(targetVector.Y, GridSize);

		return FSGridPosition((int)(targetVector.X / GridSize), (int)(targetVector.Y / GridSize));
	}
	static bool Compare(FSGridPosition first, FSGridPosition second)
	{
		return first.XPos == second.XPos && first.YPos == second.YPos;
	}
};

USTRUCT(Blueprintable)
struct FSPlacedBuilding
{
	GENERATED_USTRUCT_BODY();

	FSPlacedBuilding() {}
	FSPlacedBuilding(ABuildingBase* building, FSGridPosition position) : Building(building), Position(position) {}
	FSPlacedBuilding(ABuildingBase* building, int xPos, int yPos)
	{
		Building = building;
		Position = FSGridPosition(xPos, yPos);
	}

	UPROPERTY(EditAnywhere)
	ABuildingBase* Building;

	UPROPERTY(EditAnywhere)
	FSGridPosition Position;
};

UCLASS()
class AGameManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	TMap<TEnumAsByte<EResource>, int> ResourceCounts;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float GridSize;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABuildingBase>> UsableBuildings;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ABuildingBase> CurrentBuilding;

	UPROPERTY(VisibleAnywhere)
	TArray<FSPlacedBuilding> PlacedBuildings;

	UStaticMeshComponent* BuildingPreviewMesh;

	UStaticMeshComponent* GridIndicatorMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* GhostMaterial;

	UPROPERTY(EditAnywhere)
	float ResourceGatherSpeed;

	void SendMouseTrace(AActor* HitActor, FVector& Location, bool IsPressed);

	void GatherResources();

	bool DoesHaveResources(FSBuildingProcess process);

	void AddResources(FSBuildingProcess process);

	bool TryRemoveResources(FSBuildingProcess process);

	static AGameManager* Instance;

private:

	FTimerHandle GatherHandle;

	bool WasM0Pressed;
};