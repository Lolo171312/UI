// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillJump.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UTAD_UI_FPS_API USkillJump : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	float JumpMultiplier = 1.f;
};
