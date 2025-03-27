// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicPlayerController.h"

#include "TargetingComponent.h"

AMagicPlayerController::AMagicPlayerController()
{
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>("TargetingComponent");
}

void AMagicPlayerController::SelectCubeLet()
{
	TargetingComponent->SelectCubeLet();
}

void AMagicPlayerController::RotateCube(const FVector2D& RotateAxis)
{
	if (RotateAxis.X != 0 && RotateAxis.Y != 0)
	{
		float XY = RotateAxis.X * RotateAxis.Y;
		TargetingComponent->RotateCubePlane(XY);
	}
	else if (RotateAxis.X != 0)
		TargetingComponent->RotateCubePlane(RotateAxis.X);
	else
		TargetingComponent->RotateCubePlane(RotateAxis.Y);
}

void AMagicPlayerController::CycleRotationPlane()
{
	TargetingComponent->CycleRotationPlane();
}


