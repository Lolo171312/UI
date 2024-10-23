// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skills/SkillBase.h"
#include "SkillDamage.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UTAD_UI_FPS_API USkillDamage : public USkillBase
{
	GENERATED_BODY()
	
public:
	virtual void GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageMultiplier = 1.f;
};
