// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
//#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"
#include "EnemySquadCharacter.generated.h"

DECLARE_DELEGATE(FDeleEnemy_Single);

/**
 * 
 */
UCLASS()
class SQUAD_API AEnemySquadCharacter : public ASquadCharacter
{
	GENERATED_BODY()

	AEnemySquadCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	FDeleEnemy_Single Fun_Death;
	void SetBelongToBattleTrigger(class ABattleTrigger* BattleTigger);
	ABattleTrigger* GetBelongToBattleTrigger();

	void Enemy_Shot(AActor* Target);

	void SetUnderGrid(class AGrid* Grid);
	class AGrid* GetUnderGrid();

protected:

	/*
	UAudioComponent* AudioComp;
	UAudioComponent* AudioComp_Hit;
	*/

	USoundBase* Fire_Sound;
	USoundBase* GetHit_Sound;
	USoundBase* Death_Sound;

	void EnemyDeath(class UCharacterAnimInstance* CharAnimInst);

	class ABattleTrigger* BelongToBattleTrigger;

	

	class AGrid* UnderGrid;
public:
	void TestLog();

	FTimerHandle WaitHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetting")
	float WaitTime;

	int32 ArrayNumbering;

	
};
