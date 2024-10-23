// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class USkillPoints;
class USkillTree;
class USplashScreen;
class UDamageIndicator;
class UAmmoCounter;
class UCrosshair;
class UPlayerHealthBar;
class UReloadBar;

/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAmmoCounter* AmmoCounterWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UReloadBar* ReloadBarWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPlayerHealthBar* PlayerHealthBarWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCrosshair* CrosshairWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDamageIndicator* DamageIndicatorWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* MinimapWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USplashScreen* SplashScreen;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillTree* SkillTree;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USkillPoints* SkillPoints;
	
	/** Show the Minimap and the PlayerHealthBar */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowNoWeapon();

	/** Show all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void ShowWeapon();

	/** Hide all the widgets */
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	void ShowSkillTree();
	void HideSkillTree();

};
