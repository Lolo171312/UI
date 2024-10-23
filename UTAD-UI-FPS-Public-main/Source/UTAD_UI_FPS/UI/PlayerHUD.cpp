// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Crosshair.h"
#include "AmmoCounter.h"
#include "DamageIndicator.h"
#include "PlayerHealthBar.h"
#include "ReloadBar.h"
#include "SkillPoints.h"
#include "SkillTree.h"

void UPlayerHUD::ShowNoWeapon()
{
	PlayerHealthBarWidget->Show();
	CrosshairWidget->Hide();
	AmmoCounterWidget->Hide();
	ReloadBarWidget->Hide();
	DamageIndicatorWidget->Show();
	MinimapWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	SkillTree->Hide();
	SkillPoints->Show();
}

void UPlayerHUD::ShowWeapon()
{
	PlayerHealthBarWidget->Show();
	CrosshairWidget->Show();
	AmmoCounterWidget->Show();
	ReloadBarWidget->Show();
	DamageIndicatorWidget->Show();
	MinimapWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	SkillTree->Hide();
	SkillPoints->Show();
}

void UPlayerHUD::Hide()
{
	PlayerHealthBarWidget->Hide();
	CrosshairWidget->Hide();
	AmmoCounterWidget->Hide();
	ReloadBarWidget->Hide();
	DamageIndicatorWidget->Hide();
	MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
	SkillTree->Hide();
	SkillPoints->Hide();
}

void UPlayerHUD::ShowSkillTree()
{
	SkillPoints->Hide();
	SkillTree->Show();
}

void UPlayerHUD::HideSkillTree()
{
	SkillPoints->Show();
	SkillTree->Hide();
}
