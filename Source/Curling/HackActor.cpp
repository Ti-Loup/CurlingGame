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

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AHackActor::OnOverlapBegin);//Overlap entre l'adresse de l'actor et InteractionBox
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AHackActor::OnOverlapEnd);
}

//Fonction quand il y a contact entre collisions hack et personnage
void AHackActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // On verifie si c'est un Pawn qui rentre dans la zone du hack (box collider)
    APawn* PlayerPawn = Cast<APawn>(OtherActor);
    if (PlayerPawn)
    {
        // recupere son Controller
        ACurlingPlayerController* PlayerController = Cast<ACurlingPlayerController>(PlayerPawn->GetController());

        if (PlayerController)
        {
            bPlayerInRange = true;
            PlayerController->CurrentHack = this; // On dit au controller quel hack il utilise, 

            //TryLaunchStone qui vient du CurlingPlayerController.cpp
            PlayerController->TryLaunchStone();

            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Hack : Joueur détecté !"));
        }
    }
}

void AHackActor::OnOverlapEnd(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32)
{               //On prend l'adresse de l'acteur et on demande de trouver son InstigatorController(lui qu'il controlle) et de le cast au Playercontroller
    if (ACurlingPlayerController* PlayerControl = Cast<ACurlingPlayerController>(OtherActor->GetInstigatorController()))
    {
        bPlayerInRange = false;//On remet la variable a false
        PlayerControl->CurrentHack = nullptr;
    }

}