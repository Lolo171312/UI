// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageIndicator.h"

#include "Components/Image.h"
#include "UTAD_UI_FPS/UTAD_UI_FPSCharacter.h"

void UDamageIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	ClearDamageDirections();
}

void UDamageIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(ImpactDatas.Num() > 0)
	{
		ClearDamageDirections();

		TArray<FDirectionResult> DirectionResults;
		for (FImpactData& ImpactData : ImpactDatas)
		{
			if(ImpactData.DecreasingImpact)
			{
				ImpactData.ImpactValue-=InDeltaTime;
			}
			else
			{
				ImpactData.TimeToDecrease-=InDeltaTime;
				if(ImpactData.TimeToDecrease < 0)
				{
					ImpactData.DecreasingImpact = true;
				}
			}
			
			//Evaluate RadialDamage side of the Point
			EDirections ThisImpactDirection = DetermineDamageDirection(ImpactData.ImpactedActor, ImpactData.ImpactPoint);
			DirectionResults.Add(FDirectionResult(ThisImpactDirection, ImpactData.ImpactValue));
		}

		//Paint each evaluated RadialDamage with the higher ImpactValue
		for (const FDirectionResult& Result : DirectionResults)
		{
			PaintDamageDirection(Result);
		}

		ImpactDatas.RemoveAll([](FImpactData& Value) {
			return Value.ImpactValue < 0; // Remove every Damage Indicator with Time lower than 0
		});
	}
}

void UDamageIndicator::Show()
{
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		Character->OnReceiveDamage.BindUObject(this, &UDamageIndicator::StartDamageIndicator);
	}
	
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDamageIndicator::Hide()
{
	if(AUTAD_UI_FPSCharacter* Character = Cast<AUTAD_UI_FPSCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		Character->OnReceiveDamage.Unbind();
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UDamageIndicator::StartDamageIndicator(AActor* PlayerActor, const FVector& OtherLocation)
{
	ImpactDatas.Add(FImpactData(PlayerActor, OtherLocation));
}

EDirections UDamageIndicator::DetermineDamageDirection(AActor* PlayerActor, const FVector& OtherLocation)
{
	if(PlayerActor == nullptr) return None;
	
	FVector DirectionToImpact = (OtherLocation - PlayerActor->GetActorLocation()).GetSafeNormal();
	float Dot = FVector::DotProduct(PlayerActor->GetActorForwardVector(), DirectionToImpact);
	FVector Cross = FVector::CrossProduct(PlayerActor->GetActorForwardVector(), DirectionToImpact);
	
	if(Dot > -0.5f && Dot < 0.5f)
	{
		if(Cross.Z > 0)
		{
			return Right;
		}
		
		return Left;
	}

	if(Dot > 0)
	{
		return Top;
	}
	
	return Bottom;
}

void UDamageIndicator::PaintDamageDirection(const FDirectionResult& DirectionResult)
{
	switch (DirectionResult.Direction)
	{
	case Top:
		if(RadialDamageTop->GetRenderOpacity() < DirectionResult.ImpactValue)
		{
			RadialDamageTop->SetRenderOpacity(DirectionResult.ImpactValue);
		}
		break;
	case Right:
		if(RadialDamageRight->GetRenderOpacity() < DirectionResult.ImpactValue)
		{
			RadialDamageRight->SetRenderOpacity(DirectionResult.ImpactValue);
		}
		break;
	case Left:
		if(RadialDamageLeft->GetRenderOpacity() < DirectionResult.ImpactValue)
		{
			RadialDamageLeft->SetRenderOpacity(DirectionResult.ImpactValue);
		}
		break;
	case Bottom:
		if(RadialDamageBottom->GetRenderOpacity() < DirectionResult.ImpactValue)
		{
			RadialDamageBottom->SetRenderOpacity(DirectionResult.ImpactValue);
		}
		break;
	case None:
		break;
	default: ;
	}
}

void UDamageIndicator::ClearDamageDirections()
{
	RadialDamageTop->SetRenderOpacity(0.f);
	RadialDamageRight->SetRenderOpacity(0.f);
	RadialDamageLeft->SetRenderOpacity(0.f);
	RadialDamageBottom->SetRenderOpacity(0.f);
}
