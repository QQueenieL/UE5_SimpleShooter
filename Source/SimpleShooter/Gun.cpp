// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

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

void AGun::CheckAmmo()
{
	if (GetOwnerController()->IsPlayerController())
	{
		if (clipAmmo > 0)
		{
			clipAmmo -= 1;
			PullTrigger();
		}
		else if (totalAmmo > 0)
		{
			ReloadWeapon();
		}
		else
		{
			TriggerdOutOfAmmoPopUp();
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

void AGun::ReloadWeapon()
{
	if (clipAmmo != maxClipAmmo) //there is room to reload in the weapon
	{
		//carried more than enough ammo to fill the weapon
		if (totalAmmo - (maxClipAmmo - clipAmmo) >= 0) 
		{	
			//fill in the clip with the amount shot
			totalAmmo -= (maxClipAmmo - clipAmmo);
			clipAmmo = maxClipAmmo;
		}
		else //fill in the rest of the carried ammo
		{
			clipAmmo += totalAmmo;
			totalAmmo = 0;
		}
	}
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
