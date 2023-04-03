// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonStruct.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACommonStruct::ACommonStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Class_0_Mesh_Load();



}

// Called when the game starts or when spawned
void ACommonStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



/*
void ACommonStruct::Class_0_Mesh_Load()
{

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
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BACKPACK1(TEXT(""));
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


	SkeletalMeshArray[0]->InitSkeletalMesh(HEAD.Object, CAP.Object, CAP_EQUIP.Object, HEAD_EQUIP1.Object, HEAD_EQUIP2.Object, SHIRT.Object, PANTS.Object
		, BOOTS.Object, HANDS.Object, VEST.Object, VEST_OUTFIT.Object, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, RADIO.Object, nullptr, nullptr, nullptr);

	//character->SetSkeletalMeshComp(HEAD.Object, CAP.Object, CAP_EQUIP.Object, HEAD_EQUIP1.Object, HEAD_EQUIP2.Object, SHIRT.Object, PANTS.Object
	//	, BOOTS.Object, HANDS.Object, VEST.Object, VEST_OUTFIT.Object, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, RADIO.Object, nullptr, nullptr, nullptr);


}

void ACommonStruct::Class_1_Mesh_Load()
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head1.SK_Military_Head1'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP_EQUIP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP1(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Headset1.SK_Military_Headset1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP2(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Goggles1.SK_Military_Goggles1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SHIRT(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PANTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOOTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HANDS(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_OUTFIT(TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit9.SK_Military_Outfit9'"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BACKPACK1(TEXT(""));
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

	SkeletalMeshArray[1]->InitSkeletalMesh(HEAD.Object, nullptr, nullptr, nullptr, HEAD_EQUIP2.Object, SHIRT.Object, PANTS.Object
		, BOOTS.Object, HANDS.Object, VEST.Object, VEST_OUTFIT.Object, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, RADIO.Object, nullptr, nullptr, nullptr);


}

void ACommonStruct::Class_2_Mesh_Load()
{
	/*
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Head1.SK_Military_Head1'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2.SK_Military_Helmet2'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAP_EQUIP(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Helmet2_fastering.SK_Military_Helmet2_fastering'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP1(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Headset1.SK_Military_Headset1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HEAD_EQUIP2(TEXT("SkeletalMesh'/Game/Military/Mesh/Head/SK_Military_Goggles1.SK_Military_Goggles1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SHIRT(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Shirt5.SK_Military_Shirt5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PANTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Pants4.SK_Military_Pants4'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOOTS(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Boots5.SK_Military_Boots5'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HANDS(TEXT("SkeletalMesh'/Game/Military/Mesh/Body/SK_Military_Hands_Male.SK_Military_Hands_Male'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST(TEXT("SkeletalMesh'/Game/Military/Mesh/SK_Military_Vest1.SK_Military_Vest1'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VEST_OUTFIT(TEXT("SkeletalMesh'/Game/Military/Mesh/Outfit/SK_Military_Outfit9.SK_Military_Outfit9'"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BACKPACK1(TEXT(""));
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

	SkeletalMeshArray[2]->InitSkeletalMesh(nullptr, nullptr, nullptr, nullptr, HEAD_EQUIP2.Object, SHIRT.Object, PANTS.Object
		, BOOTS.Object, HANDS.Object, VEST.Object, VEST_OUTFIT.Object, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, RADIO.Object, nullptr, nullptr, nullptr);
	
}
*/