// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Switch the player's current primary weapon to the next in the chain
	void SwitchToNextPrimaryWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	int GetClipAmmo() const;
	
	UFUNCTION(BlueprintPure)
	int GetMaxClipAmmo() const;

	UFUNCTION(BlueprintPure)
	int GetTotalAmmo() const;

	// UFUNCTION(BlueprintPure)
	// FString GetWeaponType() const;
	
	void Shoot();
	void ManualReload();
	
	//The weapon the player is currently holding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	AGun* Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<AGun*> Weapons;

	//The index of the weapon that the character is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int weaponIndex;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(EWeaponType WeaponType);

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;
};
