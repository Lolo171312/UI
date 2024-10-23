// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCounter.h"
#include "../UTAD_UI_FPSCharacter.h"
#include "../TP_WeaponComponent.h"
#include "Components/TextBlock.h"

void UAmmoCounter::Show()
{
	//Bind events to Update widget´s info
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent())
		{
			WeaponComponent->OnCurrentNumBulletsChanged.BindUObject(this, &UAmmoCounter::UpdateCurrentAmmo);
		}
		Character->OnTotalBulletsChanged.BindUObject(this, &UAmmoCounter::UpdateTotalAmmo);
	}
	
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAmmoCounter::Hide()
{
	//Unbind Widget´s events
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		if(UTP_WeaponComponent* WeaponComponent = Character->GetAttachedWeaponComponent())
		{
			WeaponComponent->OnCurrentNumBulletsChanged.Unbind();
		}
		Character->OnTotalBulletsChanged.Unbind();
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UAmmoCounter::UpdateCurrentAmmo(int NewCurrentAmmo)
{
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(NewCurrentAmmo)));
}

void UAmmoCounter::UpdateTotalAmmo(int NewTotalAmmo)
{
	TotalAmmo->SetText(FText::FromString(FString::FromInt(NewTotalAmmo)));
}
