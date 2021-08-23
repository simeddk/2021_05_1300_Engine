#include "CDoAction_Throw.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "CAim.h"
#include "CThrow.h"

void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACDoAction_Throw::AbortByTypeChange);
}


void ACDoAction_Throw::DoAction()
{
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsInZoom());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>
	(
		Datas[0].ThrowClass, 
		transform,
		OwnerCharacter,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);
}

void ACDoAction_Throw::End_DoAction() 
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_Throw::OnAim()
{
	Aim->On();
}

void ACDoAction_Throw::OffAim()
{
	Aim->Off();
}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	UProceduralMeshComponent* otherProcMesh = Cast<UProceduralMeshComponent>(InHitResult.Component);

	if (!!otherProcMesh)
	{
		UProceduralMeshComponent* outProcMesh = nullptr;
		UMaterialInstanceConstant* material;
		CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/MI_Emissive_Red.MI_Emissive_Red'");
		FVector planeNormals[2] = { GetActorUpVector(), GetActorRightVector() };

		UKismetProceduralMeshLibrary::SliceProceduralMesh
		(
			otherProcMesh,
			InHitResult.Location,
			planeNormals[UKismetMathLibrary::RandomIntegerInRange(0, 1)],
			true,
			outProcMesh,
			EProcMeshSliceCapOption::CreateNewSectionForCap,
			material
		);

		outProcMesh->SetSimulatePhysics(true);
		outProcMesh->AddImpulse(FVector(1000.0f, 1000.0f, 1000.0f), NAME_None, true);
	}


	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);

}

void ACDoAction_Throw::AbortByTypeChange(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsInZoom())
	Aim->Off();
}


