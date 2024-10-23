// Fill out your copyright notice in the Description page of Project Settings.


#include "SplashScreen.h"

#include "Animation/WidgetAnimation.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void USplashScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(ScreenTimerHandle, this, &USplashScreen::RemoveScreen, 3.f);

	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		Character->OnSkipScreen.BindUObject(this, &USplashScreen::OnFadeAnimFinished);
	}
}

void USplashScreen::RemoveScreen()
{
	FWidgetAnimationDynamicEvent AnimFinishedEvent;
	AnimFinishedEvent.BindUFunction(this, FName("OnFadeAnimFinished"));
	BindToAnimationFinished(FadeAnim, AnimFinishedEvent);

	PlayAnimation(FadeAnim);
}

void USplashScreen::OnFadeAnimFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);

	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		Character->DisableUiInput();
		Character->CloseSplashScreen();
		Character->OnSkipScreen.Unbind();
	}
}
