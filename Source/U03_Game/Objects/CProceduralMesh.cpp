#include "CProceduralMesh.h"
#include "Global.h"
#include "ProceduralMeshComponent.h"

ACProceduralMesh::ACProceduralMesh()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Mesh, "Mesh");

	float w, h, d;
	w = h = d = 50.0f;

	//Front
	Vertices.Add(FVector(-d, -w, -h));
	Vertices.Add(FVector(-d, -w, +h));
	Vertices.Add(FVector(-d, +w, -h));
	Vertices.Add(FVector(-d, +w, +h));
	AddIndices(0);

	//Back
	Vertices.Add(FVector(+d, -w, -h));
	Vertices.Add(FVector(+d, +w, -h));
	Vertices.Add(FVector(+d, -w, +h));
	Vertices.Add(FVector(+d, +w, +h));
	AddIndices(4);

	//Top
	Vertices.Add(FVector(-d, -w, +h));
	Vertices.Add(FVector(+d, -w, +h));
	Vertices.Add(FVector(-d, +w, +h));
	Vertices.Add(FVector(+d, +w, +h));
	AddIndices(8);

	//Bottom
	Vertices.Add(FVector(-d, -w, -h));
	Vertices.Add(FVector(-d, +w, -h));
	Vertices.Add(FVector(+d, -w, -h));
	Vertices.Add(FVector(+d, +w, -h));
	AddIndices(12);

	//Left
	Vertices.Add(FVector(+d, -w, -h));
	Vertices.Add(FVector(+d, -w, +h));
	Vertices.Add(FVector(-d, -w, -h));
	Vertices.Add(FVector(-d, -w, +h));
	AddIndices(16);

	//Right
	Vertices.Add(FVector(+d, +w, -h));
	Vertices.Add(FVector(-d, +w, -h));
	Vertices.Add(FVector(+d, +w, +h));
	Vertices.Add(FVector(-d, +w, +h));
	AddIndices(20);

	//CreateMeshSection
	Mesh->CreateMeshSection
	(
		0,
		Vertices,
		Indices,
		TArray<FVector>(),
		TArray<FVector2D>(),
		TArray<FColor>(),
		TArray<FProcMeshTangent>(),
		true
	);
}

void ACProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < Vertices.Num(); i++)
		Vertices[i] += FVector(0, 0, Speed * DeltaTime);

	Mesh->UpdateMeshSection
	(
		0,
		Vertices,
		TArray<FVector>(),
		TArray<FVector2D>(),
		TArray<FColor>(),
		TArray<FProcMeshTangent>()
	);
}

void ACProceduralMesh::AddIndices(int32 InStart)
{
	Indices.Add(InStart + 2); //6
	Indices.Add(InStart + 1); //5
	Indices.Add(InStart + 0); //4

	Indices.Add(InStart + 3); //7
	Indices.Add(InStart + 1); //5
	Indices.Add(InStart + 2); //6
}

