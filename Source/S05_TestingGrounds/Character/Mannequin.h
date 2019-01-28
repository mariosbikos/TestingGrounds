// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"
class AGun;
UCLASS()
class S05_TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void UnPossessed() override;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	TSubclassOf<AGun> GunBlueprint;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();


	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Setup")
	class UCameraComponent* FirstPersonCameraComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, Category = "Setup")
	class USkeletalMeshComponent* Mesh1P = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	AGun* Gun = nullptr;
};
