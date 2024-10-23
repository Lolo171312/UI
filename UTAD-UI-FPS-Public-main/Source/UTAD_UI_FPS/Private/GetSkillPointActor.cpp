// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS/Public/GetSkillPointActor.h"

#include "Components/BoxComponent.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPS/Public/Skills/SkillsComponent.h"


// Sets default values
AGetSkillPointActor::AGetSkillPointActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootCmp");
	SetRootComponent(SceneComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	BoxComponent->SetupAttachment(SceneComponent);
}

void AGetSkillPointActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
}

// Called when the game starts or when spawned
void AGetSkillPointActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGetSkillPointActor::OnOverlapBegin);
}

void AGetSkillPointActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AUTAD_UI_FPSCharacter* UI_FPSCharacter = Cast<AUTAD_UI_FPSCharacter>(OtherActor))
	{
		if(USkillsComponent* SkillsComponent = UI_FPSCharacter->GetSkillComponent())
		{
			SkillsComponent->AddSkillPoints(SkillPoints);
			BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Destroy();
		}
	}
}