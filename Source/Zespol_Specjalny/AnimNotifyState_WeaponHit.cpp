#include "AnimNotifyState_WeaponHit.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy_MeleeGreatSword.h"

void UAnimNotifyState_WeaponHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    AEnemy_MeleeGreatSword* Enemy = Cast<AEnemy_MeleeGreatSword>(Owner);
    if (Enemy)
    {
        Enemy->StartHitWindow();
    }
}

void UAnimNotifyState_WeaponHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (!MeshComp) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    AEnemy_MeleeGreatSword* Enemy = Cast<AEnemy_MeleeGreatSword>(Owner);
    if (Enemy)
    {
        Enemy->ExecuteAttackHit();
    }
}

void UAnimNotifyState_WeaponHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    AEnemy_MeleeGreatSword* Enemy = Cast<AEnemy_MeleeGreatSword>(Owner);
    if (Enemy)
    {
        Enemy->StartHitWindow();
    }
}
