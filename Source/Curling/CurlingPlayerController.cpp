// Fill out your copyright notice in the Description page of Project Settings.


#include "CurlingPlayerController.h"
#include "HackActor.h"
#include "RockBase.h"
#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"



void ACurlingPlayerController::BeginPlay()
{
    Super::BeginPlay();
   
}
//Pourrait etre supprimer
// Les inputs sont dans le PlayerController
void ACurlingPlayerController::SetupInputComponent()
{
    

}

void ACurlingPlayerController::TryLaunchStone()//modifier pour corriger le bug
{
    //On recherche notre personnage:  GetPawn -> cast qui verifie que c'est bien le joueur
    APlayerCharacter* PersonnnageChar = Cast<APlayerCharacter>(GetPawn());//1 byte

    // Si pas de hack ou pas de perso, on annule et on ne lance pas de pierre
    if (!PersonnnageChar || !CurrentHack) return;

    
    // Si CurrentStone est vide (nullptr), on en fait apparaître une pierre
    if (CurrentStone == nullptr)
    {
        //RockClassToSpawn est une sous-classe permettant de faire spawn une pierre devant lui
        if (RockClassToSpawn) // On vérifie qu'on a bien choisi un Blueprint
        {
            //creation d'un vector de type float + Ou est ce que le hack regarde vers. 

            FVector SpawnLoc = CurrentHack->GetActorLocation() + FVector(60, 25, 0); // Un peu au-dessus du hack
            FRotator SpawnRot = CurrentHack->GetActorRotation();//(Pitch, Yaw, Roll) pour l'orientation du hack
            FActorSpawnParameters SpawnParams;//parametre de base

            //SpawnActor de la pierre (Faire apparaitre)
            CurrentStone = GetWorld()->SpawnActor<ARockBase>(RockClassToSpawn, SpawnLoc, SpawnRot, SpawnParams);

            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pierre apparait !"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Code d'erreur 1 : La pierre n'est pas apparue"));
            return;
        }
    }
    

    // CurrentStone n'est plus null et le joueur peut l'utiliser
    if (CurrentStone)
    {
        
        PersonnnageChar->CurrentRock = CurrentStone;
        CurrentPlayerState = EPlayerState::InHack;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Pierre donnée au joueur !"));
    }
}

void ACurlingPlayerController::SetCameraFollowPlayer() {

    APawn* PlayerPawn = GetPawn();
    if (!PlayerPawn) return;

    SetViewTargetWithBlend(PlayerPawn, 0.5f);
    CameraState = ECameraState::FollowPlayer;
}

//Camera qui suit la pierre lorsque lancer
void ACurlingPlayerController::SetCameraFollowStone(AActor* Stone)//On passe l'adresse de (l'actor de la pierre) a sa fonction
{
    if (!Stone) return;//exit si pas de pierre

    SetViewTargetWithBlend(Stone, 0.5f);
    CameraState = ECameraState::FollowStone;
    

    
}