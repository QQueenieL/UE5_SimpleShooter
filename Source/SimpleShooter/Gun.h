// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_AssaultRifle UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Pistol UMETA(DisplayName = "PISTOL"),
	E_Shotgun UMETA(DisplayName = "SHOTGUN")
};

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PullTrigger();

	void CheckAmmo(EWeaponType _WeaponType);
	//Reload the current weapon
	void ReloadWeapon(EWeaponType _WeaponType);
	//Calculate the ammo in the weapon's clip and on the player
	int CalculateAmmo(int _AmmoAmount);

	//The total amount of ammo that can be carried for the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int maxTotalAmmo;

	//The total amount of ammo that can be in the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int maxClipAmmo;

	//The total amount of ammo being carried for the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int totalAmmo;

	//The amount of ammo in the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int clipAmmo;

	//The time it takes to reload the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float reloadTime;

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void TriggerdOutOfAmmoPopUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType WeaponType;

	//Add ammo to the player's correct ammo type
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType AmmoType, int AmmoAmount);

	//Switches the weapon the character was using to the new weapon they are using
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SwitchWeaponMesh(int index);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;
	
	//The amount of Assult Rifle ammo the player currently has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int assaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int pistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int shotgunAmmo;
	//Add ammo to the player's correct ammo type
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType AmmoType, int AmmoAmount);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	//UPROPERTY(VisibleAnywhere)
	//USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UParticleSystem *ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase *ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult &Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
};
