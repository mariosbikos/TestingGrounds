// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"
class UActorPool;

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;

};
//
//USTRUCT()
//struct FSpawnPosition
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
//	int32 MinSpawn;
//	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
//	int32 MaxSpawn;
//	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
//	float Radius;
//	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
//	float MinScale;
//	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
//	float MaxScale;
//
//};





UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Procedural Generation")
	void PlaceActors(const TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn,float Radius, float MinScale, float MaxScale);

	UFUNCTION(BlueprintCallable, Category = "Procedural Generation")
	void PlaceAIPawns(const TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius);
	


	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);

	void PositionNavMeshBoundsVolume(UActorPool* Pool);


private:
	bool CanSpawnAtLocation(FVector Location, float Radius);
	TArray<FSpawnPosition> RandomSpawnPositions(int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale);
	bool FindEmptyLocation(FVector& OutLocatiom, float Radius);
	
	UActorPool* Pool;
	AActor* NavMeshBoundsVolume;

	template<class T>
	void RandomlyPlaceActors(const TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale);

	template<class T>
	void PlaceActor(TSubclassOf<T> ToSpawn, FSpawnPosition SpawnPosition);

	template<>
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	template<>
	void PlaceActor(const TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);
};


