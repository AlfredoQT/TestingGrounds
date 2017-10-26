// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
    BoundsVolumePool = CreateDefaultSubobject<UActorPool>(TEXT("Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* BoundsVolume)
{
    BoundsVolumePool->Add(BoundsVolume);
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
    TActorIterator<ANavMeshBoundsVolume> BoundsVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
    while (BoundsVolumeIterator) {
        AddToPool(*BoundsVolumeIterator);
        ++BoundsVolumeIterator;
    }
}
