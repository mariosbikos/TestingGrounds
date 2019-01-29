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

void ATile::PlaceActors(const TSubclassOf<AActor> ToSpawn,int32 MinSpawn,int32 MaxSpawn,float Radius,float MinScale,float MaxScale)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 Index = 0; Index < NumberToSpawn; Index++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		bool Found = FindEmptyLocation(SpawnPoint,Radius*RandomScale);
		if (Found)
		{
			float RandomRotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
		}
		
	}
	
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

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	FBox Bounds(Min, Max);
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint,float Rotation,float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	Spawned->SetActorScale3D(FVector(Scale));
	
}

