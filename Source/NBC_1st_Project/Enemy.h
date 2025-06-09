// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Enemy.generated.h"

UCLASS()
class NBC_1ST_PROJECT_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	
	// 타이머 핸들 선언
	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
							AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, 
							int32 OtherBodyIndex, 
							bool bFromSweep, 
							const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, 
						AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, 
						int32 OtherBodyIndex);

	void ThrowBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 구체 컴포넌트 (콜리전 포함)
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	// 메시 컴포넌트 (보이는 구체)
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	// 블루프린트 클래스 가져오기 (BP_Character)
	TSubclassOf<class UObject> BP_CharacterClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Character;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float EnemyRange = 1500.f;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float EnemyPower = 200.f;
};