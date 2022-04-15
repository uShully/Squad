// Fill out your copyright notice in the Description page of Project Settings.

#define BuffOn  true
#define BuffOff false

#include "PlayerSquadCharacter.h"
#include "CharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "SquadGameInstance.h"
#include "Grid.h"
//#include "Kismet/GameplayStatics.h"
//#include "UObject/ConstructorHelpers.h"

APlayerSquadCharacter::APlayerSquadCharacter()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		PlayerCharacterAnimBP(TEXT("Blueprint'/Game/DevFile/PlayerCharacterAnimInstance.PlayerCharacterAnimInstance_C'"));
	if (PlayerCharacterAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PlayerCharacterAnimBP.Class);
		UE_LOG(LogClass, Log, TEXT(" Anim Succeeded "));
	}
	/*
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("GUNSOUND"));
	AudioComp->SetupAttachment(RootComponent);
	*/

	static ConstructorHelpers::FObjectFinder<USoundBase> FireSound(L"SoundWave'/Game/GunSoundPack/Guns/gun_rifle_shot_01.gun_rifle_shot_01'");
	if (FireSound.Succeeded())
	{
		Fire_Sound = FireSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> GetHitSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_grunt_pain_12.voice_male_grunt_pain_12'");
	if (GetHitSound.Succeeded())
	{
		GetHit_Sound = GetHitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DeathSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_grunt_pain_death_08.voice_male_grunt_pain_death_08'");
	if (DeathSound.Succeeded())
	{
		Death_Sound = DeathSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SelectedSound(L"SoundWave'/Game/HumanMaleA/Wavs/voice_male_soldier_enemy_spotted_01.voice_male_soldier_enemy_spotted_01'");
	if (SelectedSound.Succeeded())
	{
		Selected_Sound = SelectedSound.Object;
	}


	/*
	// particleSystem

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->bAutoActivate = false;
	// ParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(L"");
	if (ParticleAsset.Succeeded())
	{
		ParticleSystem->SetTemplate(ParticleAsset.Object);
	}
	*/

	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoliderBODY(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_CHR.SK_Military_CHR'"));
	if (SoliderBODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SoliderBODY.Object);
	}
	*/

	//RootComponent = GetMesh();

	Cap = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cap"));
	Cap_equip = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cap equip"));
	Head_equip1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head equip1"));
	Head_equip2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head equip2"));
	Shirt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shirt"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest"));
	Vest_outfit = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest outfit"));
	Backpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpac"));
	Backpack2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpac2"));
	Vest_Collar = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VestCollar"));
	Vest_shoulder_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest shoulder R"));
	Vest_shoulder_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest shoulder L"));
	Vest_Bottom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest Bottom"));
	Decals = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Decals"));
	Radio = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Radio"));
	Kneepad_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kneepad R"));
	Kneepad_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kneepad L"));
	Holster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Holster"));

	Cap->SetupAttachment(GetMesh()); 
	Cap_equip->SetupAttachment(GetMesh());
	Head_equip1->SetupAttachment(GetMesh());
	Head_equip2->SetupAttachment(GetMesh());
	Shirt->SetupAttachment(GetMesh());
	Pants->SetupAttachment(GetMesh());
	Boots->SetupAttachment(GetMesh());
	Hands->SetupAttachment(GetMesh());
	Vest->SetupAttachment(GetMesh());
	Vest_outfit->SetupAttachment(GetMesh());
	Backpack->SetupAttachment(GetMesh());
	Backpack2->SetupAttachment(GetMesh());
	Vest_Collar->SetupAttachment(GetMesh());
	Vest_shoulder_R->SetupAttachment(GetMesh());
	Vest_shoulder_L->SetupAttachment(GetMesh());
	Vest_Bottom->SetupAttachment(GetMesh());
	Decals->SetupAttachment(GetMesh());
	Radio->SetupAttachment(GetMesh());
	Kneepad_R->SetupAttachment(GetMesh());
	Kneepad_L->SetupAttachment(GetMesh());
	Holster->SetupAttachment(GetMesh());


	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head1.SK_Military_Head1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP_EQUIP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP1(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Headset1.SK_Military_Headset1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP2(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Goggles1.SK_Military_Goggles1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SHIRT(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PANTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOOTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HANDS(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_OUTFIT(TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit9.SK_Military_Outfit9'"));
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BACKPACK(TEXT(""));
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BACKPACK2(TEXT(""));
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_COLLAR(TEXT(""));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_SHOULDER_R(TEXT(""));
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_SHOULDER_L(TEXT(""));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_BOTTOM(TEXT(""));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> DECALS(TEXT(""));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RADIO(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Radio3.SK_Military_Radio3'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> KNEEPAD_R(TEXT(""));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> KNEEPAD_L(TEXT(""));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> HOLSTER(TEXT(""));

	GetMesh()->SetSkeletalMesh(HEAD.Object);
	Cap->SetSkeletalMesh(CAP.Object);
	Cap_equip->SetSkeletalMesh(CAP_EQUIP.Object);
	Head_equip1->SetSkeletalMesh(HEAD_EQUIP1.Object);
	Head_equip2->SetSkeletalMesh(HEAD_EQUIP2.Object);
	Shirt->SetSkeletalMesh(SHIRT.Object);
	Pants->SetSkeletalMesh(PANTS.Object);
	Boots->SetSkeletalMesh(BOOTS.Object);
	Hands->SetSkeletalMesh(HANDS.Object);
	Vest->SetSkeletalMesh(VEST.Object);
	Vest_outfit->SetSkeletalMesh(VEST_OUTFIT.Object);
	Radio->SetSkeletalMesh(RADIO.Object);

	*/

	
	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Assault_Rifle(L"SkeletalMesh'/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B'");
	if (Assault_Rifle.Succeeded())
	{
		Weapon->SetSkeletalMesh(Assault_Rifle.Object);
	}
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	*/


	
}

void APlayerSquadCharacter::SetContentMesh(USkeletalMeshComponent* mesh, const TCHAR* ContentPath)
{
	if(ContentPath != nullptr)
	{
		USkeletalMesh* NewMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, ContentPath));
		if (NewMesh)
		{
			mesh->SetSkeletalMesh(NewMesh);
	
		}
		UE_LOG(LogClass, Log, TEXT(" SetSkeletalMesh "));
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT(" ContentPath nullptr "));
		mesh = nullptr;
	}
}

void APlayerSquadCharacter::SetWeaponMesh()  //USkeletalMeshComponent* mesh
{
	FName WeaponSocket(TEXT("weaponSocket"));
	if (Hands->DoesSocketExist(WeaponSocket))
	{		
		AttachToComponent(Hands, FAttachmentTransformRules::KeepRelativeTransform, "weaponSocket");
	
	}
}

void APlayerSquadCharacter::SetSkeletalMeshComp(USkeletalMesh* Head, USkeletalMesh* Cap, USkeletalMesh* Cap_equip
	, USkeletalMesh* Head_equip1, USkeletalMesh* Head_equip2, USkeletalMesh* Shirt
	, USkeletalMesh* Pants, USkeletalMesh* Boots, USkeletalMesh* Hands, USkeletalMesh* Vest
	, USkeletalMesh* Vest_outfit, USkeletalMesh* Backpack, USkeletalMesh* Backpack2, USkeletalMesh* Vest_collar
	, USkeletalMesh* Vest_shoulder_R, USkeletalMesh* Vest_shoulder_L, USkeletalMesh* Vest_bottom, USkeletalMesh* Decals
	, USkeletalMesh* Radio, USkeletalMesh* Kneepad_R, USkeletalMesh* Kneepad_L, USkeletalMesh* Holster)
{
	if (Head != nullptr) GetMesh()->SetSkeletalMesh(Head);
	if (Cap != nullptr) this->Cap->SetSkeletalMesh(Cap);
	if (Cap_equip != nullptr) this->Cap_equip->SetSkeletalMesh(Cap_equip);
	if (Head_equip1 != nullptr) this->Head_equip1->SetSkeletalMesh(Head_equip1);
	if (Head_equip2 != nullptr) this->Head_equip2->SetSkeletalMesh(Head_equip2);
	if (Shirt != nullptr) this->Shirt->SetSkeletalMesh(Shirt);
	if (Pants != nullptr) this->Pants->SetSkeletalMesh(Pants);
	if (Boots != nullptr) this->Boots->SetSkeletalMesh(Boots);
	if (Hands != nullptr) this->Hands->SetSkeletalMesh(Hands);
	if (Vest != nullptr) this->Vest->SetSkeletalMesh(Vest);
	if (Vest_outfit != nullptr) this->Vest_outfit->SetSkeletalMesh(Vest_outfit);
	if (Backpack != nullptr) this->Backpack->SetSkeletalMesh(Backpack);
	if (Backpack2 != nullptr) this->Backpack2->SetSkeletalMesh(Backpack2);
	if (Vest_collar != nullptr) this->Vest_Collar->SetSkeletalMesh(Vest_collar);
	if (Vest_shoulder_R != nullptr) this->Vest_shoulder_R->SetSkeletalMesh(Vest_shoulder_R);
	if (Vest_shoulder_L != nullptr) this->Vest_shoulder_L->SetSkeletalMesh(Vest_shoulder_L);
	if (Vest_bottom != nullptr) this->Vest_Bottom->SetSkeletalMesh(Vest_bottom);
	if (Decals != nullptr) this->Decals->SetSkeletalMesh(Decals);
	if (Radio != nullptr) this->Radio->SetSkeletalMesh(Radio);
	if (Kneepad_R != nullptr) this->Kneepad_R->SetSkeletalMesh(Kneepad_R);
	if (Kneepad_L != nullptr) this->Kneepad_L->SetSkeletalMesh(Kneepad_L);
	if (Holster != nullptr) this->Holster->SetSkeletalMesh(Holster);
}


////////////////////////////// Shot //////////////////////////////////////////////

void APlayerSquadCharacter::SetShotReady()
{
	StateEnum = EStateEnum::SE_Shot;

	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	if (CharAnimInst != nullptr)
	{
		CharAnimInst->Aiming();
	}
}

void APlayerSquadCharacter::Debug_Shot(ASquadCharacter* Target)
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
	FVector CharLoc = GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	FVector Rot = TargetLoc - CharLoc;
	SetActorRotation(Rot.ToOrientationRotator());
	

	if(gameIns->TargetCharacter != nullptr)
	{
		UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	
		CharAnim->BeShot();
		GameStatic->SpawnEmitterAttached(FireParticle, Weapon, FName("MuzzleFlash"));

		UGameplayStatics::PlaySoundAtLocation(this, Fire_Sound, GetActorLocation(), 0.2f);
		UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), this, nullptr);
	}

	SetCharacterEnd();

	if(UnderGrid != nullptr)
		UnderGrid->SetGridInfo_Material_temp2();
}

