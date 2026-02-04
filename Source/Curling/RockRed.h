// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RockBase.h"
#include "GameFramework/Actor.h"
#include "RockRed.generated.h"

UCLASS()//Classe RockRed qui herite de Rockbase
class CURLING_API ARockRed : public ARockBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockRed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
