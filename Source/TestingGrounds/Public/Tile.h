// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
    GENERATED_USTRUCT_BODY()
    
    FVector Location;
    float Rotation;
    float Scale;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
    
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    UPROPERTY(EditDefaultsOnly, Category = Navigation)
    FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    UFUNCTION(BlueprintCallable, Category = "Pool")
    void SetPool(UActorPool* InPool);

private:
    
    bool CanSpawnAtLocation(FVector Location, float Radius);
    
    void PositionNavMeshBoundsVolume();
	
    bool FindEmptyLocation(FVector& OutLocation, float Radius);
    
    void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);
    
    UActorPool* Pool;
    
    AActor* NavMeshBoundsVolume;
	
    TArray<FSpawnPosition> SpawnPointGenerator(int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale);
};
