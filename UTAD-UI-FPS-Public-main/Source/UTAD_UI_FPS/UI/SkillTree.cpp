// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTree.h"

#include "SkillButton.h"
#include "SkillDataPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPS/Public/Skills/SkillsComponent.h"

void USkillTree::NativeConstruct()
{
	Super::NativeConstruct();

	//Bind Event with every Button
	TArray<UWidget*> ChildWidgets;
	WidgetTree->GetAllWidgets(ChildWidgets);

	for (UWidget* ChildWidget : ChildWidgets)
	{
		if(ChildWidget != nullptr)
		{
			if(USkillButton* Button = Cast<USkillButton>(ChildWidget))
			{
				Button->OnPlayNoBuyAnim.BindUObject(this, &USkillTree::PlayNoBuyAnim);
				Button->OnShowPanel.BindUObject(this, &USkillTree::ShowSkillDataPanel);
				Button->OnHidePanel.BindUObject(this, &USkillTree::HideSkillDataPanel);
			}
		}
	}

	ExitButton->OnPressed.AddDynamic(this, &USkillTree::ExitTree);

	SkillDataPanel->Hide();
}

void USkillTree::NativeDestruct()
{
	Super::NativeDestruct();

	//Bind Event with every Button
	TArray<UWidget*> ChildWidgets;
	WidgetTree->GetAllWidgets(ChildWidgets);

	for (UWidget* ChildWidget : ChildWidgets)
	{
		if(ChildWidget != nullptr)
		{
			if(USkillButton* Button = Cast<USkillButton>(ChildWidget))
			{
				Button->OnPlayNoBuyAnim.Unbind();
				Button->OnShowPanel.Unbind();
				Button->OnHidePanel.Unbind();
			}
		}
	}

	ExitButton->OnPressed.RemoveAll(this);
}

void USkillTree::Show()
{
	SetVisibility(ESlateVisibility::Visible);
	
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(USkillsComponent* Component = Character->GetSkillComponent())
		{
			SetSkillPointsText(Component->GetSkillPoints());
			DelegateHandle = Component->OnGetSkillPoint.AddUObject(this, &USkillTree::SetSkillPointsText);
		}
	}
}

void USkillTree::SetSkillPointsText(int SkillPoints)
{
	SkillPointsTxt->SetText(FText::FromString(FString::FromInt(SkillPoints)));
}

void USkillTree::PlayNoBuyAnim()
{
	PlayAnimation(NoBuyAnim, 0, 2);
}

void USkillTree::ExitTree()
{
	if(AUTAD_UI_FPSCharacter* FPSCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		FPSCharacter->HideInventory();
	}
}

void USkillTree::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);

	if(DelegateHandle.IsValid())
	{
		if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
		{
			if(USkillsComponent* Component = Character->GetSkillComponent())
			{
				Component->OnGetSkillPoint.Remove(DelegateHandle);
			}
		}
	}
}

#pragma region PanelFunctions
void USkillTree::ShowSkillDataPanel(const FVector2D& NewLocation, const USkillDataAsset* Data)
{
	SkillDataPanel->Show(NewLocation, Data);
}

void USkillTree::HideSkillDataPanel()
{
	SkillDataPanel->Hide();
}
#pragma endregion PanelFunctions