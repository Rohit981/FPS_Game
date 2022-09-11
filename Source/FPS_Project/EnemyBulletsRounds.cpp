// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBulletsRounds.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemyBulletsRounds::AEnemyBulletsRounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->AttachTo(RootComponent);

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->AttachTo(Collider);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	 


}

// Called when the game starts or when spawned
void AEnemyBulletsRounds::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AFPS_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBulletsRounds::OnOverlapBegin);



}

void AEnemyBulletsRounds::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor == Player) && (OtherActor != this) && OtherComp)
	{
		Player->Health -= 30;

		Destroy();
	}
}

// Called every frame
void AEnemyBulletsRounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

