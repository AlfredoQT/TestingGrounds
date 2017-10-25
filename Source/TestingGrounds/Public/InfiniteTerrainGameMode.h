// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */

class ANavMeshBoundsVolume;

UCLASS()
class TESTINGGROUNDS_API AInfiniteTerrainGameMode : public ATestingGroundsGameMode
{
	GENERATED_BODY()
	
private:
    
    void AddToPool(ANavMeshBoundsVolume* BoundsVolume);
	
public:
    
    UFUNCTION(BlueprintCallable, Category = "Pool")
    void PopulateBoundsVolumePool();
};
