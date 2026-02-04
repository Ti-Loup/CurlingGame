// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RockBase.h"
#include "GameFramework/Actor.h"
#include "RockBlue.generated.h"

UCLASS()//RockBlue qui herite de RockBase
class CURLING_API ARockBlue : public ARockBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockBlue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
