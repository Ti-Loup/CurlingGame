// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//verifie si on a un playerPawn
	if (APawn* PlayerPawn = TryGetPawnOwner()) {
		PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
		MoveComp = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
	}
}

void UPlayerAnimInstance::UpdateAnimProperties(float DeltaTime)
{
	APawn* OwnerNow = TryGetPawnOwner();
	if (OwnerNow != PlayerCharacter || !IsValid(MoveComp)) {
		PlayerCharacter = Cast<APlayerCharacter>(MoveComp);
		MoveComp = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
	}

	if (!PlayerCharacter || !MoveComp) return;

	const FVector Vel = PlayerCharacter->GetVelocity();
	Speed = Vel.Size2D();//pas besoin de Z donc -> 2D

	//Personnage dans les airs ? 
	bIsInAir = MoveComp->IsFalling();

	//Si le joueur utilise plusieurs movement input
	bIsAccelerating = MoveComp->GetCurrentAcceleration().SizeSquared() > KINDA_SMALL_NUMBER;
}
