// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UCharacterAnimInstance();
	
	// virtual void NativeBeginPlay() override;
	// virtual void NativeUpdateAnimation(float DeltaSeconds);

public:

	// Shot
	UFUNCTION()
	void BeShot();

	UAnimMontage* shotMontage;

	// Hit
	void Hit();
	UAnimMontage* gethitMontage;

	// Death
	UFUNCTION()
	void Death();
	UAnimMontage* deathMontage;

	// Aiming
	UFUNCTION()
	void Aiming();
	UAnimMontage* aimingMontage;

	UFUNCTION()
	void BeCover();
	UAnimMontage* coverMontage;

	UFUNCTION()
		void Enemy_BeShot();
	UAnimMontage* Enemy_shotMontage;
	UFUNCTION()
		void Enemy_BeCover();
	UAnimMontage* Enemy_BeCoverMontage;
	UFUNCTION()
		void Enemy_Hit();
	UAnimMontage* Enemy_HitMontage;
	UFUNCTION()
		void Enemy_Death();
	UAnimMontage* Enemy_DeathMontage;
	UFUNCTION()
		void Enemy_Aiming();
	UAnimMontage* Enemy_AimingMontage;


	void StopMontage();

};
