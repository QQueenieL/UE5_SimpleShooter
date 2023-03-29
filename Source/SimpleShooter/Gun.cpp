// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Ammo.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	//Set the default values for variables
	maxTotalAmmo = 100;
	maxClipAmmo = 12;
	totalAmmo = 64;
	clipAmmo = 12;
	reloadTime = 1.0f;

	WeaponType = EWeaponType::E_AssaultRifle;
	assaultRifleAmmo = 80;
	pistolAmmo = 30;
	shotgunAmmo = 16;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::CheckAmmo(EWeaponType _WeaponType)
{
	if (GetOwnerController()->IsPlayerController())
	{
		if (clipAmmo > 0)
		{
			clipAmmo -= 1;
			PullTrigger();
		}
		else
		{
			ReloadWeapon(WeaponType);
		}
	}
	else
	{
		PullTrigger();
	}
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleSoundSocket"));
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if(bSuccess)
	{
		//DrawDebugPoint(GetWorld(),Hit.Location, 20, FColor::Red, true);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

}

void AGun::ReloadWeapon(EWeaponType _WeaponType)
{
	switch(_WeaponType)
	{
		case EWeaponType::E_AssaultRifle:
			assaultRifleAmmo = CalculateAmmo(assaultRifleAmmo);
			break;
		case EWeaponType::E_Pistol:
			pistolAmmo = CalculateAmmo(pistolAmmo);
			break;
		case EWeaponType::E_Shotgun:
			shotgunAmmo = CalculateAmmo(shotgunAmmo);
			break;
		default:
			break;
	}
}

int AGun::CalculateAmmo(int _AmmoAmount)
{
	if (clipAmmo != maxClipAmmo) //there is room to reload in the weapon
	{
		//carried more than enough ammo to fill the weapon
		if (_AmmoAmount - (maxClipAmmo - clipAmmo) >= 0) 
		{	
			//fill in the clip with the amount shot
			_AmmoAmount -= (maxClipAmmo - clipAmmo);
			clipAmmo = maxClipAmmo;
		}
		else //fill in the rest of the carried ammo
		{
			clipAmmo += _AmmoAmount;
			_AmmoAmount = 0;
		}
	}
	else
	{
		TriggerdOutOfAmmoPopUp();
	}

	return _AmmoAmount;
}

bool AGun::GunTrace(FHitResult &Hit, FVector& ShotDirection)
{

	//Find the character's controller
	AController* OwnerController = GetOwnerController(); 
	if(OwnerController == nullptr)
	{
		return false;
	}

	//find its viewport location and rotation
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();
	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	//Go to the character
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

void AGun::AddAmmo(EAmmoType AmmoType, int AmmoAmount)
{
	switch (AmmoType)
	{
	case EAmmoType::E_AssaultRifle:
		assaultRifleAmmo += AmmoAmount;
		break;
	case EAmmoType::E_Pistol:
		pistolAmmo += AmmoAmount;
		break;
	case EAmmoType::E_Shotgun:
		shotgunAmmo += AmmoAmount;
		break;
	default:
		break;
	}
}