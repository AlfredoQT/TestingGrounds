// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
    Pool = CreateDefaultSubobject<UActorPool>(TEXT("Pool"));
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* BoundsVolume)
{
    UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *BoundsVolume->GetName());
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
    TActorIterator<ANavMeshBoundsVolume> BoundsVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
    while (BoundsVolumeIterator) {
        ANavMeshBoundsVolume* BoundsVolume = *BoundsVolumeIterator;
        AddToPool(BoundsVolume);
        ++BoundsVolumeIterator;
    }
}
