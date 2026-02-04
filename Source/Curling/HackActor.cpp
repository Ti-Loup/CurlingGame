// Fill out your copyright notice in the Description page of Project Settings.


#include "HackActor.h"
#include "Components/BoxComponent.h"
#include "CurlingPlayerController.h"

/*
Detecte si le joueur est proche du hack
Active / désactive l’interaction
Permet au joueur de savoir depuis quel hack il joue
*/
AHackActor::AHackActor()
{
    PrimaryActorTick.bCanEverTick = false;

    HackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HackMesh"));
    RootComponent = HackMesh;

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetCollisionProfileName(TEXT("Trigger"));

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AHackActor::OnOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AHackActor::OnOverlapEnd);
}

void AHackActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // On verifie si c'est un Pawn qui rentre dans la zone du hack (box collider)
    APawn* PlayerPawn = Cast<APawn>(OtherActor);
    if (PlayerPawn)
    {
        // 2. On récupère son Controller
        ACurlingPlayerController* PC = Cast<ACurlingPlayerController>(PlayerPawn->GetController());

        if (PC)
        {
            bPlayerInRange = true;
            PC->CurrentHack = this; // On dit au controller quel hack il utilise

            //TryLaunchStone qui vient du CurlingPlayerController.cpp
            PC->TryLaunchStone();

            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Hack : Joueur détecté !"));
        }
    }
}

void AHackActor::OnOverlapEnd(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32)
{
    if (ACurlingPlayerController* PC =
        Cast<ACurlingPlayerController>(OtherActor->GetInstigatorController()))
    {
        bPlayerInRange = false;
        PC->CurrentHack = nullptr;
    }

}