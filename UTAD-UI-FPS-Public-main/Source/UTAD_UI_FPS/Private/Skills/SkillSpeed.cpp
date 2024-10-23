// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS/Public/Skills/SkillSpeed.h"

#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void USkillSpeed::GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr)
{
	UE_LOG(LogTemp, Warning, TEXT("New Speed Skill"));
	PlayerCharacterPtr->SetSpeed(SpeedMultiplier);
}
