// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::SetBulletOwner(AActor* pBulletOwner)
{
	 this->BulletOwner = pBulletOwner;
}

AActor* ABullet::GetBulletOwner()
{
	return BulletOwner;;
}

void ABullet::SetHit(AActor* HitCharacter)
{
	if (BulletOwner != nullptr) {
		if (BulletOwner == Cast<APlayerSquadCharacter>(BulletOwner)) {
			if (HitCharacter == Cast<AEnemySquadCharacter>(HitCharacter))
			{
				if(Cast<AEnemySquadCharacter>(HitCharacter)->StateEnum != EStateEnum::SE_Death)
					Destroy();
			}
		}
		else if (BulletOwner == Cast<AEnemySquadCharacter>(BulletOwner)) {
			if (HitCharacter == Cast<APlayerSquadCharacter>(HitCharacter))
			{
				if (Cast<APlayerSquadCharacter>(HitCharacter)->StateEnum != EStateEnum::SE_Death)
					Destroy();
			}
		}
	}
}