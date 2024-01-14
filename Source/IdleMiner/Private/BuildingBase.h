// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BuildingBase.generated.h"

UENUM(Blueprintable)
enum EResource
{
	R_None,
	R_Coin,
	R_Copper,
	R_Iron,
	R_Gold
};

USTRUCT(BlueprintType)
struct FSBuildingProcess
{
	GENERATED_USTRUCT_BODY();

	FSBuildingProcess() {}
	FSBuildingProcess(TEnumAsByte<EResource> type, int count) : Type(type)
	{
		Count = FMath::Max(0, count);
	}

	UPROPERTY(EditAnywhere)
	int Count;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EResource> Type;

};

UCLASS()
class ABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABuildingBase();

	UPROPERTY(EditAnywhere)
	bool DisableOnReplaced;

	UPROPERTY(EditAnywhere)
	TArray<FSBuildingProcess> BuildCosts;

	UPROPERTY(EditAnywhere)
	TArray<FSBuildingProcess> GainsPerClock;

	UPROPERTY(EditAnywhere)
	TArray<FSBuildingProcess> NeedsPerClock;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABuildingBase> BuildingToPlaceOver;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	void SetActorHidden(bool setTo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
