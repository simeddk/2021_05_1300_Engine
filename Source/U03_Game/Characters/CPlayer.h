#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY(EditDefaultsOnly)
		class UCDataAsset* Test_DataAsset;

	UPROPERTY(EditDefaultsOnly)
		float ScanSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		float SmearLength = 0.1f;

private: //SceneComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UPostProcessComponent* PostProcess;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
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

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

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
	
	UFUNCTION()
		void End_Evade();

	void OnInteract();
	void OnScan();

private:
	void Begin_BackStep();
	void Begin_Roll();

public:
	void End_BackStep();
	void End_Roll();

private:
	void UpdateSmear();

private:
	UFUNCTION() void OnFist();
	UFUNCTION() void OnOneHand();
	UFUNCTION() void OnTwoHand();
	UFUNCTION() void OnMagicBall();
	UFUNCTION() void OnWarp();
	UFUNCTION() void OnTornado();

	UFUNCTION()
		void OnProgress(float Output);
	
	void OnDoAction();

	void OnAim();
	void OffAim();

	void OnSelectAction();
	void OffSelectAction();

	void Hitted();
	
	UFUNCTION()
		void Hitted_End();

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
	FTimeline Timeline;
	class UCurveFloat* Curve;
	class UMaterialParameterCollection* ParameterCollection;

protected:
	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Select* SelectWidget;
};
