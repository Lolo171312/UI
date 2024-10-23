// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataPanel.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Skills/SkillDataAsset.h"

void USkillDataPanel::Show(const FVector2D& NewLocation, const USkillDataAsset* Data)
{
	if(Data)
	{
		DataText->SetText(Data->SkillDescription);
		CostText->SetText(FText::FromString(FString::FromInt(Data->SkillCost)));
	}
	
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	// Obtenemos el CanvasPanelSlot
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasPanelSlot)
	{
		// Ajustar la posición del widget en relación con los anclajes
		FVector2D AnchorsMin = CanvasPanelSlot->GetAnchors().Minimum;
		FVector2D AnchorsMax = CanvasPanelSlot->GetAnchors().Maximum;

		// Ajustamos la posición para tener en cuenta los anclajes (si es necesario)
		FVector2D AdjustedPosition = MousePosition;

		// Comprobamos si los anclajes están centrados o no
		if (AnchorsMin != AnchorsMax)
		{
			// Los anclajes están en diferentes posiciones, lo que significa que no están centrados
			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			// Convertir la posición del ratón relativa a los anclajes
			AdjustedPosition.X -= ViewportSize.X * AnchorsMin.X;
			AdjustedPosition.Y -= ViewportSize.Y * AnchorsMin.Y;
		}

		// Establecer la nueva posición ajustada
		CanvasPanelSlot->SetPosition(AdjustedPosition);
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USkillDataPanel::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}
