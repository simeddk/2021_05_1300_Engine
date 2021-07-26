#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class U03_GAME_API ACHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableDraw() { bDraw = true; }
	FORCEINLINE void DisableDraw() { bDraw = false; }

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;
	
public:
	ACHUD();

	virtual void DrawHUD() override;

private:
	bool bDraw;
};
