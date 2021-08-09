#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class U03_GAME_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	FORCEINLINE FString GetCollisionName() { return Datas[Index].CollisionName; }
	
public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void ResetGlobalDilation();

private:
	int32 Index;
	bool bEnable;
	bool bExist;
	bool bLast;

	TArray<class ACharacter*> HittedCharacters;
};
