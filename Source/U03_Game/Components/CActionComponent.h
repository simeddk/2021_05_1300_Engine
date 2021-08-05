#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, Tornado, MagicBall, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U03_GAME_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DataAssets[(int32)EActionType::Max];

public:
	FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }


	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTornadoMode() { return Type == EActionType::Tornado; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }

	void OffAllCollision();

public:
	UFUNCTION(BlueprintCallable) void SetUnarmedMode();
	UFUNCTION(BlueprintCallable) void SetFistMode();
	UFUNCTION(BlueprintCallable) void SetOneHandMode();
	UFUNCTION(BlueprintCallable) void SetTwoHandMode();
	UFUNCTION(BlueprintCallable) void SetWarpMode();
	UFUNCTION(BlueprintCallable) void SetTornadoMode();
	UFUNCTION(BlueprintCallable) void SetMagicBallMode();

public:
	void DoAction();

	void DoAim_Begin();
	void DoAim_End();

	void Dead();
	void End_Dead();

	void AbortByDamage();

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	//Purpose : 무기에 따른 블렌드 포즈 변경
	//Call : Set(EActionType)Mode 호출 시
	//Bind : ex) ACAnimInstance::OnTypeChanged
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;

	UPROPERTY()
		class UCAction* Datas[(int32)EActionType::Max];
};
