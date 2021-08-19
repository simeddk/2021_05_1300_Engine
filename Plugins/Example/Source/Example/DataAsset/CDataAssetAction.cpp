#include "CDataAssetAction.h"
#include "CDataAsset.h"

CDataAssetAction::CDataAssetAction(EAssetTypeCategories::Type InCategory)
	: Category(InCategory)
{
}

CDataAssetAction::~CDataAssetAction()
{
}

FText CDataAssetAction::GetName() const
{
	return FText::FromString(L"Custom DataAsset");
}

UClass* CDataAssetAction::GetSupportedClass() const
{
	return UCDataAsset::StaticClass();
}

FColor CDataAssetAction::GetTypeColor() const
{
	return FColor::Black;
}

uint32 CDataAssetAction::GetCategories()
{
	return Category;
}
