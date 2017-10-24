// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    for (int32 Index = 0; Index != NumberToSpawn; Index++) {
        FVector SpawnPoint;
        float RandomScale = FMath::RandRange(MinScale, MaxScale);
        bool found = FindEmptyLocation(SpawnPoint, Radius * RandomScale);
        if (found) {
            float RandomRotation = FMath::RandRange(-180.f, 180.f);
            PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
        }
    }
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
    AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
    Spawned->SetActorRelativeLocation(SpawnPoint);
    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    // We only want to rotate it in the azimuth
    Spawned->SetActorRotation(FRotator(0.f, Rotation, 0.f));
    Spawned->SetActorScale3D(FVector(Scale));
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
