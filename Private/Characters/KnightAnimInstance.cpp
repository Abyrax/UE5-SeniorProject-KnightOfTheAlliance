// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KnightAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UKnightAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (MainCharacter) {
		MainCharacterMovement = MainCharacter->GetCharacterMovement();
	}
}

void UKnightAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacterMovement) {

		GroundSpeed = UKismetMathLibrary::VSizeXY(MainCharacterMovement->Velocity);
		IsFalling = MainCharacterMovement->IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
	}
}