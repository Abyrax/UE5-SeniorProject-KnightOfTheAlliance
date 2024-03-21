// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "KnightAnimInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class KNIGHTOFALLIANCE_API UKnightAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* MainCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;
};
