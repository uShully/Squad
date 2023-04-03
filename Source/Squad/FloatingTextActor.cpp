// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingTextActor.h"
#include "FloatingTextWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "SquadCharacter.h"

// Sets default values
AFloatingTextActor::AFloatingTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Floating = CreateDefaultSubobject<UWidgetComponent>(FName("Floating"));
	SetRootComponent(Floating);

	static ConstructorHelpers::FClassFinder<UFloatingTextWidget> FLOATING(TEXT("WidgetBlueprint'/Game/UI/FloatingBP.FloatingBP_C'"));
	if (FLOATING.Class)
	{
		FloatingWidgetClass = FLOATING.Class;
	}

	Floating->SetWidgetClass(FloatingWidgetClass);

}

// Called when the game starts or when spawned
void AFloatingTextActor::BeginPlay()
{
	Super::BeginPlay();		

	Cast<UFloatingTextWidget>(Floating->GetUserWidgetObject())->FloatingAnim->OnAnimationFinished.AddDynamic(this, &AFloatingTextActor::FinishedFloatText);

	//this->TakenDamage = Cast<ASquadCharacter>(GetOwner())->TakenDamage;
	SetDamage(Cast<ASquadCharacter>(GetOwner())->TakenDamage);

	
}

void AFloatingTextActor::SetDamage(float Damage)
{
	Cast<UFloatingTextWidget>(Floating->GetUserWidgetObject())->TakenDamage = Damage;	
}


void AFloatingTextActor::FinishedFloatText()
{
	Destroy();
}