#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Interfaces/ICharacter.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

//PlayEquipMontage, PawnControl, ChangeBodyMaterial
void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bPawnControl)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	character->ChangeColor(Color);
}

//Socket? Particle? Decal? -> Delegate Execute
void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

//State -> Idle
void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();
	bEquipped = true;
}

//OrientRotation -> true
void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}
