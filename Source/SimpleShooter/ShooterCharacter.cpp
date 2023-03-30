// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Gun.h"
#include "Ammo.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	weaponIndex = 0;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("Weapon-r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Weapons.Add(Gun);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ManualReload);
	PlayerInputComponent->BindAction(TEXT("SwitchPrimary"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchToNextPrimaryWeapon);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	//Weapons[weaponIndex]->PullTrigger();
	Weapons[weaponIndex]->CheckAmmo(Weapons[weaponIndex]->WeaponType);
}

void AShooterCharacter::ManualReload()
{

	Weapons[weaponIndex]->ReloadWeapon(Weapons[weaponIndex]->WeaponType);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// if Health is lower than DamageToApply, take Health b/c Health cannot be negative
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		ASimpleShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

int AShooterCharacter::GetClipAmmo() const
{
	return Weapons[weaponIndex]->clipAmmo;
}

int AShooterCharacter::GetMaxClipAmmo() const
{
	return Weapons[weaponIndex]->maxClipAmmo;
}

// int AShooterCharacter::GetTotalAmmo() const
// {
// 	switch (Weapons[weaponIndex]->WeaponType)
// 	{
// 	case EWeaponType::E_AssaultRifle:
// 		return Weapons[weaponIndex]->Ammo->assaultRifleAmmo;
// 	case EWeaponType::E_Pistol:
// 		return Weapons[weaponIndex]->Ammo->pistolAmmo;
// 	case EWeaponType::E_Shotgun:
// 		return Weapons[weaponIndex]->Ammo->shotgunAmmo;
// 	default:
// 		return 0;
// 	}
// }

void AShooterCharacter::SwitchToNextPrimaryWeapon()
{
	switch(weaponIndex)
	{
		case 0:
			if(Weapons.Num() > 1)
			{
				weaponIndex = 1;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			else
			{
				weaponIndex = 0;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			break;
		case 1:
			if(Weapons.Num() > 2)
			{
				weaponIndex = 2;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			else
			{
				weaponIndex = 0;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			break;
		case 2:
			if(Weapons.Num() > 3)
			{
				weaponIndex = 3;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			else
			{
				weaponIndex = 0;
				Weapons[weaponIndex]->SwitchWeaponMesh(weaponIndex);
			}
			break;
		default:
			break;
	}
}

void AShooterCharacter::AddWeapon(EWeaponType WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponType::E_Pistol:
	{
		Weapons[weaponIndex++]->WeaponType = WeaponType;
		Weapons.Add(Weapons[weaponIndex++]);
		break;
	}

	case EWeaponType::E_Shotgun:
	{
		Weapons[weaponIndex++]->WeaponType = WeaponType;
		Weapons.Add(Weapons[weaponIndex++]);
		break;
	}

	default:
		break;
	}
}