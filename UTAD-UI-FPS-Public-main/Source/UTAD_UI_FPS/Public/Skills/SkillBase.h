// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

class AUTAD_UI_FPSCharacter;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class UTAD_UI_FPS_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetSkill(AUTAD_UI_FPSCharacter* PlayerCharacterPtr) PURE_VIRTUAL(USkillBase::GetSkill);
};
