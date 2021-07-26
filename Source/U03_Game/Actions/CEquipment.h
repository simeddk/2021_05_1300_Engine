#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class U03_GAME_API ACEquipment : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:	
	ACEquipment();

public:
	//PlayEquipMontage, PawnControl, ChangeBodyMaterial
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//Socket? Particle? Decal? -> Delegate Execute
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip(); 
	void Begin_Equip_Implementation();
	
	//State -> Idle
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip(); 
	void End_Equip_Implementation();

	//OrientRotation -> true
	UFUNCTION(BlueprintNativeEvent)
		void Unequip(); 
	void Unequip_Implementation();

protected:
	virtual void BeginPlay() override;

public:
	//Purpose : Set(EActionType)Mode 시 Attachment(무기 액터)의 소켓을 변경하는 용도
	//Call : Begin_Equip이 노티파이에서 호출될 때
	//Bind : Attacment::OnEquip -> AttachToComponent
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;
	
	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	bool bEquipped;
	FEquipmentData Data;
	FLinearColor Color;

};
