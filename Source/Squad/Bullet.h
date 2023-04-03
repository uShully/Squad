 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SQUAD_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FVector Velocity = FVector(100.f);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* BulletOwner;



public:
	UFUNCTION()
		void SetBulletOwner(AActor* pBulletOwner);

	UFUNCTION(BlueprintCallable)
		AActor* GetBulletOwner();

	UFUNCTION(BlueprintCallable)
	void SetHit(AActor * HitCharacter);

};
