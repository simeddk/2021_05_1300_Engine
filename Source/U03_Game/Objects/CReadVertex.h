#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CReadVertex.generated.h"

UCLASS()
class U03_GAME_API ACReadVertex : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	
public:	
	ACReadVertex();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void PaintVertexColor();
};
