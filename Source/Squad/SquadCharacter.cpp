// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SquadCharacter.h"
#include "SquadGameInstance.h"
#include "Grid.h"
#include "StatusBarWidget.h"
#include "SquadController.h"
#include "CharacterAnimInstance.h"
#include "CursorHighlight.h"

ASquadCharacter::ASquadCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 32.f));

	LifeBar = CreateDefaultSubobject<UWidgetComponent>(FName("LifeBar Widget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder(TEXT("WidgetBlueprint'/Game/DevFile/StatusBar.StatusBar_C'")); // Change the path to your file path, or just add the widget class on the Blueprint
	LifeBar->SetWidgetClass(MenuWidgetClassFinder.Class);
	LifeBar->AttachTo(RootComponent);
	
	/*
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	*/

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MS(L"SkeletalMesh'/Game/Characters/Modular_soldier_01/Meshes/SM_Modular_soldier_14.SM_Modular_soldier_14'");
	if (MS.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MS.Object);
	}
	

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Assault_Rifle(L"SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B'");
		if (Assault_Rifle.Succeeded())
		{
			Weapon->SetSkeletalMesh(Assault_Rifle.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/MilitaryWeapDark/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASquadCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	//PlayerInputComponent->BindAxis("MoveRight", this, &ASquadCharacter::MoveRight);

}

void ASquadCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(-1.f,0.f,0.f), Value);
	
}



void ASquadCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaxLifePoint = LifePoint;

	LifeBar->InitWidget();
	auto StatusBarUserWidget = Cast<UStatusBarWidget>(LifeBar->GetUserWidgetObject());
	if (!StatusBarUserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to Cast to UHealthBarWidget"), *GetOwner()->GetName());
		return;
	}
	StatusBarUserWidget->GetHealthBar()->PercentDelegate.BindUFunction(this, FName("GetLifePointPercent"));
	/*

	TArray<AActor*> FoundHighlight;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACursorHighlight::StaticClass(), FoundHighlight);

	if (GetController() == Cast<ASquadController>(GetController()))
	{
		ASquadController* MyController = Cast<ASquadController>(GetController());
		MyController->SetHighLight(Cast<ACursorHighlight>(FoundHighlight[0]));
	}
	*/

}

void ASquadCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	animInstance = GetMesh()->GetAnimInstance();


}


float ASquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	LifePoint -= ActualDamage;


	return ActualDamage;
}

float ASquadCharacter::GetLifePointPercent() const
{
	float result = LifePoint / MaxLifePoint;

	return result;
}

void ASquadCharacter::Characterdeath()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IsDeath = true;
}

// HP Bar
void ASquadCharacter::Test()
{
	auto LBLoc = this->LifeBar->GetComponentLocation();
	auto Ins = GetWorld()->GetGameInstance();
	auto SCM = Cast<USquadGameInstance>(Ins)->SCMIns;
	auto SCMLoc = SCM->SideViewCameraComponent->GetComponentLocation();
	auto SCMBLoc = SCM->CameraBoom->GetComponentLocation();
	auto Locc = (GetActorLocation() -  SCMLoc) * -1;

	auto Loccc = Locc * FVector::ForwardVector;
	auto Rot = Loccc.Rotation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(LBLoc, SCMLoc);

	auto test = SCMLoc.Rotation();
	LifeBar->SetWorldRotation(LookAtRot);

}

void ASquadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Test();
}
