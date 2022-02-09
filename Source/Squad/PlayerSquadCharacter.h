// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
//#include "EnemySquadCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
//#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "PlayerSquadCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API APlayerSquadCharacter : public ASquadCharacter
{
	GENERATED_BODY()

	APlayerSquadCharacter();

public:

	void SetShotReady();
	void Debug_Shot(ASquadCharacter* Target);

	void SetMoveReady();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetStay();

	void SetCharacterEnd();
	
	void PlaySelectedSound();

	void InputTest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Numbering")
	int32 numbering;

	void SetUnderGrid(class AGrid* Grid);
	class AGrid* GetUnderGrid();

protected:

	// 오디오 정리 필요
	/*
	UAudioComponent* AudioComp;
	UAudioComponent* AudioComp_Hit;
	*/
	USoundBase* Fire_Sound;
	USoundBase* GetHit_Sound;
	USoundBase* Death_Sound;

	//UParticleSystemComponent* ParticleSystem;

	
	USoundBase* Selected_Sound;
	class AGrid* UnderGrid;
};
