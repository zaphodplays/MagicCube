// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCube.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAGICCUBE_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent();


	UFUNCTION(BlueprintCallable)
	void SelectCubeLet();
	
	void RotateCubePlane(const float& PlaneRelativeCoordinate);

	void FinishRotation();

	void SelectRotationPlane(const EMCubePlane& PlaneAxis);

	void DeSelectRotationPlane();

	UFUNCTION(BlueprintCallable)
	void CycleRotationPlane();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	

	AActor* GetActorUnderCrosshair() const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AMCube> Cube;

	UPROPERTY(Transient)
	bool bRotationSelected = false;
};
