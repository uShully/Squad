// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"

UCLASS()
class SQUAD_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingTextActor();

	float TakenDamage;
	void SetDamage(float Damage);

	UFUNCTION()
	void FinishedFloatText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* Floating;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UFloatingTextWidget> FloatingWidgetClass;
	

};
