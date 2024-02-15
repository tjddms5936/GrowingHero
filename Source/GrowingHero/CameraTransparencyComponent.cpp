// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTransparencyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Set.h"

// Sets default values for this component's properties
UCameraTransparencyComponent::UCameraTransparencyComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    CapsulePercentageForTrace = 1.0f;
    DebugLineTraces = true;
    IsOcclusionEnabled = true;
}


void UCameraTransparencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

// Called when the game starts
void UCameraTransparencyComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCameraTransparencyComponent::init(USpringArmComponent* SpringArm, UCameraComponent* Camera, UCapsuleComponent* Capsule, APawn* Pawn)
{
    ActiveSpringArm = SpringArm;
    ActiveCamera = Camera;
    ActiveCapsuleComponent = Capsule;
    ActivePawn = Pawn;

    FTimerHandle DestroyTimer{};
    GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UCameraTransparencyComponent::SyncOccludedActors, 0.1f, true, 0.f);
}

bool UCameraTransparencyComponent::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
    {
        OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
    }

    return true;
}

void UCameraTransparencyComponent::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
    if (!IsValid(OccludedActor.Actor))
    {
        OccludedActors.Remove(OccludedActor.Actor);
    }

    OccludedActor.IsOccluded = false;
    OnShowOccludedActor(OccludedActor);
}

bool UCameraTransparencyComponent::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
    {
        OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
    }

    return true;
}

void UCameraTransparencyComponent::ForceShowOccludedActors()
{
    for (auto& Elem : OccludedActors)
    {
        if (Elem.Value.IsOccluded)
        {
            ShowOccludedActor(Elem.Value);
        }
    }
}

bool UCameraTransparencyComponent::HideOccludedActor(const AActor* Actor)
{
    // �츮�� OccludedActors�� �ش� Actor�� �켱 �ִ��� Ȯ���Ѵ�.
    FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

    // �̹� ����ȭ �� ���¶�� �׳� �Ѿ��
    if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
    {
        return false;
    }

    if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
    {
        ExistingOccludedActor->IsOccluded = true; // ����ȭ �Ǿ��ٰ� ���� �ʱ�ȭ ���ְ�
        OnHideOccludedActor(*ExistingOccludedActor); // �츮�� ���� ���� ��Ƽ����� �ٲپ� �ֱ�
    }
    else // �츮�� OccludedActors���� ���� Actor���?
    {
        UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
            Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
        if (StaticMesh)
        {
            FCameraOccludedActor OccludedActor;
            OccludedActor.Actor = Actor;
            OccludedActor.StaticMesh = StaticMesh;
            OccludedActor.Materials = StaticMesh->GetMaterials();
            OccludedActor.IsOccluded = true;
            OccludedActors.Add(Actor, OccludedActor); // ���Ӱ� �츮�� OccludedActors�� �߰����ְ�
            OnHideOccludedActor(OccludedActor); // �츮�� ���� ���� ��Ƽ����� �ٲپ� �ش�.
        }
    }

    return true;
}

void UCameraTransparencyComponent::SyncOccludedActors()
{
    // �⺻ ���� üũ
    if (!ShouldCheckCameraOcclusion()) return;

    // ���������� ��ü ������ ���� �Ÿ��� Ȯ �������°� �����ִٸ�
    if (ActiveSpringArm->bDoCollisionTest)
    {
        ForceShowOccludedActors();
        // -> ShowOccludedActor
        // -> OnShowOccludedActor : OccludedActors�� ����ִ� Actor�� �� ���� Material�� �ٲپ���
        return;
    }

    // ���� ��ġ�� ����� �� ����ó�� �� ���̴�.


    // ========================================================= ��ħ �ֵ� ã�� ==============================================
    FVector Start = ActiveCamera->GetComponentLocation();
    FVector End = ActivePawn->GetActorLocation();

    TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
    CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> OutHits;

    // ����Ʈ���̽� ����׿����� �׸��� ����. �׸��� ������ EDrawDebugTrace::None ����ϸ� �ȴ�.
    auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
        ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace, CollisionObjectTypes, true,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.f);
    // ========================================================= ��ħ �ֵ� ã�� ���� ==========================================

    if (bGotHits) // ��ģ�� ����.
    {
        // ����Ʈ���̽��� ��ģ �ֵ� �Ÿ��ǵ�, ��״� ���� ����ȭ �� ���̴�.
        TSet<const AActor*> ActorsJustOccluded;

        // ����ȭ �۾�
        for (FHitResult Hit : OutHits)
        {
            const AActor* HitActor = Cast<AActor>(Hit.Actor);
            HideOccludedActor(HitActor);
            // OnHideOccludedActor : �ش� Actor ����ȭ �۾� ����

            ActorsJustOccluded.Add(HitActor); // ���� Mesh�� �ݺ��ؼ� ó������ �ʱ� ���� Set �۾�
        }

        // �̹� ����ȭ �� Actor�� + ���� ����ȭ �� Actor���� OccludedActors�� ����Ǿ� �ִ�. 
        // OccludedActors�� ����ִ� Actor�� �߿� ActorsJustOccluded�� ���ٸ�? ���� ����ȭ �����ؾ� �ϴ� Actor�� ���̴�.
        for (auto& Elem : OccludedActors)
        {
            // OccludedActors�ȿ��� ������ ActorsJustOccluded���� ���� Actor. �׸��� �� Actor�� ���� ����ȭ �Ǿ��ִٸ�
            if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
            {
                // �ش� Actor�� ���� ����ȭ ����
                ShowOccludedActor(Elem.Value);
            }
        }
    }
    else // ���� Ʈ���̽��� ��ġ�� Actor�� �ϳ��� ���ٴ� �Ҹ�. ��, ��� OccludedActors���� Actor���� ���� ����ȭ ���� ���Ѿ� �Ѵ�.
    {
        ForceShowOccludedActors();
    }
}