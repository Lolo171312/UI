// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicator.generated.h"

class UImage;
/**
 * 
 */

UENUM()
enum EDirections
{
	Top,
	Right,
	Left,
	Bottom,
	None
};

USTRUCT()
struct FDirectionResult
{
	GENERATED_BODY()

	FDirectionResult() : Direction(None), ImpactValue(0.f)
	{}

	FDirectionResult(EDirections NewDirection, float NewImpactValue) : Direction(NewDirection), ImpactValue(NewImpactValue)
	{}
	
	EDirections Direction;
	float ImpactValue;
};

USTRUCT()
struct FImpactData
{
	GENERATED_BODY()

	FImpactData() : ImpactedActor(nullptr), ImpactPoint(FVector::Zero()), DecreasingImpact(false), TimeToDecrease(0.f), ImpactValue(0.f)
	{}

	FImpactData(AActor* Actor, FVector Point) : ImpactedActor(Actor), ImpactPoint(Point), DecreasingImpact(false), TimeToDecrease(1.5f), ImpactValue(1.0f)
	{}
	
	UPROPERTY()
	AActor* ImpactedActor;
	FVector ImpactPoint;
	bool DecreasingImpact;
	float TimeToDecrease;
	float ImpactValue;
};

UCLASS()
class UTAD_UI_FPS_API UDamageIndicator : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Show();
	void Hide();

	UFUNCTION()
	void StartDamageIndicator(AActor* PlayerActor, const FVector& OtherLocation);
	
	EDirections DetermineDamageDirection(AActor* PlayerActor, const FVector& OtherLocation);
	void PaintDamageDirection(const FDirectionResult& DirectionResult);
	void ClearDamageDirections();
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RadialDamageTop;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RadialDamageRight;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RadialDamageLeft;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RadialDamageBottom;

	UPROPERTY()
	AActor* ImpactedActor;
	FVector ImpactPoint = FVector::Zero();
	bool DecreasingImpact;
	float TimeToDecrease;
	float ImpactValue;
	
	TArray<FImpactData> ImpactDatas;
};
