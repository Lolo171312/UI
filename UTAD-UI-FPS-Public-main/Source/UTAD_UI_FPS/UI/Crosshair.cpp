// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"

#include "Components/Image.h"
#include "UTAD_UI_FPS/TP_WeaponComponent.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void UCrosshair::Show()
{
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent())
		{
			WeaponComponent->OnWeaponFire.BindUObject(this, &UCrosshair::Shoot);
			WeaponComponent->OnAimToEnemy.BindUObject(this, &UCrosshair::SetIsAimingToEnemy);
		}
	}

	BaseCrosshairColor = Crosshair->GetColorAndOpacity();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCrosshair::Hide()
{
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent())
		{
			WeaponComponent->OnWeaponFire.Unbind();
			WeaponComponent->OnAimToEnemy.Unbind();
		}
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UCrosshair::Shoot()
{
	PlayAnimation(ShootAnimation);
}

void UCrosshair::SetIsAimingToEnemy(bool AimingToEnemy)
{
	Crosshair->SetColorAndOpacity(AimingToEnemy? AimEnemyCrosshairColor : BaseCrosshairColor);
}
