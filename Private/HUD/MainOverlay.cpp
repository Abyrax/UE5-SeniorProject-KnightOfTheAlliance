// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthBar) {
		HealthBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar) {
		StaminaBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetSouls(int32 Souls)
{
	if (SoulsCou) {
		SoulsCou->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}

void UMainOverlay::SetGold(int32 Gold)
{
	if (GoldCou) {
		GoldCou->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}
