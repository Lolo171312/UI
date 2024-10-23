// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skills/SkillDataAsset.h"
#include "SkillTree.generated.h"

class USkillDataAsset;
class USkillDataPanel;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API USkillTree : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget),Category="Components")
	USkillDataPanel* SkillDataPanel;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void Show();
	UFUNCTION()
	void SetSkillPointsText(int SkillPoints);
	UFUNCTION()
	void PlayNoBuyAnim();
	UFUNCTION()
	void ExitTree();
	void Hide();
	UFUNCTION()
	void ShowSkillDataPanel(const FVector2D& NewLocation, const USkillDataAsset* Data);
	UFUNCTION()
	void HideSkillDataPanel();
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Components")
	UTextBlock* SkillPointsTxt;
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Components")
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, Transient, meta=(BindWidgetAnim), Category="Animations")
	UWidgetAnimation* NoBuyAnim;

	FDelegateHandle DelegateHandle;
};
