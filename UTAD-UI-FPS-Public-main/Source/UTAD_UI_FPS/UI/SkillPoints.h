// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillPoints.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API USkillPoints : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	void Hide();

	UFUNCTION()
	void SetSkillPointsTxt(const int SkillPoints);
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Components")
	UTextBlock* SkillPtsTxt;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidgetAnim), Category="Components")
	UWidgetAnimation* GetSkillPoint;
	UPROPERTY(EditAnywhere, Transient, meta=(BindWidgetAnim), Category="Components")
	UWidgetAnimation* GetSkillPointAppear;

	FDelegateHandle DelegateHandle;
};
