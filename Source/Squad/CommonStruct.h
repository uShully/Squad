// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonStruct.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FCharacterDataStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifePoint = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ClassNumber = 0;
};


USTRUCT(Atomic, BlueprintType)
struct FSkeletalMeshCop
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Cap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Cap_equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Head_equip1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Head_equip2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Shirt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Pants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Boots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Hands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_outfit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Backpack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Backpack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_collar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_shoulder_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_shoulder_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Vest_bottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Decals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Radio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Kneepad_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Kneepad_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Holster;


	void InitSkeletalMesh(USkeletalMesh* Head, USkeletalMesh* Cap, USkeletalMesh* Cap_equip
		, USkeletalMesh* Head_equip1, USkeletalMesh* Head_equip2, USkeletalMesh* Shirt
		, USkeletalMesh* Pants, USkeletalMesh* Boots, USkeletalMesh* Hands, USkeletalMesh* Vest
		, USkeletalMesh* Vest_outfit, USkeletalMesh* Backpack, USkeletalMesh* Backpack2, USkeletalMesh* Vest_collar
		, USkeletalMesh* Vest_shoulder_R, USkeletalMesh* Vest_shoulder_L, USkeletalMesh* Vest_bottom, USkeletalMesh* Decals
		, USkeletalMesh* Radio, USkeletalMesh* Kneepad_R, USkeletalMesh* Kneepad_L, USkeletalMesh* Holster)
	{
		if (Head != nullptr) this->Head = (Head);
		if (Cap != nullptr) this->Cap = (Cap);
		if (Cap_equip != nullptr) this->Cap_equip = (Cap_equip);
		if (Head_equip1 != nullptr) this->Head_equip1 = (Head_equip1);
		if (Head_equip2 != nullptr) this->Head_equip2 = (Head_equip2);
		if (Shirt != nullptr) this->Shirt = (Shirt);
		if (Pants != nullptr) this->Pants = (Pants);
		if (Boots != nullptr) this->Boots = (Boots);
		if (Hands != nullptr) this->Hands = (Hands);
		if (Vest != nullptr) this->Vest = (Vest);
		if (Vest_outfit != nullptr) this->Vest_outfit = (Vest_outfit);
		if (Backpack != nullptr) this->Backpack = (Backpack);
		if (Backpack2 != nullptr) this->Backpack2 = (Backpack2);
		if (Vest_collar != nullptr) this->Vest_collar = (Vest_collar);
		if (Vest_shoulder_R != nullptr) this->Vest_shoulder_R = (Vest_shoulder_R);
		if (Vest_shoulder_L != nullptr) this->Vest_shoulder_L = (Vest_shoulder_L);
		if (Vest_bottom != nullptr) this->Vest_bottom = (Vest_bottom);
		if (Decals != nullptr) this->Decals = (Decals);
		if (Radio != nullptr) this->Radio = (Radio);
		if (Kneepad_R != nullptr) this->Kneepad_R = (Kneepad_R);
		if (Kneepad_L != nullptr) this->Kneepad_L = (Kneepad_L);
		if (Holster != nullptr) this->Holster = (Holster);
	};
};


UCLASS()
class SQUAD_API ACommonStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommonStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	void Class_0_Mesh_Load();
	void Class_1_Mesh_Load();
	void Class_2_Mesh_Load();


	TArray<FSkeletalMeshCop*> SkeletalMeshArray;
	*/
};
