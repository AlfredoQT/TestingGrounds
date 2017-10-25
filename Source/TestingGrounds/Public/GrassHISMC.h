// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassHISMC.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UGrassHISMC : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
private:
    
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void SpawnGrass(int32 SpawnCount);
	
};
