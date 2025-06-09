// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 1. 콜리전 컴포넌트 생성 및 루트 설정
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SphereComponent->InitSphereRadius(19.f);
	SphereComponent->SetCollisionProfileName("PhysicsActor");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnOverlapBegin);
	RootComponent = SphereComponent;

	// 2. 메시 컴포넌트 생성 및 부착
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -16.5));
	MeshComponent->SetWorldScale3D(FVector(0.34f, 0.34f, 0.34f));
	MeshComponent->SetMassOverrideInKg(TEXT("None"), 70.f);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(SphereMesh.Object);
	}
	SphereComponent->SetSimulatePhysics(true);

	// 3. 머티리얼 적용
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/MyProject/M_Black"));
	if (MaterialAsset.Succeeded())
	{
		MeshComponent->SetMaterial(0, MaterialAsset.Object);
	}

	// 4. 콜리전 응답 설정 (BP_Character와만 반응)
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	//파일 경로: \Content\StarterContent\Audio\Explosion01
	//사운드 가져오기
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01'"));
	if (SoundAsset.Succeeded())
	{
		OverlapSound = SoundAsset.Object;
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABall::DestroyBall, 3.f, false);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
									 AActor* OtherActor, 
									 UPrimitiveComponent* OtherComp, 
									 int32 OtherBodyIndex, 
									 bool bFromSweep, 
									 const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// BP_Character와만 오버랩 이벤트 발생
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		UGameplayStatics::PlaySoundAtLocation(this, OverlapSound, GetActorLocation());
		Destroy();
	}
}

void ABall::DestroyBall()
{
	//Destroy();
}