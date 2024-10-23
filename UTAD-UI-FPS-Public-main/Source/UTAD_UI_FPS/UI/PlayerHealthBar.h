// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UProgressBar* PlayerHealthBar;
	
protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float BlinkTimer = 0.f;
	bool bIsLowHealth = false;
	bool bBlinkTurningRed = true;

	UPROPERTY(EditAnywhere, Category="Data")
	FLinearColor LowHealthBarBackgroundColor;

	FLinearColor BaseBarBackgroundColor;

	UFUNCTION()
	void UpdatePlayerHealthBar(int NewHealth, int MaxHealth);

	void LowHealthBlink(float Delta);
};
