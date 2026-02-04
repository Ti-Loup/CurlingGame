// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HackActor.h"
#include "CameraState.h"//pour le changement de camera lorsque dans le hack ou non
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"//pour player movement
#include "CurlingPlayerController.generated.h"

class ARockBase;


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    FreeMove    UMETA(DisplayName = "Free Move"),
    InHack      UMETA(DisplayName = "In Hack"),
    Sweeping   UMETA(DisplayName = "Sweeping")

};

UCLASS()
class CURLING_API ACurlingPlayerController : public APlayerController
{

	GENERATED_BODY()

protected: 
    virtual void BeginPlay() override;

public:
    virtual void SetupInputComponent() override;
    void TryLaunchStone();
   
    //etat du joueur
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    EPlayerState CurrentPlayerState = EPlayerState::FreeMove;
    //etat de la camera
    UPROPERTY(BlueprintReadOnly)
    ECameraState CameraState = ECameraState::FollowPlayer;

    //controle de la camera
    UFUNCTION(BlueprintCallable)
    void SetCameraFollowPlayer();

    UFUNCTION(BlueprintCallable)
    void SetCameraFollowStone(AActor* Stone);

    
    UPROPERTY()
    AHackActor* CurrentHack = nullptr;

    // pierre active
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ARockBase* CurrentStone = nullptr;
    //creation d'une sous classe pour faire apparaitre une pierre en avant du hack
    UPROPERTY(EditAnywhere, Category = "Curling")
    TSubclassOf<class ARockBase> RockClassToSpawn;
    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* IMC_Player;

    
};
