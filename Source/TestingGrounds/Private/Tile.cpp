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
    CastSphere(GetActorLocation(), 300.f);
    CastSphere(GetActorLocation() + FVector(0.f, 0.f, 500.f), 300.f);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn)
{
    int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    FVector Min = FVector(0, -2000, 0);
    FVector Max = FVector(4000, 2000, 0);
    FBox Bounds(Min, Max);
    for (int32 Index = 0; Index != NumberToSpawn; Index++) {
        FVector SpawnPoint = FMath::RandPointInBox(Bounds);
        AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
        Spawned->SetActorRelativeLocation(SpawnPoint);
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    }
}

bool ATile::CastSphere(FVector Location, float Radius)
{
    FHitResult HitResult;
    bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(Radius));
    FColor ResultColor = (HasHit) ? FColor(255, 0, 0) : FColor(0, 255, 0);
    DrawDebugSphere(GetWorld(), Location, Radius, 25, ResultColor);
    return HasHit;
}
