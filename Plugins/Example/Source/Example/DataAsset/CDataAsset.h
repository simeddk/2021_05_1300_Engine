#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CDataAsset.generated.h"

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	Body, Logo, Max
};

UCLASS()
class EXAMPLE_API UCDataAsset : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FName Name;
	
	UPROPERTY(EditAnywhere)
		float Value;

	UPROPERTY(EditAnywhere)
		class UMaterialInstanceConstant* Materials[(int32)EMaterialType::Max];

public:
	FORCEINLINE FName GetName() { return Name; }
	FORCEINLINE float GetValue() { return Value; }
	FORCEINLINE class UMaterialInstanceConstant** GetMaterials() { return Materials; }
};
