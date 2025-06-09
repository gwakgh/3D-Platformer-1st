// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Ball.generated.h"

UCLASS()
class NBC_1ST_PROJECT_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 블루프린트 클래스 가져오기 (BP_Character)
	TSubclassOf<class UObject> BP_CharacterClass;
	
	// Overlap 이벤트 발생 시 소리 재생
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* OverlapSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 오버랩 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	// Mesh 및 Collision 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	
	// 타이머 핸들 선언
	FTimerHandle DestroyTimerHandle; 
	// 액터 삭제 함수
	void DestroyBall();

};
