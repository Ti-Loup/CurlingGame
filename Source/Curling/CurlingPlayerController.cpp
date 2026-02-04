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

    if (ULocalPlayer* LP = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(IMC_Player, 0);
        }
    }
}
//input F pour interagir avec le hack
void ACurlingPlayerController::SetupInputComponent()
{
    

}

void ACurlingPlayerController::TryLaunchStone()//modifier pour corriger le bug
{
    APlayerCharacter* MyChar = Cast<APlayerCharacter>(GetPawn());

    // Si pas de hack ou pas de perso, on annule
    if (!MyChar || !CurrentHack) return;

    
    // Si CurrentStone est vide (null), on en fait apparaître une pierre
    if (CurrentStone == nullptr)
    {
        if (RockClassToSpawn) // On vérifie qu'on a bien choisi un Blueprint
        {
            FVector SpawnLoc = CurrentHack->GetActorLocation() + FVector(60, 25, 0); // Un peu au-dessus du hack
            FRotator SpawnRot = CurrentHack->GetActorRotation();
            FActorSpawnParameters SpawnParams;

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
        
        MyChar->CurrentRock = CurrentStone;
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
void ACurlingPlayerController::SetCameraFollowStone(AActor* Stone)
{
    if (!Stone) return;//exit si pas de pierre

    SetViewTargetWithBlend(Stone, 0.5f);
    CameraState = ECameraState::FollowStone;
    

    
}