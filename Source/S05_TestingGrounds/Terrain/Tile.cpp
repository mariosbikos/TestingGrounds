// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavigationSystem/Public/NavigationSystem.h"
// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);

	NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Pool->Return(NavMeshBoundsVolume);
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(const TSubclassOf<AActor> ToSpawn,int32 MinSpawn,int32 MaxSpawn,float Radius,float MinScale,float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
	
}

void ATile::PlaceAIPawns(const TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius,1,1);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(ToSpawn, SpawnPosition);
	}

}

void ATile::PlaceAIPawn(const TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	Spawned->SpawnDefaultController();

	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));

	Spawned->Tags.Add(FName("Enemy"));
}

void ATile::SetPool(UActorPool* Pool)
{
	this->Pool = Pool;

	PositionNavMeshBoundsVolume(this->Pool);

}

void ATile::PositionNavMeshBoundsVolume(UActorPool* Pool)
{

	NavMeshBoundsVolume = Pool->Checkout();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough actors in pool for Tile: %s"), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Checked out actor: %s"), *NavMeshBoundsVolume->GetName());
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough actors in pool"));

		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation()+NavigationBoundsOffset);
	UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	//local to global location
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0,Radius, FQuat::Identity,HasHit ? FColor::Red : FColor::Green,true,100);

	return !HasHit;
}

TArray<FSpawnPosition> ATile::RandomSpawnPositions(int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 Index = 0; Index < NumberToSpawn; Index++)
	{
		FVector SpawnPoint;
		FSpawnPosition SpawnPosition;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		SpawnPosition.Scale = RandomScale;
		bool Found = FindEmptyLocation(SpawnPosition.Location, Radius*RandomScale);
		if (Found)
		{

			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}

	return SpawnPositions;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 100;

	for(int32 Attempt=0;Attempt< MAX_ATTEMPTS;Attempt++)
	{
		FVector RandomLocation = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(RandomLocation, Radius))
		{
			OutLocation = RandomLocation;
			return true;
		}
	}
	return false;
	
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	
}

