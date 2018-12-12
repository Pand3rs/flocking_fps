// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/world.h"
#include "EngineUtils.h"

AFPSProjectile::AFPSProjectile() 
{
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 100.f;
	ProjectileMovement->MaxSpeed = 300.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 100.0f;
}
/** Sound to play each time we fire */
UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
USoundBase* FireSound;

/** Projectile class to spawn */
UPROPERTY(EditDefaultsOnly, Category = "Projectile")
TSubclassOf<AFPSProjectile> ProjectileClass;

void AFPSProjectile::Tick(float DeltaTime)
{
	UWorld* world = GetWorld();
	FVector avg_pos = { 0,0,0 };
	FVector avg_vel = { 0,0,0 };
	int n_bullets = 0;

	Super::Tick(DeltaTime);

	for (TActorIterator<AFPSProjectile> Itr(world, AFPSProjectile::StaticClass()); Itr; ++Itr)
	{
		if (this == *Itr) continue;
		FVector second_vel = (*Itr)->GetVelocity();

		FVector second_pos = (*Itr)->CollisionComp->GetRelativeTransform().GetLocation();
	
		float distance = AActor::GetDistanceTo(*Itr);

		if (distance < 600)
		{	
			n_bullets++;
			avg_pos += second_pos;
			avg_vel += second_vel;
		}

		if (distance < 100)
		{
			FVector current_loc = CollisionComp->GetRelativeTransform().GetLocation();
			FVector diff = current_loc - second_pos;
			diff.Normalize();
			CollisionComp->AddImpulse(diff * 220);
		}		
	}

	if (n_bullets >= 1)
	{
		avg_pos /= n_bullets;
		avg_vel /= n_bullets;

		FVector current_loc = CollisionComp->GetRelativeTransform().GetLocation();
		FVector diff = avg_pos - current_loc;
		diff.Normalize();

		FVector diff_vel = avg_vel;
		diff_vel.Normalize();

		CollisionComp->AddImpulse(diff * 100);
		CollisionComp->AddImpulse(diff_vel * 20);
	}
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		//Destroy();

		//FVector MuzzleLocation = GetActorLocation();
		//FRotator MuzzleRotation = GetActorRotation();

		//Set Spawn Collision Handling Override
		//FActorSpawnParameters ActorSpawnParams;
		//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		//GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
}