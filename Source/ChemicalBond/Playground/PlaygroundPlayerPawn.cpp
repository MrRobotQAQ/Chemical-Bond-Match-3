#include "PlaygroundPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"
#include "../Movement/FluidMotionComponent.h"
#include "UObject/ConstructorHelpers.h"

APlaygroundPlayerPawn::APlaygroundPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CollisionRoot = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionRoot"));
	CollisionRoot->SetSphereRadius(70.f);
	CollisionRoot->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CollisionRoot;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetRelativeScale3D(FVector(0.8f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		VisualMesh->SetStaticMesh(SphereMesh.Object);
	}

	ElementLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ElementLabel"));
	ElementLabel->SetupAttachment(RootComponent);
	ElementLabel->SetText(FText::FromString(TEXT("C_Player")));
	ElementLabel->SetHorizontalAlignment(EHTA_Center);
	ElementLabel->SetVerticalAlignment(EVRTA_TextCenter);
	ElementLabel->SetTextRenderColor(FColor(40, 230, 170));
	ElementLabel->SetWorldSize(110.f);
	ElementLabel->SetRelativeLocation(FVector(0.f, 0.f, 130.f));

	FluidMotionComponent = CreateDefaultSubobject<UFluidMotionComponent>(TEXT("FluidMotionComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1700.f;
	CameraBoom->SetWorldRotation(FRotator(-70.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void APlaygroundPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (FluidMotionComponent)
	{
		FluidMotionComponent->SetEffectiveMass(PlayerAtomMass);
	}

	if (VisualMesh)
	{
		const FLinearColor PlayerColor(0.15f, 0.9f, 0.65f);
		const FVector PlayerColorVector(PlayerColor.R, PlayerColor.G, PlayerColor.B);
		VisualMesh->SetVectorParameterValueOnMaterials(TEXT("Color"), PlayerColorVector);
		VisualMesh->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), PlayerColorVector);
	}
}

void APlaygroundPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PollPlaygroundInput();
}

void APlaygroundPlayerPawn::PollPlaygroundInput()
{
	if (!FluidMotionComponent)
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		FluidMotionComponent->ClearMoveInput();
		FluidMotionComponent->ClearYawInput();
		FluidMotionComponent->SetSprintActive(false);
		return;
	}

	FVector MoveDirection = FVector::ZeroVector;
	if (PlayerController->IsInputKeyDown(EKeys::W))
	{
		MoveDirection.X += 1.f;
	}
	if (PlayerController->IsInputKeyDown(EKeys::S))
	{
		MoveDirection.X -= 1.f;
	}
	if (PlayerController->IsInputKeyDown(EKeys::D))
	{
		MoveDirection.Y += 1.f;
	}
	if (PlayerController->IsInputKeyDown(EKeys::A))
	{
		MoveDirection.Y -= 1.f;
	}

	if (MoveDirection.IsNearlyZero())
	{
		FluidMotionComponent->ClearMoveInput();
	}
	else
	{
		FluidMotionComponent->SetMoveInput(MoveDirection, 1.f);
	}

	float YawInput = 0.f;
	if (PlayerController->IsInputKeyDown(EKeys::E))
	{
		YawInput += 1.f;
	}
	if (PlayerController->IsInputKeyDown(EKeys::Q))
	{
		YawInput -= 1.f;
	}
	FluidMotionComponent->SetYawInput(YawInput);

	const bool bWantsSprint =
		PlayerController->IsInputKeyDown(EKeys::LeftShift) ||
		PlayerController->IsInputKeyDown(EKeys::RightShift);
	FluidMotionComponent->SetSprintActive(bWantsSprint);
}