////////////////////////////// Move //////////////////////////////////////////////

void APlayerSquadCharacter::SetMoveReady()
{
	StateEnum = EStateEnum::SE_Move;


}

////////////////////////////// Cover //////////////////////////////////////////////

void APlayerSquadCharacter::SetCover()
{
	if(StateEnum == EStateEnum::SE_Stay)
	{
		UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
		auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

		CharAnim->BeCover();
		Buff_Cover(BuffOn);
		SetCharacterEnd();

		if (UnderGrid != nullptr)
			UnderGrid->SetGridInfo_Material_temp2();

		gameIns->BCIns->EndTurnSystem();
	}
}

void APlayerSquadCharacter::SetStay()
{
	StateEnum = EStateEnum::SE_Stay;

	
	UCharacterAnimInstance* CharAnim = Cast<UCharacterAnimInstance>(animInstance);
	CharAnim->Montage_Stop(0.5f);

}

void APlayerSquadCharacter::SetCharacterEnd()
{
	StateEnum = EStateEnum::SE_End;
	Cast<ABattleController>((Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns))->AddPlayerEndBattleArray(this);
	UE_LOG(LogClass, Log, TEXT(" Add EndArray " ));
}

float APlayerSquadCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage - CharacterDefenceArmor, DamageEvent, EventInstigator, DamageCauser);

	if (LifePoint > 0)
	{
		if (IsActiveBuffCover == true)
		{
			Buff_Cover(BuffOff);
		}
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GetHit_Sound, GetActorLocation(), 1.0f);
			CharAnimInst->Hit();
		}
	}
	else
	{
		UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
		if (CharAnimInst != nullptr)
		{
			PlayerDeath(CharAnimInst);
		}

	}

	return ActualDamage ;
}

