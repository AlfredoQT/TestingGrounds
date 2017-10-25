// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassHISMC.h"

void UGrassHISMC::SpawnGrass(int32 SpawnCount)
{
    FVector Min = FVector(0, -2000, 0);
    FVector Max = FVector(4000, 2000, 0);
    FBox Bounds(Min, Max);
    for (int32 Index = 0; Index != SpawnCount; Index++) {
        FVector Location = FMath::RandPointInBox(Bounds);
        AddInstance(FTransform(Location));
    }
}


