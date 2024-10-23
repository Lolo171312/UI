// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS/Public/Skills/SkillsComponent.h"

#include "Skills/SkillDataAsset.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPS/Public/Skills/SkillBase.h"

USkillsComponent::USkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USkillsComponent::BuySkill(const USkillDataAsset& SkillDataAsset)
{
	if(!SkillDataAsset.SkillTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Skill Tag NOT Valid!"));
		return false;
	}

	if(SkillPoints < SkillDataAsset.SkillCost) return false;
	
	for (TTuple<FGameplayTag, USkillBase*> Skill : SkillsAvailable)
	{
		if(Skill.Key == SkillDataAsset.SkillTag)
		{
			Skill.Value->GetSkill(CharacterOwner);
			AddSkillPoints(-SkillDataAsset.SkillCost);
			return true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Skill Tag wasn´t found in SkillsComponent!"));
	return false;	
}

void USkillsComponent::AddSkillPoints(const int AddedSkillPoints)
{
	SkillPoints+=AddedSkillPoints;
	
	if(OnGetSkillPoint.IsBound())
	{
		OnGetSkillPoint.Broadcast(SkillPoints);
	}
}

void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AUTAD_UI_FPSCharacter>(GetOwner());
}