void APlayerSquadCharacter::PlayerDeath(UCharacterAnimInstance* CharAnimInst)
{
	//UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	Cast<ABattleController>(gameIns->BCIns)->RemoveFromPlayerEndBattleArray(ArrayNumbering, numbering);
	Characterdeath(); // 충돌 무시, 무브먼트 정지 , 상태 변환

	UGameplayStatics::PlaySoundAtLocation(this, Death_Sound, GetActorLocation(), 1.0f);
	CharAnimInst->Death();

	LifeBar->SetHiddenInGame(true);

}

void APlayerSquadCharacter::PlaySelectedSound()
{
	// 
	UGameplayStatics::PlaySoundAtLocation(this, Selected_Sound, GetActorLocation(), 1.0f);
}

void APlayerSquadCharacter::StopMontage()
{
	UCharacterAnimInstance* CharAnimInst = Cast<UCharacterAnimInstance>(animInstance);
	CharAnimInst->StopMontage();
}

void APlayerSquadCharacter::InputTest()
{
	UE_LOG(LogClass, Log, L"!!!");
}

void APlayerSquadCharacter::SetUnderGrid(AGrid* Grid)
{
	UnderGrid = Grid;
}

AGrid* APlayerSquadCharacter::GetUnderGrid()
{
	return UnderGrid;
}

