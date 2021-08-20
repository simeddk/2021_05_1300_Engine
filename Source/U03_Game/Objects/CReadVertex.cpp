#include "CReadVertex.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ACReadVertex::ACReadVertex()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/StaticMeshes/SM_Cube.SM_Cube'");
	Mesh->SetStaticMesh(mesh);

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/MI_VertexColor.MI_VertexColor'");
	Mesh->SetMaterial(0, material);
}

void ACReadVertex::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::K2_SetTimer(this, "PaintVertexColor", 1.0f, true);
}

void ACReadVertex::PaintVertexColor()
{
	Mesh->SetLODDataCount(1, Mesh->LODData.Num());

	UStaticMesh* mesh = Mesh->GetStaticMesh();

	FStaticMeshComponentLODInfo* info = &Mesh->LODData[0];
	info->PaintedVertices.Empty();
	info->OverrideVertexColors = new FColorVertexBuffer();
	
	FStaticMeshLODResources* resources = &mesh->RenderData->LODResources[0];

	TArray<FColor> colors;
	colors.Reserve(resources->GetNumVertices());

	for (int32 i = 0; i < resources->GetNumVertices(); i++)
	{
		colors.Add(FColor::MakeRandomColor());
	}

	info->OverrideVertexColors->InitFromColorArray(colors);

	BeginInitResource(info->OverrideVertexColors);
	Mesh->MarkRenderStateDirty();
}

