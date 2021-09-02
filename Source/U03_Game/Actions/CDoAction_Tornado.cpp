#include "CDoAction_Tornado.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "CAttachment.h"
#include "Components/BoxComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/CEnemy.h"

void ACDoAction_Tornado::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Tornado"))
		{
			Box = CHelpers::GetComponent<UBoxComponent>(actor);
			break;
		}
	}

	
}

void ACDoAction_Tornado::DoAction()
{
	Super::DoAction();

	CheckFalse(State->IsIdleMode());
	CheckTrue(bOnce);

	bOnce = true;
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Tornado::Begin_DoAction()
{
	Super::Begin_DoAction();

	AttachParticle = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, "");
	AttachParticle->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());
	AttachParticle->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());

	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OnCollision();
	
	UKismetSystemLibrary::K2_SetTimer(this, "Hitted", HittedTime, true);
}

void ACDoAction_Tornado::End_DoAction()
{
	Super::End_DoAction();

	Status->SetMove();
	State->SetIdleMode();

	FTimerDynamicDelegate dele;
	dele.BindUFunction(this, "Finish");
	UKismetSystemLibrary::K2_SetTimerDelegate(dele, LifeTime, false);
}

void ACDoAction_Tornado::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckNull(OwnerCharacter);

	FVector location = OwnerCharacter->GetActorLocation();

	Angle += Speed * DeltaSeconds;

	if (FMath::IsNearlyEqual(Angle, 360.0f))
		Angle = 0.0f;

	FVector pivot = FVector(Distance, 0, 0);
	FVector value = pivot.RotateAngleAxis(Angle, FVector::UpVector);

	location += value;

	Box->SetWorldLocation(location);
}

void ACDoAction_Tornado::Hitted()
{
	FDamageEvent e;
	for (ACharacter* character : HittedCharacters)
	{
		bool bDead = false;
		ACEnemy* enemy = Cast<ACEnemy>(character);
		if (!!enemy)
		{
			UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
			bDead = state->IsDeadMode();
		}

		if (!!character && bDead == false)
		{
			character->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
			break;
		}
	}
}

void ACDoAction_Tornado::Finish()
{
	if (!!AttachParticle)
		AttachParticle->DestroyComponent();

	bOnce = false;

	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OffCollision();

	UKismetSystemLibrary::K2_ClearTimer(this, "Hitted");
}

void ACDoAction_Tornado::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.AddUnique(InOtherCharacter);
}

void ACDoAction_Tornado::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.Remove(InOtherCharacter);
}

void ACDoAction_Tornado::Abort()
{
	Finish();
}