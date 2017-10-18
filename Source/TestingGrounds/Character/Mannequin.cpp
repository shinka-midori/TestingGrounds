// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Weapons/Gun.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FP_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_CameraComponent->SetupAttachment(GetCapsuleComponent());
	FP_CameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FP_CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FP_MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Mesh"));
	FP_MeshComponent->SetOnlyOwnerSee(true);
	FP_MeshComponent->SetupAttachment(FP_CameraComponent);
	FP_MeshComponent->bCastDynamicShadow = false;
	FP_MeshComponent->CastShadow = false;
	FP_MeshComponent->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FP_MeshComponent->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	
	if (GunBlueprint == NULL) { return; }
	FP_Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	FP_Gun->AttachToComponent(FP_MeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	FP_Gun->AnimInstance = FP_MeshComponent->GetAnimInstance();
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, FP_Gun, &AGun::OnFire);
}

void AMannequin::Fire()
{
	FP_Gun->OnFire();
}
