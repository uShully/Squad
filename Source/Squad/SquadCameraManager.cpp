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

	ExploreRot = FRotator(-40.f, -180.f, 0.f);
	BattleRot = FRotator(-50.f, -180.f, 0.f);

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
	Control_SetBattleCameraLocation(DeltaTime);
}

void ASquadCameraManager::GetFriendlyChar()
{
	

	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), FriendlyCharList);
	UE_LOG(LogClass, Log, TEXT(" Get Frind num : %d"), FriendlyCharList.Num());
	SortFrindlyCharList();
}


void ASquadCameraManager::SortFrindlyCharList()
{

	for (int32 i = 0 ; i < FriendlyCharList.Num(); i++)
	{
		for (int32 j = 0; j < FriendlyCharList.Num(); j++)
		{

			if (Cast<APlayerSquadCharacter>(FriendlyCharList[j])->BattleLineNumber == i)
			{
				if (i == j)
				{
					Cast<APlayerSquadCharacter>(FriendlyCharList[j])->numbering = i;
				}
				else
				{
					Cast<APlayerSquadCharacter>(FriendlyCharList[j])->numbering = i;
					FriendlyCharList.Swap(j, i);
				}				
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
	PlayerInputComponent->BindAction("Plus", EInputEvent::IE_Released, this, &ASquadCameraManager::PlusTargetArmLeght);
	PlayerInputComponent->BindAction("Minus", EInputEvent::IE_Released, this, &ASquadCameraManager::MinusTargetArmLength);
	PlayerInputComponent->BindAction("PlusRot", EInputEvent::IE_Released, this, &ASquadCameraManager::PlusCameraBoomRotator);
	PlayerInputComponent->BindAction("MinusRot", EInputEvent::IE_Released, this, &ASquadCameraManager::MinusCameraBoomRotator);
	//PlayerInputComponent->BindAction("Temp5F", EInputEvent::IE_Released, this, &ASquadCameraManager::Control_PlayerCharacterMovement);
}

void ASquadCameraManager::PlusTargetArmLeght()
{
	CameraBoom->TargetArmLength += 100.f;
}

void ASquadCameraManager::MinusTargetArmLength()
{
	CameraBoom->TargetArmLength -= 100.f;
}

void ASquadCameraManager::PlusCameraBoomRotator()
{
	FRotator Rot = CameraBoom->GetComponentRotation();
	CameraBoom->AddRelativeRotation(FRotator(-1.f, 0.f, 0.f));
	//FRotator ChangeRot = Rot - FRotator(0.f, 0.f, -1.f);

	//CameraBoom->SetRelativeRotation(ChangeRot);
}

void ASquadCameraManager::MinusCameraBoomRotator()
{
	FRotator Rot = CameraBoom->GetComponentRotation();
	CameraBoom->AddRelativeRotation(FRotator(1.f, 0.f, 0.f));
	//FRotator ChangeRot = Rot - FRotator(0.f, 0.f, 1.f);

	//CameraBoom->SetRelativeRotation(ChangeRot);

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
	ExplorerLocation = FVector(Loc.X + 450.f, GetActorLocation().Y , GetActorLocation().Z);
	FVector tempVector(0.f, 0.f, 50.f);
	BattleLocation = (Loc + tempVector);
}

void ASquadCameraManager::Control_SetBattleCameraLocation(float DeltaTime)
{		
	if(MoveSwitch == true)
	{
		if(IsExploreToBattle == true)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector Interp = FMath::VInterpTo(CurrentLocation, BattleLocation , GetWorld()->GetDeltaSeconds(), 5.0f);
			
			SetActorLocation(Interp);
	
		

			if (CurrentLocation.X < BattleLocation.X + 1.f)
			{
				MoveSwitch = false;
				IsExploreToBattle = false;
			}
		}
		else if (IsBattleToExplore == true)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector Interp = FMath::VInterpTo(CurrentLocation, ExplorerLocation, GetWorld()->GetDeltaSeconds(), 5.0f);

			SetActorLocation(Interp);



			if (CurrentLocation.X > ExplorerLocation.X - 1.f)
			{
				MoveSwitch = false;
				IsBattleToExplore = false;
			}
		}
	}
	
}

void ASquadCameraManager::Control_SetBattleEnd()
{
	SetActorLocation(ExplorerLocation);
}

void ASquadCameraManager::Control_ResultToRun()
{
	for (int32 i = 0; i < FriendlyCharList.Num(); i++)
	{
		Cast<APlayerSquadCharacter>(FriendlyCharList[i])->LifePoint -= 1.0f;
	}
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
		FRotator Interp = FMath::RInterpTo(CameraBoom->RelativeRotation, BattleRot, DeltaTime, 4.0f);
		CameraBoom->SetRelativeRotation(Interp);
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, minTargetArmLength, DeltaTime, 4.0f);
		FRotator Interp = FMath::RInterpTo(CameraBoom->RelativeRotation, ExploreRot, DeltaTime, 4.0f);
		CameraBoom->SetRelativeRotation(Interp);
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

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Camera switch");
}