// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOver::Show()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		PlayerController->Pause();
	}

	TryAgainButton->SetFocus();
	TryAgainButton->OnClicked.AddDynamic(this, &UGameOver::TryAgain);
	QuitButton->OnClicked.AddDynamic(this, &UGameOver::Quit);
}

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	TryAgainButton->OnClicked.RemoveDynamic(this, &UGameOver::TryAgain);
	QuitButton->OnClicked.RemoveDynamic(this, &UGameOver::Quit);
}

void UGameOver::TryAgain()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->RestartLevel();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

	TryAgainButton->OnClicked.RemoveDynamic(this, &UGameOver::TryAgain);
	QuitButton->OnClicked.RemoveDynamic(this, &UGameOver::Quit);
}

void UGameOver::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
