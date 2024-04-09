// Fill out your copyright notice in the Description page of Project Settings.

#include "Interfaces/PickingInterface.h"
#include "Items/Souls.h"

void ASouls::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickingInterface* PickingInterface = Cast<IPickingInterface>(OtherActor);
	if (PickingInterface) {
		PickingInterface->AddSouls(this);
		PickupEffectSpawn();
		PickupSoundSpawn();
		Destroy();
	}
}
