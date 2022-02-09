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
	SE_Move UMETA(DisplayName = "Move"),
	SE_End UMETA(DisplayName = "End"),
	SE_Death UMETA(DisplayName = "Death")

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

	// Animation
	UAnimInstance* animInstance;
	virtual void PostInitializeComponents() override;
	   	 
public:

	ASquadCharacter();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere , Category = "Status")
	float LifePoint;

	UPROPERTY(EditAnywhere, Category = "Status")
	float Damage;

	void Characterdeath();
	bool IsDeath = false;

	int32 MaxLifePoint;

	/** Called for side to side input */
	void MoveRight(float Val);
	

public:
	
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetLifePointPercent() const;

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

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY()
	class UGameplayStatics* GameStatic;

	UPROPERTY()
	class UParticleSystem* FireParticle;

	void Test();

	//// 1 20 ////

	class AGrid* pGridOnCharacter;
};
