#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "DebugRenderSceneProxy.h"

struct CGameplayDebuggerCategory_Struct
{
	class AActor* Actor;
	FVector Location;
};

class EXAMPLE_API CGameplayDebuggerCategory : public FGameplayDebuggerCategory
{
public:
	CGameplayDebuggerCategory();
	~CGameplayDebuggerCategory();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override; \

private:
	CGameplayDebuggerCategory_Struct Data;
};
