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
	bool SweepResult = CastSphere(GetActorLocation(), 300);
	//CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(const TSubclassOf<AActor> ToSpawn,int32 MinSpawn,int32 MaxSpawn)
{
	FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);

	FBox Bounds(Min, Max);
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 Index = 0; Index < NumberToSpawn; Index++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
		Spawned->SetActorRelativeLocation(SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative,false));
	}
	

}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Camera,
		FCollisionShape::MakeSphere(Radius)
	);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 100, HasHit ? FColor::Red : FColor::Green,true,100);

	return HasHit;
}


