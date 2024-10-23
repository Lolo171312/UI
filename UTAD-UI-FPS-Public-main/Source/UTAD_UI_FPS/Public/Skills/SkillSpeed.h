// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SkillSpeed.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UTAD_UI_FPS_API USkillSpeed : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Speed")
	float SpeedMultiplier = 1.f;
};
