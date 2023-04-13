// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimInstance.h"
#include "SquadCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SQUAD_API USquadCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USquadCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetClassData(int32 ClassNum);
	void SetWeaponData(int32 WeaponNumber);
	void SetBranchStat();
	
	FOnHPIsZeroDelegate OnHPIsZero;

	//// get brunchStat data ////

	float GetCharacterHPValue();
	float GetCharacterAccuracyCorrectionValue();
	float GetCharacterCriticalCorrectionValue();
	float GetCharacterEvasionCorrectionValue();
	FText GetCharacterBrunchName();
	FString GetCharacterWeaponName() { return Character_EquipWeaponName; };

	//// get WeaponStat data //// 

	float GetWeaponMaxFireCount();
	float GetWeaponDamage();
	float GetWeaponFireCount();
	float GetWeaponAccuracy();
	float GetWeaponCritical();
	float GetWeaponAvoidence();

protected:

	struct FWeaponData* CurrentStatData = nullptr;
	struct FBrunchData* CurrentBrunchStatData = nullptr;

	/////////////////// 병과 스테이터스 ////////////////////////////

	UPROPERTY(meta = (AllowPrivateAccess = true))
		int32 Character_BrunchNumber; // Class Number , 병과 번호

	UPROPERTY(meta = (AloowPrivateAccess = true))
		float Character_EquipWeaponNumber;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		FString Character_EquipWeaponName;	

	UPROPERTY(meta = (AllowPrivateAccess = true))
		float Character_AccuracyCorrectionValue;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		float Character_CriticalCorrectionValue;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		float Character_EvasionCorrectionValue;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		float Character_HPValue;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		FText Character_BrunchName;
		

	///////////////////////////////////////////////////////////////

	UPROPERTY()
		TSubclassOf<class UAnimInstance> RifleAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> PistolAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> ShotgunAnimBP;

	UPROPERTY()
		TSubclassOf<class UAnimInstance> SniperAnimBP;


	////////////////// 무기 스테이터스 ////////////////////////////

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponNum;

	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess = true))
	FString WeaponName;

	UPROPERTY(Meta = (AllowPrivateAccess = true))
	FString WeaponContentPath;

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponMaxFireCount; // 탄창량 - 최대 몇번을 공격을 가능하는지

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponDamage; // 공격력

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponFireCount; //탄환량 - 공격 한번에 몇번을 쏘는지

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponAccuracy; // 명중

	UPROPERTY(Meta = (AllowPrivateAccess = true))
		float WeaponCritical; // 치명

	UPROPERTY(Meta = (AllowPriavteAccess = true))
		float WeaponAvoidence;

	///////////////////////////////////////////////////////////////
};
