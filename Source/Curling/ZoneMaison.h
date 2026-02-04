// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneMaison.generated.h"

class USphereComponent;
class ARockBase;

UCLASS()
class CURLING_API AZoneMaison : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZoneMaison();
	void CalculerScore(int32& OutBluePoints, int32& OutRedPoints);
	//int32& pour travailler sur la variable originel
protected:
	

	UPROPERTY(VisibleAnywhere)
	USphereComponent* ZoneMaison;


};
