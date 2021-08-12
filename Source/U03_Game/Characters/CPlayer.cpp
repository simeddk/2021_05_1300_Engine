#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/CUserWidget_Select.h"
#include "Widgets/CUserWidget_SelectItem.h"
#include "Objects/CInteractDoor.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");

	//Component Settings
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> animClass;
	CHelpers::GetClass<UAnimInstance>(&animClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animClass);

	bUseControllerRotationYaw = false;
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::GetClass<UCUserWidget_Select>(&SelectWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Select.WB_Select_C'");
}


void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;
	
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/MI_Player_Body.MI_Player_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Player_Logo.MI_Player_Logo'");
	
	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);


	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	Action->SetUnarmedMode();

	SelectWidget = CreateWidget<UCUserWidget_Select, APlayerController>(GetController<APlayerController>(), SelectWidgetClass);
	SelectWidget->AddToViewport();

	SelectWidget->SetVisibility(ESlateVisibility::Hidden);
	
	UCUserWidget_SelectItem* item = SelectWidget->GetItem("Item1");

	SelectWidget->GetItem("Item1")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnFist);
	SelectWidget->GetItem("Item2")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnOneHand);
	SelectWidget->GetItem("Item3")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnTwoHand);
	SelectWidget->GetItem("Item4")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnMagicBall);
	SelectWidget->GetItem("Item5")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnWarp);
	SelectWidget->GetItem("Item6")->OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnTornado);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACPlayer::OnComponentEndOverlap);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("Tornado", EInputEvent::IE_Pressed, this, &ACPlayer::OnTornado);
	
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelectAction);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Released, this, &ACPlayer::OffSelectAction);
	
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &ACPlayer::OnInteract);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += (Option->GetZoomSpeed() * InAxis * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetZoomRange().X, Option->GetZoomRange().Y);
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackStepMode();
		return;
	}

	State->SetRollMode();
}

void ACPlayer::OnInteract()
{
	CheckNull(InteractDoor);
	InteractDoor->Interact(GetActorForwardVector());
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackStep();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector target = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	Montages->PlayRoll();
}

void ACPlayer::End_BackStep()
{
	if (Action->IsUnarmedMode() || Action->IsWarpMode())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	State->SetIdleMode();
}

void ACPlayer::End_Roll()
{
	if (Action->IsUnarmedMode() == false  && Action->IsWarpMode() == false)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	State->SetIdleMode();
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());
	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(State->IsIdleMode());
	Action->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());
	Action->SetWarpMode();
}

void ACPlayer::OnTornado()
{
	CheckFalse(State->IsIdleMode());
	Action->SetTornadoMode();
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::OnAim()
{
	Action->DoAim_Begin();
}

void ACPlayer::OffAim()
{
	Action->DoAim_End();
}

void ACPlayer::OnSelectAction()
{
	CheckFalse(State->IsIdleMode());

	SelectWidget->SetVisibility(ESlateVisibility::Visible);
	GetController<APlayerController>()->bShowMouseCursor = true;

	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffSelectAction()
{
	SelectWidget->SetVisibility(ESlateVisibility::Hidden);
	GetController<APlayerController>()->bShowMouseCursor = false;

	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	Action->AbortByDamage();

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACPlayer::Hitted()
{
	Status->SetMove();
	Montages->PlayHitted();
}

void ACPlayer::Dead()
{
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDead();
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	UKismetSystemLibrary::QuitGame(GetWorld(), GetController<APlayerController>(), EQuitPreference::Quit, false);
	//Get Player Controller
	//(1)UGameplayStatics::GetPlayerController(GetWorld());
	//(2)GetController<APlayerController>()
	//(3)GetWorld()->GetFirstPlayercontroller()
}

void ACPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);

	if (OtherActor->GetClass()->IsChildOf(ACInteractDoor::StaticClass()))
		InteractDoor = Cast<ACInteractDoor>(OtherActor);
}

void ACPlayer::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	CheckTrue(OtherActor == this);

	if (!!InteractDoor)
		InteractDoor = nullptr;
}


void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll:	Begin_Roll(); break;
		case EStateType::BackStep: Begin_BackStep(); break;
		case EStateType::Hitted: Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

