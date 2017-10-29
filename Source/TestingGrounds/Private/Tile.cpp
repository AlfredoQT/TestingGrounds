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
    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius);
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    for (int32 Index = 0; Index != NumberToSpawn; Index++)
    {
        FSpawnPosition SpawnPosition;
        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
        bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
        if (found) {
            SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
            PlaceActor(ToSpawn, SpawnPosition);
        }
    }
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
    FBox Bounds(MinBounds, MaxBounds);
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
    if (Spawned == nullptr) return;
    Spawned->SetActorRelativeLocation(SpawnPosition.Location);
    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    // We only want to rotate it in the azimuth
    Spawned->SetActorRotation(FRotator(0.f, SpawnPosition.Rotation, 0.f));
    Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition)
{
    APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
    if (Spawned == nullptr) return;
    Spawned->SetActorRelativeLocation(SpawnPosition.Location);
    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    // We only want to rotate it in the azimuth
    Spawned->SetActorRotation(FRotator(0.f, SpawnPosition.Rotation, 0.f));
    Spawned->SpawnDefaultController();
    Spawned->Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
    Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (Pool == nullptr || NavMeshBoundsVolume == nullptr) return;
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
