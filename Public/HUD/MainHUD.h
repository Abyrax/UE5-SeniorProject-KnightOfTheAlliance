// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UMainOverlay;

UCLASS()
class KNIGHTOFALLIANCE_API AMainHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Main)
	TSubclassOf<UMainOverlay> MainOverlayClass;

	UPROPERTY()
	UMainOverlay* MainOverlay;
public:
	FORCEINLINE UMainOverlay* GetMainOverlay() const { return MainOverlay; }
};
