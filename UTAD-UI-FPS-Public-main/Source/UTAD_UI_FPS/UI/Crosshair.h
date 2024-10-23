// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class UTAD_UI_FPS_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* ShootAnimation;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Crosshair;

	UPROPERTY(EditAnywhere, Category="Data")
	FLinearColor AimEnemyCrosshairColor;

	FLinearColor BaseCrosshairColor;
	
	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Show();

	UFUNCTION(BlueprintCallable, Category = Visibility)
	void Hide();

	UFUNCTION()
	void Shoot();
	UFUNCTION()
	void SetIsAimingToEnemy(bool AimingToEnemy);
};
