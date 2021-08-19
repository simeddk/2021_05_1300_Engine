#include "CCustomDetailPannel.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

CCustomDetailPannel::CCustomDetailPannel()
{
}

CCustomDetailPannel::~CCustomDetailPannel()
{
}

TSharedRef<IDetailCustomization> CCustomDetailPannel::MakeInstance()
{
	return MakeShareable(new CCustomDetailPannel());
}

void CCustomDetailPannel::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Custom Button");

	category.AddCustomRow(FText::FromString("Color"))
		.ValueContent()
		.VAlign(VAlign_Center)
		.MaxDesiredWidth(250)
		[
			SNew(SButton).VAlign(VAlign_Center)
			.OnClicked(this, &CCustomDetailPannel::OnClicked)
			.Content()
			[
				SNew(STextBlock).Text(FText::FromString("Random Color"))
			]
		];

	DetailBuilder.GetObjectsBeingCustomized(Objects);
}

FReply CCustomDetailPannel::OnClicked()
{
	return FReply::Handled();
}
