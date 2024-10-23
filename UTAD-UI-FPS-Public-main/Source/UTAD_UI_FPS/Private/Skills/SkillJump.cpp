// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/SkillJump.h"

#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void USkillJump::GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr)
{
	UE_LOG(LogTemp, Warning, TEXT("New Jump Skill"));
	PlayerCharacterPtr->SetJumpForce(JumpMultiplier);
}
