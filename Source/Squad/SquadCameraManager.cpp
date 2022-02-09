// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "PlayerSquadCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SquadGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SquadAIController.h"
#include "Engine/Engine.h"

ASquadCameraManager::ASquadCameraManager()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteLocation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("UnitSet"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 5.f));
	BoxColiision->SetCollisionProfileName("UnitSet");
	BoxColiision->SetupAttachment(RootComponent);
	BoxColiision->RelativeLocation = FVector(-720.0f, 0.f, 0.f);
}

void ASquadCameraManager::BeginPlay()
{
	Super::BeginPlay();

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SCMIns = this;

	GetFriendlyChar();
	
}

void ASquadCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeArmLeght(DeltaTime);
}

void ASquadCameraManager::GetFriendlyChar()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), FriendlyCharList);

	SortFrindlyCharList();
}


void ASquadCameraManager::SortFrindlyCharList()
{
	for (int32 i = 0 ; i < FriendlyCharList.Num(); i++)
	{
		FString tempSting = FString::FromInt(i);
				
		for (int32 j = 0; j < FriendlyCharList.Num(); j++)
		{

			if (FriendlyCharList[j]->ActorHasTag(*tempSting))
			{
				FriendlyCharList.Swap(j, i);
				Cast<APlayerSquadCharacter>(FriendlyCharList[j])->numbering = i;
				break;
			}		
		}		
	}
}

void ASquadCameraManager::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ASquadCameraManager::MoveRight);

	// ex)BindAction 
	PlayerInputComponent->BindAction("Etc", EInputEvent::IE_Released, this, &ASquadCameraManager::zoomswitch);
	//PlayerInputComponent->BindAction("Temp5F", EInputEvent::IE_Released, this, &ASquadCameraManager::Control_PlayerCharacterMovement);
}

void ASquadCameraManager::MoveRight(float Value)
{
	// add movement in that direction
	if(!ControlValue_CameraMovement)
	AddMovementInput(FVector(-1.f,  0.f, 0.f), Value);
	
	
	//MoveChar(Value);
}

void ASquadCameraManager::Control_PlayerCharacterMovement(bool TorF)
{
	// true - 캐릭터가 카메라에 따라 움직임, false - 캐릭터가 카메라 이동에 움직이지 않음
	if (TorF == true)
		ControlValue_PlayerCharacterMovement = true;
	else if(TorF != true)
		ControlValue_PlayerCharacterMovement = false;
}

void ASquadCameraManager::Control_CameraMovement()
{
	if (ControlValue_CameraMovement)
		ControlValue_CameraMovement = false;
	else
		ControlValue_CameraMovement = true;
}

void ASquadCameraManager::Control_SetBattleInit(FVector Loc)
{
	FVector tempVector(0.f, 0.f, 50.f);
	SetActorLocation(Loc + tempVector);

}


void ASquadCameraManager::MoveChar(float Value)
{

	/*
	for (int32 i = 0; i < FriendlyCharList.Num(); i++)
	{
		Cast<ASquadCharacter>(FriendlyCharList[i])->MoveRight(Value);
	}
	*/
	for(int32 i = 0 ; 0 < FriendlyCharList.Num(); i++)
	{ 
		ASquadAIController* AIControl = Cast<ASquadAIController>(Cast<ASquadCharacter>(FriendlyCharList[i])->GetController());
		AIControl->MoveToLocation(BoxColiision->GetComponentLocation() + (0.f , 100.f * i, 0.f));
	}
}

void ASquadCameraManager::ChangeArmLeght(float DeltaTime)
{
	if (ZoomBool)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, MaxTargetArmLength, DeltaTime, 4.0f);
		
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, minTargetArmLength, DeltaTime, 4.0f);
		
	}
}

// ���ǽ� ���⶧���� ���� �� �ڿ� ����

void ASquadCameraManager::zoomswitch()
{
	if (!ZoomBool)
	{		
		ZoomBool = true;
	}
	else
	{	
		ZoomBool = false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Camera switch");
}