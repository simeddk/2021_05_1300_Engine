#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CDataAssetFactory.generated.h"

UCLASS()
class EXAMPLE_API UCDataAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCDataAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	
};
