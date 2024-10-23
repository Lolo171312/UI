// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FGameplayTag SkillTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	int SkillCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	FText SkillDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	UTexture2D* SkillTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data")
	UTexture2D* BoughtSkillTexture;
};