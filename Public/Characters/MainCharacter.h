// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickingInterface.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class ASouls;
class UAnimMontage;
class UMainOverlay;

UCLASS()
class KNIGHTOFALLIANCE_API AMainCharacter : public ABaseCharacter, public IPickingInterface
{
	GENERATED_BODY()

public:


	// Sets default values for this character's properties
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere)
	float linetraceLenght;
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASouls* Souls) override;
	UFUNCTION(BlueprintCallable)
	void SetHealthPercentOnHUD();
protected:

	
	virtual void BeginPlay() override;
	void take();
	//callbacks for inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	virtual void Attack() override;
	void Roll();
	void Dodge();

	bool CheckStamina();

	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void RollEnd() override;
	virtual bool CanAttack() override;
	void PlayEquipMontage(const FName& SectionName);
	//virtual void Die_Implementation() override;
	bool CanDisarm();
	bool CanArm();

	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	void InitializeMainOverlay();

	

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	
	

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY()
	UMainOverlay* MainOverlay;

public:
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	UPROPERTY(BlueprintReadWrite)
	USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* ViewCamera;
};
