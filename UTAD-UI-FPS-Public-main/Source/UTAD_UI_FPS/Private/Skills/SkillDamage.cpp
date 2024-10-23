// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillDamage.h"

#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void USkillDamage::GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr)
{
	PlayerCharacterPtr->SetDamage(DamageMultiplier);
}
