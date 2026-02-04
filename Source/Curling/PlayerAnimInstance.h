// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class CURLING_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimProperties(float DeltaTime);

private://Transient -> Use it while the game is running and remove it after
	//Character using the animation
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "True"))
	TObjectPtr<APlayerCharacter> PlayerCharacter = nullptr;

	//Movement Component of Character using the animation
	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MoveComp = nullptr;

	//Si le personnage saute ou est en train de tomber
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "True"))
	bool bIsInAir = false;

	//Le personnage change sa vitesse
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "True"))
	bool bIsAccelerating = false;

	//Le personnage bouge
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "True"))
	bool bIsMoving = false;

	//Vitesse du personnage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "True"))
	float Speed = 0;

};
