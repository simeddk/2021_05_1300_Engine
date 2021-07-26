#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Components/CActionComponent.h"
#include "CDoAction_Throw.generated.h"

UCLASS()
class U03_GAME_API ACDoAction_Throw : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE class UCAim* GetAim() { return Aim; }

private:
	UFUNCTION()
		void AbortByTypeChange(EActionType InPrevType, EActionType InNewType);

	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnAim() override;
	virtual void OffAim() override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);

private:
	UPROPERTY()
		class UCAim* Aim;

private:
	class UCActionComponent* Action;

};
