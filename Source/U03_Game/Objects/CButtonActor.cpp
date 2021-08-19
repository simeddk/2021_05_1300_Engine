#include "CButtonActor.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"


ACButtonActor::ACButtonActor()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	
	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/StaticMeshes/SM_Cone.SM_Cone'");
	Mesh->SetStaticMesh(mesh);

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/MI_Emissive_Blue.MI_Emissive_Blue'");
	Mesh->SetMaterial(0, material);
}

void ACButtonActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACButtonActor::ApplyColor(FLinearColor InColor)
{
	UMaterialInstanceConstant* material = Cast<UMaterialInstanceConstant>(Mesh->GetMaterial(0));

	for (FVectorParameterValue& value : material->VectorParameterValues)
	{
		if (value.ParameterInfo.Name.Compare("Emissive") == 0)
			material->SetVectorParameterValueEditorOnly(value.ParameterInfo, InColor);
	}
}