int32 APlayerSquadCharacter::GetBattleLineNumber()
{
	return BattleLineNumber;
}

void APlayerSquadCharacter::Buff_System() // 임시로 종료시키는 시스템으로 변경
{
	Buff_Cover(BuffOff);
}

void APlayerSquadCharacter::Buff_Cover(bool Onoff)
{
	// 이미 켜져있을때는?
	 // 종료하게 만든다 - 카운트가 생기면?
	 // 카운트가 0가 아닐시 아무일도 안일어남 - 추후 추가예정
	// 안켜져있을때
	 // 켜지게 하면 됨

	if (Onoff == BuffOff) // 버프 체크용 - 임시로 버프활성화시 버프를 끄는 함수로 변경
	{
		if (IsActiveBuffCover == true) // 버프가 켜져있으면 종료
		{
			CharacterDefenceArmor -= 1.f;
			IsActiveBuffCover = false;
			//UE_LOG(LogClass, Log, L"Buff off");
		}
		else
		{

		}
			//UE_LOG(LogClass, Log, L"Buff Not Active");
		// 꺼저있을때는 아무일도 안일어남
		
	}
	else // Onoff == BuffOn 버프 활성화
	{
		if(IsActiveBuffCover == false) // 버프가 비활성화시 버프가 활성화
		{ 
			CharacterDefenceArmor += 1.f;
			IsActiveBuffCover = true;
			//UE_LOG(LogClass, Log, L"Buff On");
		}
		else // 버프가 이미 활성화됬을때는 아무일도 안일어남
		{
			//UE_LOG(LogClass, Log, L"Buff Already Active");
		}
	}
}