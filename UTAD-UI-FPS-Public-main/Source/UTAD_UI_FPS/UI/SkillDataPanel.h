// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDataPanel.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API USkillDataPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(const FVector2D& NewLocation, const USkillDataAsset* Data);
	void Hide();

private:
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Components")
	UTextBlock* DataText;
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Components")
	UTextBlock* CostText;
};
