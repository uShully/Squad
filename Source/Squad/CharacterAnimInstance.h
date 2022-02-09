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


};
