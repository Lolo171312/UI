// Fill out your copyright notice in the Description page of Project Settings.


#include "UTAD_UI_FPS_Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/EnemyHealthBar.h"
#include "Components/WidgetComponent.h"

AUTAD_UI_FPS_Enemy::AUTAD_UI_FPS_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUTAD_UI_FPS_Enemy::BeginPlay()
{
	Super::BeginPlay();

	//Get WidgetComponent from EnemyActor
	WidgetComponent = FindComponentByClass<UWidgetComponent>();
	if(WidgetComponent)
	{
		WidgetComponent->CastShadow = false;
		WidgetComponent->SetVisibility(false);
	}
}

void AUTAD_UI_FPS_Enemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Rotate WidgetComponent to face Player´s Camera
	if(WidgetComponent->IsVisible())
	{
		if(APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
		{
			FVector HealthBarLocation = WidgetComponent->GetComponentLocation();
			FVector PlayerCameraLocation = PlayerCameraManager->GetCameraLocation();
			FRotator NewHealthBarRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarLocation, PlayerCameraLocation);
			WidgetComponent->SetWorldRotation(NewHealthBarRotation);
		}
	}
}

void AUTAD_UI_FPS_Enemy::SetHealth(int NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	if(WidgetComponent)
	{
		//Enable WidgetComponent if it not already Enabled
		if(!WidgetComponent->IsVisible())
		{
			WidgetComponent->SetVisibility(true);
		}
		//Update Enemy´s Health Bar value
		if(UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(WidgetComponent->GetWidget()))
		{
			HealthBar->UpdateEnemyHealthBarValue(Health, MaxHealth);
		}
	}
	if (Health == 0)
	{
		Destroy();
	}
}

int AUTAD_UI_FPS_Enemy::GetHealth()
{
	return Health;
}

void AUTAD_UI_FPS_Enemy::SetMaxHealth(int NewMaxHealth)
{
	MaxHealth = FMath::Max(0, NewMaxHealth);
}

int AUTAD_UI_FPS_Enemy::GetMaxHealth()
{
	return MaxHealth;
}