// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS/UI/SkillPoints.h"

#include "Components/TextBlock.h"
#include "Skills/SkillsComponent.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void USkillPoints::Show()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if(AUTAD_UI_FPSCharacter* FPSCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(USkillsComponent* Component = FPSCharacter->GetSkillComponent())
		{
			DelegateHandle = Component->OnGetSkillPoint.AddUObject(this, &USkillPoints::SetSkillPointsTxt);
		}
	}
}

void USkillPoints::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
	if(DelegateHandle.IsValid())
	{
		if(AUTAD_UI_FPSCharacter* FPSCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
		{
			if(USkillsComponent* Component = FPSCharacter->GetSkillComponent())
			{
				Component->OnGetSkillPoint.Remove(DelegateHandle);
			}
		}
	}
}

void USkillPoints::SetSkillPointsTxt(const int SkillPoints)
{
	if(GetVisibility() == ESlateVisibility::Hidden) return;
	
	if(IsAnimationPlaying(GetSkillPoint) || IsAnimationPlaying(GetSkillPointAppear))
	{
		PlayAnimation(GetSkillPointAppear);
	}
	else
	{
		PlayAnimation(GetSkillPoint);
	}
	
	SkillPtsTxt->SetText(FText::FromString(FString::FromInt(SkillPoints)));
}
