#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Interfaces/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class U03_GAME_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 0;

private: //SceneComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private: //ActorComponent
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

public:
	FORCEINLINE class UCUserWidget_Select* GetSelectWidget() { return SelectWidget; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Select> SelectWidgetClass;

public:
	ACPlayer();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);

private:
	void OnWalk();
	void OffWalk();

private:
	void OnEvade();
	void OnInteract();

private:
	void Begin_BackStep();
	void Begin_Roll();

public:
	void End_BackStep();
	void End_Roll();

private:
	UFUNCTION() void OnFist();
	UFUNCTION() void OnOneHand();
	UFUNCTION() void OnTwoHand();
	UFUNCTION() void OnMagicBall();
	UFUNCTION() void OnWarp();
	UFUNCTION() void OnTornado();
	
	void OnDoAction();

	void OnAim();
	void OffAim();

	void OnSelectAction();
	void OffSelectAction();

	void Hitted();
	void Dead();
	void End_Dead() override;

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void ChangeColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

private:
	class AController* DamageInstigator;
	class ACInteractDoor* InteractDoor;

protected:
	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Select* SelectWidget;
};
