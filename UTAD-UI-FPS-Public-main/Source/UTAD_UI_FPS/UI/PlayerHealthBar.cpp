// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "../UTAD_UI_FPSCharacter.h"
#include "Components/ProgressBar.h"
#include "Math/Color.h"
#include "Styling/SlateColor.h"

#define BLINK_ANIMATION_TIME 1.f
#define BLINK_THRESHOLD 0.25f

void UPlayerHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(bIsLowHealth)
	{
		LowHealthBlink(InDeltaTime);
	}
}

void UPlayerHealthBar::Show()
{
	//Bind UpdatePlayerHealthBar function to Player event
	if(AUTAD_UI_FPSCharacter* MyCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		MyCharacter->OnHealthChanged.BindUObject(this, &UPlayerHealthBar::UpdatePlayerHealthBar);
	}

	BaseBarBackgroundColor = PlayerHealthBar->GetWidgetStyle().BackgroundImage.TintColor.GetSpecifiedColor();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerHealthBar::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);

	//Unbind UpdatePlayerHealthBar function to Player event
	if(AUTAD_UI_FPSCharacter* MyCharacter = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		MyCharacter->OnHealthChanged.Unbind();
	}
}

void UPlayerHealthBar::UpdatePlayerHealthBar(int NewHealth, int MaxHealth)
{
	//Set Bar´s Percent
	float NewPercent = static_cast<float>(NewHealth) / static_cast<float>(MaxHealth);
	PlayerHealthBar->SetPercent(NewPercent);

	//Set to True if Player has low Health
	bIsLowHealth = NewPercent <= BLINK_THRESHOLD;
	if(bIsLowHealth)
	{
		bBlinkTurningRed = true;
		BlinkTimer = 0.f;
	}
}

void UPlayerHealthBar::LowHealthBlink(float Delta)
{
	if(bBlinkTurningRed)
	{
		BlinkTimer+=Delta;
		if(BlinkTimer > BLINK_ANIMATION_TIME)
		{
			BlinkTimer = BLINK_ANIMATION_TIME;
			bBlinkTurningRed = false;
		}
	}
	else
	{
		BlinkTimer-=Delta;
		if(BlinkTimer < 0.f)
		{
			BlinkTimer = 0.f;
			bBlinkTurningRed = true;
		}
	}

	FSlateBrush BackgroundImageBrush = PlayerHealthBar->GetWidgetStyle().BackgroundImage;
	BackgroundImageBrush.TintColor = FSlateColor(FMath::Lerp(BaseBarBackgroundColor, LowHealthBarBackgroundColor, BlinkTimer));
	FProgressBarStyle NewBarStyle;
	NewBarStyle.SetBackgroundImage(BackgroundImageBrush);
	NewBarStyle.SetMarqueeImage(PlayerHealthBar->GetWidgetStyle().MarqueeImage);
	NewBarStyle.SetFillImage(PlayerHealthBar->GetWidgetStyle().FillImage);
	NewBarStyle.SetEnableFillAnimation(PlayerHealthBar->GetWidgetStyle().EnableFillAnimation);
	PlayerHealthBar->SetWidgetStyle(NewBarStyle);
}
