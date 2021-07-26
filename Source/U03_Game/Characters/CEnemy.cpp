#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"

ACEnemy::ACEnemy()
{
	//Create SceneComponent
	CHelpers::CreateComponent(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent(this, &HealthWidget, "HealthWidget", GetMesh());

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");

	//Component Settings
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> animClass;
	CHelpers::GetClass<UAnimInstance>(&animClass, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animClass);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/MI_Enemy_Body.MI_Enemy_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Enemy_Logo.MI_Enemy_Logo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetName());

	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	Action->SetUnarmedMode();
}



float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted();	break;
		case EStateType::Dead: Dead();	break;
	}
}

void ACEnemy::Hitted()
{
	//Update Health Widget
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())
		->Update(Status->GetHealth(), Status->GetMaxHealth());

	//Play Hitted Montage
	Montages->PlayHitted();

	//Find Look At Rotation
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	//Launch Character
	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LauchValue, true, false);

	//ChangeColor
	ChangeColor(FLinearColor::Red);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetColor", LogoLightTime, false);
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	Montages->PlayDead();
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	if (State->IsHittedMode())
	{
		FLinearColor color = InColor * LogoLightIntensity;
		LogoMaterial->SetVectorParameterValue("LogoLight", color);
		LogoMaterial->SetScalarParameterValue("UseLight", 1);

		return;
	}

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::ResetColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();

	LogoMaterial->SetVectorParameterValue("LogoLight", color);
	LogoMaterial->SetScalarParameterValue("UseLight", 0);
}


