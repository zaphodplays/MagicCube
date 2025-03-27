// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MagicPlayerController.generated.h"

class UTargetingComponent;
/**
 * 
 */
UCLASS()
class MAGICCUBE_API AMagicPlayerController : public APlayerController
{
public:
	 AMagicPlayerController();

private:
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SelectCubeLet();

	UFUNCTION(BlueprintCallable)
	void RotateCube(const FVector2D& RotateAxis);

	UFUNCTION(BlueprintCallable)
	void CycleRotationPlane();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UTargetingComponent* TargetingComponent;
};
