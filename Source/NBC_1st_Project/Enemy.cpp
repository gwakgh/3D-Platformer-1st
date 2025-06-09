// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SphereComponent 생성 (Collision)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SphereComponent->InitSphereRadius(EnemyRange);
	SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
	RootComponent = SphereComponent;

	// MeshComponent 생성 (렌더링)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);;

	// 구체 메시 적용 (SM_Bottington)
	// 경로: Content\LowerSector_Mod\Models\Bot\SM_Bottington.uasset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BottingtonMesh(TEXT("/Game/LowerSector_Mod/Models/Bot/SM_Bottington"));
	if (BottingtonMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(BottingtonMesh.Object);
		MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		MeshComponent->SetMassOverrideInKg(TEXT("None"), 70.f);
	}
	
	// 크기 조정 (개체 크기)
	MeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	
	// Material 적용 (M_Bottington)
	// 경로: Content\LowerSector_Mod\Models\Bot\Materials\M_Bottington.uasset
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BottingtonMaterial(TEXT("/Game/LowerSector_Mod/Models/Bot/Materials/M_Bottington"));
	if (BottingtonMaterial.Succeeded())
	{
		MeshComponent->SetMaterial(0, BottingtonMaterial.Object);
	}
	
	// 블루프린트 클래스 가져오기 (BP_Character)
	static ConstructorHelpers::FObjectFinder<UBlueprint> BPChar(TEXT("Blueprint'/Game/MyProject/Blueprint/BP_Character.BP_Character'"));
	if (BPChar.Succeeded())
	{
		BP_CharacterClass = (UClass*)BPChar.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::ThrowBall, 3.0f, true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Character)
	{
		//Kismet/KismetMathLibrary.h - FindLookAtRotation 노드 구현
		FVector StartLocation = GetActorLocation();
		FVector TargetLocation = Character->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

		//Kismet/KismetMathLibrary.h - LERP (Rotator) 노드 구현
		FRotator InterpolatedRotation = UKismetMathLibrary::RLerp(GetActorRotation(), LookAtRotation, DeltaTime * 5.f, true);

		//FindLookAtRotation, LERP를 이용하여 Character의 위치를 따라 이동시킴
		SetActorRotation(InterpolatedRotation);
	}

}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
						AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, 
						int32 OtherBodyIndex, 
						bool bFromSweep, 
						const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass() == BP_CharacterClass)
	{
		Character = OtherActor;
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, 
					AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, 
					int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass() == BP_CharacterClass)
	{
		Character = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	}
}

void AEnemy::ThrowBall()
{
	if (Character)
	{
		FTransform SpawnTransform = GetActorTransform();
		SpawnTransform.SetLocation(FVector(SpawnTransform.GetLocation().X,SpawnTransform.GetLocation().Y,
											SpawnTransform.GetLocation().Z + 150.f));
		// 액터 스폰
		TSubclassOf<ABall> BallClass = ABall::StaticClass();
		ABall* SpawnedBall = GetWorld()->SpawnActor<ABall>(BallClass, SpawnTransform);
		SpawnedBall->MeshComponent->SetupAttachment(SpawnedBall->SphereComponent);

		FVector Direction = Character->GetActorLocation() - GetActorLocation();
		Direction *= EnemyPower;
		SpawnedBall->MeshComponent->AddImpulse(Direction);
		
	}
}