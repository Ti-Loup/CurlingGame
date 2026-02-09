
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "RockBase.h"
#include "InputMappingContext.h"


#include "PlayerCharacter.generated.h"
//enum DE L'etat de la camera du personnage
UENUM(BlueprintType)
enum class ECameraView : uint8
{
	FirstPerson,
	ThirdPersonClose,
	ThirdPersonFar
};
//ajout de ARockBase pour connecter Le input de PlayerCharacter au RockBase 
class ARockBase;
class ACurlingPlayerController;
class ABroomBase;//Pour l'interaction avec le balai (Combat)
class AFightingStone;//Pour l'interaction avec une pierre (Combat)
class APickUpObject;
UCLASS()
class CURLING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	//pour la camera 3eme personne
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))//Gives blueprints ability to access private variables
	class UCameraComponent* Camera;
	//pour pouvoir changer entre 1ere personne et 3eme personne
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))//Gives blueprints ability to access private variables
	class USpringArmComponent* SpringArm;

	



public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//on creer ici le bool pour connecter le blind action qu'on a mis dans c++ pour pouvoir le modifier dans unreal blueprints.
	UPROPERTY(BlueprintReadWrite, Category = "Animations")
	bool bWantsToEmote1;
	UPROPERTY(BlueprintReadWrite, Category = "Animations")
	bool bWantsToEmote2;
	UPROPERTY(BlueprintReadWrite, Category = "Animations")
	bool bWantsToEmote3;
	UPROPERTY(BlueprintReadWrite, Category = "Animations")
	bool bWantsToEmote4;

	//Ajout de la classe ARockBase + class ARockBase;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Curling")
	class ARockBase* CurrentRock;
	

protected:
	

	//			~~~~ INPUT ~~~~
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* IMC_Player;
	//IMC


	//IA_Movement
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Movement;
	//IA_Jump
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Jump;
	//Camera
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Camera;
	//ToggleView pour switcher entre 1ere personne et 3eme personne
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_ToggleView;

	//Emote Binding Pour Emote 1
	UPROPERTY (EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Emote1;
	//Emote Binding Pour Emote 1
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Emote2;
	//Emote Binding Pour Emote 1
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Emote3;
	//Emote Binding Pour Emote 1
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Emote4;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_RockThrow;
	//Pour choisir la rotation de la pierre
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_AdjustRotation;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_ToggleCurl;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Interaction;



	// Camera
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ThirdPersonArmLengthONE = 150.f;//Clique premiere fois pour Longeur de la camera a 150
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ThirdPersonArmLengthTWO = 300.f;//Clique une deuxieme fois pour avoir la longeur a 300

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FirstPersonArmLength = 0.f;

	//Variable camera actuelle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ECameraView CameraView = ECameraView::ThirdPersonClose;
	
	void ApplyCameraView();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* AdjustRotationAction;

	// La fonction unique qui gère + et -
	void AdjustRotation(const FInputActionValue& Value);
	


	

	// Le Mesh de l'objet (Balai, Pierre, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickUpMesh;
	// Si(nullptr) = mains vides.
	// Sinon on tient un ballet.
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	ABroomBase* HeldBroom;
	//Pour tenir une pierre
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AFightingStone* HeldFightingStone;
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	APickUpObject* HeldObject;

	//test
	void TestInput();
	
	//Inputs voids
	void Move(const FInputActionValue& InputValue);
	void CameraLook(const FInputActionValue & InputValue);
	void Jump();
	void ToggleView();//Fonction changement entre 1ere et 3eme personne
	void EmoteUn();//pour lancer l'emote 1
	void EmoteDeux();//pour lancer l'emote 2
	void EmoteTrois();// pour lancer l'emote 3
	void EmoteQuatre();//pour lancer l'emote 4
	void StopEmote();//pour arreter l'emote
	

	//Si le joueur interagie avec la pierre il charge sa force et lorsqu'il arrete d'appuiyer cela arrete
	void InteractCharge();
	void InteractStopCharge();
	
	void ToggleCurlSide(const FInputActionValue& Value);//pour choisir le curl de gauche ou droite
	//Pour pouvoir interagir avec les autres objets du monde.
	
	// appuie sur E
	void GrabObject();

	// relache E
	void DropObject();

	//Gestions d'appuier sur e
	void QuandInteractionAppuyer();  // appuie
	void QuandInteractionRelacher(); // relache
	FTimerHandle TimerHandle_DropInteraction;//pour quand on relache e
}; 
