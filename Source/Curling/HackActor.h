// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HackActor.generated.h"

UCLASS()
class CURLING_API AHackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHackActor();

protected:
	

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* HackMesh;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* InteractionBox;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

public:	
	bool bPlayerInRange = false;

};
