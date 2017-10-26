// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    NavigationBoundsOffset = FVector(2000.f, 0.f, 0.f);

}

void ATile::SetPool(UActorPool* InPool)
{
    Pool = InPool;
    PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
    NavMeshBoundsVolume = Pool->Checkout();
    if (NavMeshBoundsVolume == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No elements in pool to checkout"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Got [%s]"), *NavMeshBoundsVolume->GetName());
    NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
    GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    TArray<FSpawnPosition> GeneratedPoints = SpawnPointGenerator(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
    for (auto& SpawnPosition : GeneratedPoints)
    {
        PlaceActor(ToSpawn, SpawnPosition);
    }
}

TArray<FSpawnPosition> ATile::SpawnPointGenerator(int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    TArray<FSpawnPosition> GeneratedPoints;
    int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    for (int32 Index = 0; Index != NumberToSpawn; Index++)
    {
        FSpawnPosition SpawnPosition;
        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
        bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
        if (found) {
            SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
            GeneratedPoints.Add(SpawnPosition);
        }
    }
    return GeneratedPoints;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
    FVector Min = FVector(0, -2000, 0);
    FVector Max = FVector(4000, 2000, 0);
    FBox Bounds(Min, Max);
    constexpr int32 MAX_ATTEMPTS = 100;
    for (int32 Index = 0; Index != MAX_ATTEMPTS; Index++) {
        FVector Location = FMath::RandPointInBox(Bounds);
        if (CanSpawnAtLocation(Location, Radius)) {
            OutLocation = Location;
            return true;
        }
    }
    return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
    AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
    Spawned->SetActorRelativeLocation(SpawnPosition.Location);
    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    // We only want to rotate it in the azimuth
    Spawned->SetActorRotation(FRotator(0.f, SpawnPosition.Rotation, 0.f));
    Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
    Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
    FHitResult HitResult;
    FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
    bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
    // FColor ResultColor = (HasHit) ? FColor(255, 0, 0) : FColor(0, 255, 0);
    // DrawDebugCapsule(GetWorld(), GlobalLocation, 0.f, Radius, FQuat::Identity, ResultColor);
    return !HasHit;
}
