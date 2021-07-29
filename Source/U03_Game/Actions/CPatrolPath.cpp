#include "CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Scene);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Scene);

	Spline->SetRelativeLocation(FVector(0, 0, 30));

	Text->SetRelativeLocation(FVector(0, 0, 100));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Text->Text = FText::FromString(GetActorLabel());

	Spline->SetClosedLoop(bLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
	Text->SetVisibility(false);
}

