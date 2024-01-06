// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.h"
#include "GameSlot.h"
#include "GameManager.generated.h"


UENUM(Blueprintable)
enum EResource
{
	R_Coin,
	R_Copper,
	R_Iron,
	R_Gold
};

USTRUCT(Blueprintable)
struct FSBuildingProcess
{
	GENERATED_USTRUCT_BODY();

	FSBuildingProcess() {}
	FSBuildingProcess(TArray< TEnumAsByte<EResource>> gains, TArray< TEnumAsByte<EResource>> needs) : Gains(gains), Needs(needs){}

	UPROPERTY(EditAnywhere)
	TArray< TEnumAsByte<EResource>> Gains;

	UPROPERTY(EditAnywhere)
	TArray< TEnumAsByte<EResource>> Needs;

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
	int Count_Coin;

	UPROPERTY(VisibleAnywhere)
	int Count_Copper;

	UPROPERTY(VisibleAnywhere)
	int Count_Iron;

	UPROPERTY(VisibleAnywhere)
	int Count_Gold;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABuildingBase>> UsableBuildings;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ABuildingBase> CurrentBuilding;

	UPROPERTY(VisibleAnywhere)
	TArray<ABuildingBase*> PlacedBuildings;

	UStaticMeshComponent* DisplayMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* GhostMaterial;

	UPROPERTY(EditAnywhere)
	float ResourceGatherSpeed;

	void SendMouseTrace(AActor* HitActor, FVector& Location, bool IsPressed);

	void GatherResources();

private:

	AGameSlot* OldGameSlot;

	FTimerHandle GatherHandle;
};