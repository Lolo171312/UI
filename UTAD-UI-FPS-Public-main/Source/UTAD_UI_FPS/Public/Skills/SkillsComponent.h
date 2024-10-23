// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SkillsComponent.generated.h"


class USkillDataAsset;
class AUTAD_UI_FPSCharacter;
class USkillBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetSkillPoint, int /*SkillPoints*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTAD_UI_FPS_API USkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillsComponent();

	bool BuySkill(const USkillDataAsset& SkillDataAsset);
	void AddSkillPoints(const int AddedSkillPoints);

	int GetSkillPoints() const {return SkillPoints;}

	FOnGetSkillPoint OnGetSkillPoint;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category="Skills")
	int SkillPoints = 0;
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Skills")
	TMap<FGameplayTag, USkillBase*> SkillsAvailable;

	UPROPERTY()
	AUTAD_UI_FPSCharacter* CharacterOwner;
};
