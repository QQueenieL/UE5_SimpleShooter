// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_AssaultRifle UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Pistol UMETA(DisplayName = "PISTOL"),
	E_Shotgun UMETA(DisplayName = "SHOTGUN")
};

UCLASS()
class SIMPLESHOOTER_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The total amount of ammo in this pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int AmmoAmount;

	//The type of ammo in this pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EAmmoType AmmoType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
