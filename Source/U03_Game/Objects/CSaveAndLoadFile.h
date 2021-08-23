#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSaveAndLoadFile.generated.h"

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditAnywhere)
		float Health;

	friend FArchive& operator<<(FArchive& InArchive, FSaveData& InData)
	{
		return InArchive << InData.Name << InData.Health;
	}

	
};

UCLASS()
class U03_GAME_API ACSaveAndLoadFile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSaveAndLoadFile();

protected:
	virtual void BeginPlay() override;

private:	
	void OnSave();
	void OnLoad();

};
