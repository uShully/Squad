﻿// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SquadCharacter.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "StatusBarWidget.h"
#include "SquadController.h"
#include "CharacterAnimInstance.h"
#include "FloatingTextActor.h"
#include "CursorHighlight.h"
#include "SquadCharacterStatComponent.h"
#include "SquadAIController.h"
#include "Engine/Blueprint.h"
#include "Engine/Engine.h"
#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASquadCharacter::ASquadCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 32.f));

	LifeBar = CreateDefaultSubobject<UWidgetComponent>(FName("LifeBar Widget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> StatusBarClassFinder(TEXT("WidgetBlueprint'/Game/UI/StatusBar_PlayerCharacter.StatusBar_PlayerCharacter_C'")); // Change the path to your file path, or just add the widget class on the Blueprint
	LifeBar->SetWidgetClass(StatusBarClassFinder.Class);
	LifeBar->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MS(L"SkeletalMesh'/Game/Characters/Modular_soldier_01/Meshes/SM_Modular_soldier_14.SM_Modular_soldier_14'");
	if (MS.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MS.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/MilitaryWeapDark/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Blood(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Gameplay/Player/P_body_bullet_impact.P_body_bullet_impact'"));
	if (Blood.Succeeded())
	{
		BloodParticle = Blood.Object;
	}

	// 능력치 컴포넌트
	CharacterStat = CreateDefaultSubobject<USquadCharacterStatComponent>(TEXT("CharacterStat"));

}

void ASquadCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
}

void ASquadCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(-1.f,0.f,0.f), Value);
	
}

void ASquadCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	Character_Rotator_StartRotator = this->GetActorRotation();

	MaxLifePoint = LifePoint;

	LifeBar->InitWidget();
	   
	auto StatusBarUserWidget = GetStatustBarWidget();
	if (!StatusBarUserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to Cast to UHealthBarWidget"), *GetOwner()->GetName());
		return;
	}

	StatusBarUserWidget->GetHealthBar()->PercentDelegate.BindUFunction(this, FName("GetLifePointPercent"));
	StatusBarUserWidget->GetAmmoBar()->PercentDelegate.BindUFunction(this, FName("GetAmmoPercent"));

	LifeBar->SetRelativeLocation(FVector(XPos_HPBar, YPos_HPBar, ZPos_HPBar));

	StatusBarUserWidget->SetBarRenderOpacity(1.0f);
}

AGrid* ASquadCharacter::GetUnderGrid()
{
	return UnderGrid;
}

void ASquadCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	animInstance = GetMesh()->GetAnimInstance();


}

// 데미지를 입었을때 발동
float ASquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if(ActualDamage >= 0)
		LifePoint -= ActualDamage;
		

	TakenDamage = Damage;
	SpawnDamageUI();

	return ActualDamage;
}

void ASquadCharacter::SpawnDamageUI()
{
	UWorld* world = GetWorld();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FRotator rotator = GetActorRotation();
	FVector SpawnLocation = GetActorLocation();

	AFloatingTextActor* ActorRef = world->SpawnActor<AFloatingTextActor>(FloatingTextActorBP , SpawnLocation, rotator, SpawnParams );
	
}

void ASquadCharacter::SpawnBullet(FVector SocketLoc)
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation();

	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ProjectileBulletClass, SocketLoc, SpawnRotation);
	Bullet->SetBulletOwner(this);
	FVector NewVelocity = GetActorForwardVector() * Bullet->Velocity;

	Bullet->ProjectileMovementComponent->Velocity = FVector(NewVelocity);
}

float ASquadCharacter::GetLifePointPercent() const
{
	float result = LifePoint / MaxLifePoint;

	return result;
}


float ASquadCharacter::GetAmmoPercent() const
{
	float result = CurrentAmmo / MaxAmmo;

	return result;
}

void ASquadCharacter::Characterdeath()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StateEnum = EStateEnum::SE_Death;

	IsDeath = true;
}

void ASquadCharacter::SetGridOn()
{
	IsGridOn = true;

	UnderGrid->SetActorHiddenInGame(false);
}

void ASquadCharacter::SetGridOff()
{
	IsGridOn = true;

	UnderGrid->SetActorHiddenInGame(true);
}

void ASquadCharacter::SetGridColor(FColor Color)
{
	if(Color == FColor::Red) UnderGrid->SetGridInfo_Material_Red();
	else if (Color == FColor::Blue) UnderGrid->SetGridInfo_Material_Blue();
	else if (Color == FColor::Green) UnderGrid->SetGridInfo_Material_Green();
}

void ASquadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ASquadCharacter::Change_Character_RotatorToStartBattle()
{
	SetActorRelativeRotation(Character_Rotator_StartRotator);
	
	//Character_BattleRotator_Start = false;
}

void ASquadCharacter::CharacterBuff_Init()
{
	int32 Buff_Accrancy = 0;
	int32 Buff_Evasion = 0;

	int32 Debuff_Accrancy = 0;
	int32 Debuff_Evasion = 0;
}	

int32 ASquadCharacter::Get_Calc_BuffAccrancy()
{
	return Buff_Accrancy + Debuff_Accrancy;
}

void ASquadCharacter::Set_Buff_Accrancy(int32 InAccrancy)
{
	Buff_Accrancy += InAccrancy;
}
void ASquadCharacter::Set_Buff_Evasion(int32 InEvasion)
{
	Buff_Evasion += InEvasion;
}
void ASquadCharacter::Set_DeBuff_Accrancy(int32 InAccrancy)
{
	Debuff_Accrancy += InAccrancy;
}
void ASquadCharacter::Set_DeBuff_Evasion(int32 InEvasion)
{
	Debuff_Evasion += InEvasion;
}


int32 ASquadCharacter::Get_Calc_BuffEvasion()
{
	return Buff_Evasion + Debuff_Evasion;
}

class UStatusBarWidget* ASquadCharacter::GetStatustBarWidget()
{
	auto StatusBarUserWidget = Cast<UStatusBarWidget>(LifeBar->GetUserWidgetObject());

	return StatusBarUserWidget;
}

void ASquadCharacter::Add_CCArray(FCCStatus CastCC)
{
	if (CastCC.Skill_Count > 0) {
		CCArray.Add(CastCC);

		if(CastCC.GetUseCharacterData() != CastCC.GetTargetCharacterData()) {
			this->Evasion += CastCC.Skill_TargetAvoidanceRateCorrectionValue;
			this->Accuracy += CastCC.Skill_TargetAccurancyRateCorretionValue;
		}
		else {
			this->Evasion += CastCC.Skill_UsedCharacterEvasionCorrectionValue;
			this->Defense += CastCC.Skill_UsedCharacterDefensiveCorrectionValue;
		}

		if (CastCC.Skill_Stun == true) {
			this->IsStun = true;
		}
	}
}



void ASquadCharacter::Control_CCArray()
{
	if(CCArray.Num() > 0) {
		for (int32 i = 0; i < CCArray.Num(); i++) {
			if (CCArray[i].Skill_Count > 0) { 
		
				if (CCArray[i].IsCCActive == false) {

				}
				// 액티브가 false 이면 효과 적용 후 액티브를 true	
				CCArray[i].Skill_Count -= 1;
			}
			else if (CCArray[i].Skill_Count == 0){

				if (CCArray[i].GetUseCharacterData() != CCArray[i].GetTargetCharacterData()) {
					this->Evasion -= CCArray[i].Skill_TargetAvoidanceRateCorrectionValue;
					this->Accuracy -= CCArray[i].Skill_TargetAccurancyRateCorretionValue;
				}
				else {
					this->Evasion -= CCArray[i].Skill_UsedCharacterEvasionCorrectionValue;
					this->Defense -= CCArray[i].Skill_UsedCharacterDefensiveCorrectionValue;
				}

				if (CCArray[i].Skill_Stun == true) {
					this->IsStun = false;
				}
				CCArray.RemoveAt(i);
				i--;
			}
		}
	}
}

void ASquadCharacter::Clear_CCArray()
{
	for (int32 i = 0 ; i < CCArray.Num(); i++) {
		if (CCArray[i].GetUseCharacterData() != CCArray[i].GetTargetCharacterData()) {
			this->Evasion -= CCArray[i].Skill_TargetAvoidanceRateCorrectionValue;
			this->Accuracy -= CCArray[i].Skill_TargetAccurancyRateCorretionValue;
		}
		else {
			this->Evasion -= CCArray[i].Skill_UsedCharacterEvasionCorrectionValue;
			this->Defense -= CCArray[i].Skill_UsedCharacterDefensiveCorrectionValue;
		}

		if (CCArray[i].Skill_Stun == true) {
			this->IsStun = false;
		}
		CCArray.RemoveAt(i);
		i--;
	}
}