// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ProgressBar.h"
#include "SquadCharacter.generated.h"


UENUM(BlueprintType)
enum class EStateEnum : uint8
{
	SE_Stay UMETA(DisplayName = "Stay"),
	SE_Shot UMETA(DisplayName = "Shot"),
	SE_Skill1 UMETA(DisplayName = "Skill1"),
	SE_Skill2 UMETA(DisplayName = "Skill2"),
	SE_Cover UMETA(DisplayName = "Cover"),
	SE_Reload UMETA(DisplayName = "Reload"),
	SE_End UMETA(DisplayName = "End"),
	SE_Death UMETA(DisplayName = "Death")

};

USTRUCT(BlueprintType)
struct FCCStatus
{
	GENERATED_BODY()

public:

	FCCStatus() : UseCharacter(nullptr) , TargetCharacter(nullptr)  ,IsCCActive(false), Skill_Count(0.f),
		Skill_UsedCharacterAccurancyrateCorrectionValue(0.f),
		Skill_UsedCharacterCriticalCorrectionValue(0.f),
		Skill_TargetAvoidanceRateCorrectionValue(0.f),
		Skill_TargetAccurancyRateCorretionValue(0.f),
		Skill_Stun(false) {}
	
private:
	class ASquadCharacter* UseCharacter;
	class ASquadCharacter* TargetCharacter;
public:
	ASquadCharacter* GetUseCharacterData() { return UseCharacter; }
	ASquadCharacter* GetTargetCharacterData() { return TargetCharacter; }
	
	void SetUseCharacterData(ASquadCharacter* UseCharacterData) { UseCharacter = UseCharacterData; }
	void SetTargetCharacterData(ASquadCharacter* TargetCharacterData) { TargetCharacter = TargetCharacterData; }


	UPROPERTY()
		bool IsCCActive;

	UPROPERTY()
		int32 Skill_Count;
	//1
	UPROPERTY()
		int32 Skill_UsedCharacterAccurancyrateCorrectionValue;

	//2
	UPROPERTY()
		int32 Skill_UsedCharacterCriticalCorrectionValue;

	UPROPERTY()
		int32 Skill_UsedCharacterEvasionCorrectionValue;

	UPROPERTY()
		int32 Skill_UsedCharacterDefensiveCorrectionValue;

	//3
	UPROPERTY()
		int32 Skill_TargetAvoidanceRateCorrectionValue;

	//4
	UPROPERTY()
		int32 Skill_TargetAccurancyRateCorretionValue;

	//5
	UPROPERTY()
		bool Skill_Stun;
	
public:
		
	void ClearStructData() {
		UseCharacter = nullptr;
		TargetCharacter = nullptr;
		IsCCActive = false;
		Skill_Count = 0.f;
		Skill_UsedCharacterAccurancyrateCorrectionValue = 0.f;
		Skill_UsedCharacterCriticalCorrectionValue = 0.f;
		Skill_TargetAvoidanceRateCorrectionValue = 0.f;
		Skill_TargetAccurancyRateCorretionValue = 0.f;
		Skill_Stun = false;
	}

};

UCLASS(config=Game)
class ASquadCharacter : public ACharacter
{
	GENERATED_BODY()

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	*/

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// Animation
	UAnimInstance* animInstance;

	class AGrid* GetUnderGrid();

protected:
	virtual void PostInitializeComponents() override;
	   	 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFloatingTextActor> FloatingTextActorBP;

	class AGrid* UnderGrid;

	USoundBase* Rifle_Shot_Sound;
	USoundBase* Rifle_Reload_Sound;
	USoundBase* Pistol_Shot_Sound;
	USoundBase* Pistol_Reload_Sound;
	USoundBase* Shotgun_Shot_Sound;
	USoundBase* Shotgun_Reload_Sound;
	USoundBase* Sniper_Shot_Sound;
	USoundBase* Sniper_Reload_Sound;

public:

	ASquadCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SpawnDamageUI();
	float TakenDamage;



	void Characterdeath();
	bool IsDeath = false;

	int32 MaxLifePoint;

	/** Called for side to side input */
	void MoveRight(float Val);


public:
	
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetLifePointPercent() const;

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetAmmoPercent() const;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent *LifeBar {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxColiision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStateEnum StateEnum = EStateEnum::SE_Stay;


	/*
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY()
	class UGameplayStatics* GameStatic;

	UPROPERTY()
	class UParticleSystem* FireParticle;

	UPROPERTY()
		class UParticleSystem* BloodParticle;

	void Test();

	//// 1 20 ////

	class AGrid* pGridOnCharacter;

	void SetGridOn();
	void SetGridOff();

	void SetGridColor(FColor Color);

	bool IsGridOn = false;

public:
	// 캐릭터 능력치 // - 삭제 예정

	UPROPERTY(EditAnywhere, Category = "Status")
		float LifePoint;

	UPROPERTY(EditAnywhere, Category = "Status")
		float MaxAmmo = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float CurrentAmmo = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float Damage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float Accuracy = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float Evasion = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float Critical = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float FireCount = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		float Defense = 0.f;

	UPROPERTY(EditAnywhere, Category = "Status")
		bool IsStun = false;

	///////////////////////////////////////////

	UPROPERTY(VisibleAnywhere)
		class USquadCharacterStatComponent* CharacterStat;

	
	UPROPERTY(EditAnywhere, Category = "HPBar")
		float XPos_HPBar = 0.f;
	UPROPERTY(EditAnywhere, Category = "HPBar")
		float YPos_HPBar = 0.f;
	UPROPERTY(EditAnywhere, Category = "HPBar")
		float ZPos_HPBar = 0.f;

	/////

	UFUNCTION()
		class UStatusBarWidget* GetStatustBarWidget();

	public:

		void CharacterBuff_Init();

		void Set_Buff_Accrancy(int32 InAccrncy);
		void Set_Buff_Evasion(int32 InEvasion);
		void Set_DeBuff_Accrancy(int32 InAccrncy);
		void Set_DeBuff_Evasion(int32 InEvasion);

		int32 Get_Calc_BuffAccrancy();
		int32 Get_Calc_BuffEvasion();

	private:

		int32 Buff_Accrancy = 0;
		int32 Buff_Evasion = 0;

		int32 Debuff_Accrancy = 0;
		int32 Debuff_Evasion = 0;

	private:

		TArray<FCCStatus> CCArray;

	public:
		void Add_CCArray(FCCStatus CastCC);
		void Control_CCArray();
		void Calc_CCArray_Data();

		void Clear_CCArray();

	public:
		FRotator Character_Rotator_StartRotator;
		bool Character_BattleRotator_Start = false;
		FVector BattleLocation;

		void Change_Character_RotatorToStartBattle();

		int32 Character_Skill1_Cooldown = 0;
		int32 Character_Skill2_Cooldown = 0;

		UPROPERTY()
			TSubclassOf<class ABullet> ProjectileBulletClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class ABullet> AssaultBulletClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class ABullet> ShotgunBulletClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class ABullet> SniperBulletClass;

		UFUNCTION(BlueprintCallable)
		void SpawnBullet(FVector SocketLoc);
};
