// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetSkillPointActor.generated.h"

class UBoxComponent;

UCLASS()
class UTAD_UI_FPS_API AGetSkillPointActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGetSkillPointActor();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	UBoxComponent* BoxComponent;

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Skill")
	uint8 SkillPoints = 1;
};
