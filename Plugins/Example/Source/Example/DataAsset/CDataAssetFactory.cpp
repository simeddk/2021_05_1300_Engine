#include "CDataAssetFactory.h"
#include "CDataAsset.h"

UCDataAssetFactory::UCDataAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCDataAsset::StaticClass();
}

UObject* UCDataAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UCDataAsset>(InParent, InClass, InName, Flags);
}