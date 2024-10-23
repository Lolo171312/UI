// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

class USkillDataAsset;
class UProgressBar;
class UButton;

DECLARE_DELEGATE(FOnPlayNoBuyAnim);
DECLARE_DELEGATE_TwoParams(FOnShowPanel, const FVector2D& /*NewPanelLocation*/, const USkillDataAsset* /*SkillDataAsset*/);
DECLARE_DELEGATE(FOnHidePanel);

UCLASS()
class UTAD_UI_FPS_API USkillButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	FOnPlayNoBuyAnim OnPlayNoBuyAnim;
	FOnShowPanel OnShowPanel;
	FOnHidePanel OnHidePanel;

	UPROPERTY(EditAnywhere, Category="Components")
	USkillButton* PrevButton;
	UPROPERTY(EditAnywhere, Category="Components")
	USkillButton* NextButton;
private:
	UFUNCTION()
	void OnButtonPressed();
	UFUNCTION()
	void OnButtonReleased();
	UFUNCTION()
	void OnButtonHovered();
	UFUNCTION()
	void OnButtonEndHovered();

	void BuySkill();
	void SetSkillImg(UTexture2D* NewButtonTexture);

	void ShowPanel();
	void SetInitialButtonState();
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget), Category="Components")
	UButton* SkillButton;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget), Category="Components")
	UProgressBar* SkillBar;

	UPROPERTY(EditAnywhere, Category="Skills")
	USkillDataAsset* SkillDataAsset;
	UPROPERTY(EditAnywhere, Category="Skills")
	float TotalTimeToBuySkill = 1.5f;
	UPROPERTY(EditAnywhere, Category="Skills")
	float HoverSecondsToShowData = 0.5f;

	bool BuyingSkill = false;
	float CurrentBuySkillTimer = 0.f;

	FTimerHandle ShowDataPanelHandle;
};
