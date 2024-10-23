// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillButton.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Character.h"
#include "Skills/SkillDataAsset.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPS/Public/Skills/SkillsComponent.h"

#pragma region NativeEvents
void USkillButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	SkillButton->OnPressed.AddDynamic(this, &USkillButton::OnButtonPressed);
	SkillButton->OnReleased.AddDynamic(this, &USkillButton::OnButtonReleased);

	SkillButton->OnHovered.AddDynamic(this, &USkillButton::OnButtonHovered);
	SkillButton->OnUnhovered.AddDynamic(this, &USkillButton::OnButtonEndHovered);

	if(SkillDataAsset)
	{
		SetSkillImg(SkillDataAsset->SkillTexture);
	}

	SetInitialButtonState();
}

void USkillButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(BuyingSkill)
	{
		CurrentBuySkillTimer += InDeltaTime;
		SkillBar->SetPercent(CurrentBuySkillTimer / TotalTimeToBuySkill);
		if(CurrentBuySkillTimer > TotalTimeToBuySkill)
		{
			BuySkill();
		}
	}
}

void USkillButton::NativeDestruct()
{
	Super::NativeDestruct();

	SkillButton->OnPressed.RemoveAll(this);
	SkillButton->OnReleased.RemoveAll(this);
	SkillButton->OnHovered.RemoveAll(this);
	SkillButton->OnUnhovered.RemoveAll(this);
}
#pragma endregion NativeEvents

#pragma region ButtonEvents
void USkillButton::OnButtonPressed()
{
	CurrentBuySkillTimer = 0.f;
	BuyingSkill = true;

	if(ShowDataPanelHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(ShowDataPanelHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowDataPanelHandle);
	}

	if(OnHidePanel.IsBound())
	{
		OnHidePanel.Execute();
	}
}

void USkillButton::OnButtonReleased()
{
	BuyingSkill = false;
	SkillBar->SetPercent(0.f);
}

void USkillButton::OnButtonHovered()
{
	GetWorld()->GetTimerManager().SetTimer(ShowDataPanelHandle, this, &USkillButton::ShowPanel, HoverSecondsToShowData);
}

void USkillButton::OnButtonEndHovered()
{
	if(ShowDataPanelHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(ShowDataPanelHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowDataPanelHandle);
	}

	if(OnHidePanel.IsBound())
	{
		OnHidePanel.Execute();
	}
}

#pragma endregion ButtonEvents

void USkillButton::BuySkill()
{
	BuyingSkill = false;
	SkillBar->SetPercent(0.f);

	if(const AUTAD_UI_FPSCharacter* PlayerCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(PlayerCharacter->GetSkillComponent()->BuySkill(*SkillDataAsset))
		{
			SkillButton->SetIsEnabled(false);
			SkillButton->OnPressed.RemoveAll(this);
			SkillButton->OnReleased.RemoveAll(this);
			if(SkillDataAsset)
			{
				SetSkillImg(SkillDataAsset->BoughtSkillTexture);
			}

			if(NextButton != nullptr)
			{
				NextButton->SkillButton->SetIsEnabled(true);
			}
		}
		else
		{
			if(OnPlayNoBuyAnim.IsBound())
			{
				OnPlayNoBuyAnim.Execute();
			}
		}
	}
}

void USkillButton::SetSkillImg(UTexture2D* NewButtonTexture)
{
	if(NewButtonTexture == nullptr) return;
	
	FProgressBarStyle NewProgressBarStyle = SkillBar->GetWidgetStyle();
	FSlateBrush NewBackgroundImg = NewProgressBarStyle.BackgroundImage;
	NewBackgroundImg.SetResourceObject(NewButtonTexture);
	NewProgressBarStyle.SetBackgroundImage(NewBackgroundImg);
	SkillBar->SetWidgetStyle(NewProgressBarStyle);
}

void USkillButton::ShowPanel()
{
	if(OnShowPanel.IsBound())
	{
		const FVector2D& MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		OnShowPanel.Execute(MousePosition, SkillDataAsset);
	}
}

void USkillButton::SetInitialButtonState()
{
	SkillButton->SetIsEnabled(PrevButton == nullptr);
}
