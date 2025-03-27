// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCube.generated.h"

struct FMagicCubeLetInfo;
class AMCubeLet;

enum class EMCubePlane
{
	XPlane = 0,

	YPlane = 1,

	ZPlane = 2,

	AllPlanes = 3
};

UCLASS()
class MAGICCUBE_API AMCube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMCube();

	UStaticMeshComponent* GetCoreMesh();

	UFUNCTION()
	void SelectCubeLet(uint32 CubeLetID, bool bIsSelected);

	UFUNCTION()
	AMCubeLet* GetSelectedCubeLet();
	
	void ActivateRotationPlane(const EMCubePlane& PlaneType);

	void DeActivateRotationPlane();

	void RotateCube(const float& PlaneRelativeCoordinate);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CoreMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AMCubeLet> CubeLetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AMCubeLet*> CubeLets;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int SelectedCubeLetID;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AMCubeLet*> SelectedCubeLetsInPlane;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WhiteString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName YellowString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BlueString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName OrangeString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName GreenString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RedString;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetWhiteCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector WhiteCenterOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetYellowCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector YellowCenterOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetBlueCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector BlueCenterOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetOrangeCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector OrangeCenterOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetGreenCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector GreenCenterOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AMCubeLet* CubeLetRedCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RedCenterOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMagicCubeLetInfo> CubeLetsInfo;
	
	EMCubePlane CurrentPlaneType = EMCubePlane::AllPlanes;
	
};
