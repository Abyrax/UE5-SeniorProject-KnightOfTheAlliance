// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/MainOverlay.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller) {
			MainOverlay = CreateWidget<UMainOverlay>(Controller, MainOverlayClass);
			MainOverlay->AddToViewport();
		}
	}
}
