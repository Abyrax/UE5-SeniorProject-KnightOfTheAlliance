// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(1.f);
		HealthBarWidget->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && PatrolTarget) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints) {
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}
}

void AEnemy::Die()
{
	//death montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage) {
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection) {
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(10.f);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget) {
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
		if (DistanceToTarget > CombatRadius) {
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}

		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}
	if (Attributes && Attributes->IsAlive()) {
		DirectionalHitReact(ImpactPoint);
	}
	else {
		Die();
	}
	

	/*
	if(HitSound){
		UGameplayStatics::PlaySoundAtLocation(
		this,
		HitSound,
		ImpactPoint
	}
	*/

	if (HitParticles && GetWorld()) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	//Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| =1,|ToHit| =1, so Forward * ToHit= cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// invers cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degree
	Theta = FMath::RadiansToDegrees(Theta);

	//if CP points down, theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) {
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f) {
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f) {
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f) {
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

	/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget) {
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}
