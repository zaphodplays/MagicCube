// Fill out your copyright notice in the Description page of Project Settings.

UE_DISABLE_OPTIMIZATION
#include "TargetingComponent.h"
#include "MCube.h"
#include "MCubeLet.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTargetingComponent::SelectCubeLet()
{
	AActor* TargetActor = GetActorUnderCrosshair();
	if (!TargetActor) return;
	if (TargetActor->IsA(AMCubeLet::StaticClass()))
	{
		if (const TObjectPtr<AMCubeLet> CubeLet = Cast<AMCubeLet>(TargetActor))
		{
			if (Cube == nullptr)
			{
				Cube = CubeLet->GetCubeOwner();
			}
			if (Cube.IsValid())
			{
				if (IsValid(Cube->GetSelectedCubeLet()))
				{
					return;
				}
				Cube->SelectCubeLet(CubeLet->GetCubeLetID(), true);
				SelectRotationPlane(EMCubePlane::ZPlane);
			}
		}
		
	}
	
}

void UTargetingComponent::SelectRotationPlane(const EMCubePlane& PlaneAxis)
{
	Cube->CurrentPlaneType = PlaneAxis;
	Cube->ActivateRotationPlane(PlaneAxis);
}

void UTargetingComponent::CycleRotationPlane()
{
	if (!Cube.IsValid() || !IsValid(Cube->GetSelectedCubeLet()))
	{
		return;
	}
	EMCubePlane NextPlane = static_cast<EMCubePlane>((static_cast<uint32>(Cube->CurrentPlaneType) + 1) % (static_cast<uint32>(EMCubePlane::AllPlanes)));
	//Cube->CurrentPlaneType = NextPlane;

	DeSelectRotationPlane();
	SelectRotationPlane(NextPlane);
}

void UTargetingComponent::DeSelectRotationPlane()
{
	if (!Cube.IsValid())
	{
		return;
	}
	Cube->DeActivateRotationPlane();
}

void UTargetingComponent::RotateCubePlane(const float& PlaneRelativeCoordinate)
{
	if (Cube.IsValid())
	{
		if (!IsValid(Cube->GetSelectedCubeLet()))
		{
			return;
		}
		auto SetRotationValue = [&PlaneRelativeCoordinate](double& RotationValue, const EMCubePlane& ValidRotationPlane, const EMCubePlane& CurrentRotationPlane)
		{
			int Sign = PlaneRelativeCoordinate == 0 ? 0 : PlaneRelativeCoordinate > 0 ? 1 : -1;
			RotationValue = CurrentRotationPlane == ValidRotationPlane ? Sign * 10.f : 0.f;
		};
		
		FRotator DeltaRot = FRotator(0, 0, 0);
		SetRotationValue(DeltaRot.Pitch, EMCubePlane::YPlane, Cube->CurrentPlaneType);
		SetRotationValue(DeltaRot.Yaw, EMCubePlane::ZPlane, Cube->CurrentPlaneType);
		SetRotationValue(DeltaRot.Roll, EMCubePlane::XPlane, Cube->CurrentPlaneType);
		
		Cube->GetRootComponent()->AddRelativeRotation(DeltaRot, false, nullptr, ETeleportType::TeleportPhysics);
		FRotator CurrentRot = Cube->GetRootComponent()->GetRelativeRotation();
		auto QuantizeRotation = [](double& RotationValue)
		{
			if (static_cast<int>(RotationValue) % 90 <= 1.0f)
			{
				RotationValue = static_cast<int>(RotationValue);
			}
		};
		QuantizeRotation(CurrentRot.Pitch);
		QuantizeRotation(CurrentRot.Yaw);
		QuantizeRotation(CurrentRot.Roll);
		Cube->GetRootComponent()->SetRelativeRotation(CurrentRot, false, nullptr, ETeleportType::TeleportPhysics);
	}
}


AActor* UTargetingComponent::GetActorUnderCrosshair() const
{
	UE_CLOG(GetOwner()->GetLocalRole() == ROLE_Authority, LogTemp, Error, TEXT("%s should be called on client"), ANSI_TO_TCHAR(__FUNCTION__));
	// Get screen center
	int32 ScreenWidth, ScreenHeight;
	const APlayerController* Controller = Cast<APlayerController>(GetOwner());
	Controller->GetViewportSize(ScreenWidth, ScreenHeight);
	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);

	// Convert screen position to world direction
	FVector WorldLocation, WorldDirection;
	Controller->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

	FHitResult HitResult;
	FVector Start = Controller->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (WorldDirection * 4000.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Controller->GetPawn());
	DrawDebugLine(this->GetWorld(), Start, End, FColor::Red, false, 1.0, 0, 3.0);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldDynamic, Params))
	{
		return HitResult.GetActor();
	}
	return nullptr;
}


// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

