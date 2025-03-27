// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCubeLet.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct MAGICCUBE_API FMagicCubeLetInfo
{

	GENERATED_BODY()

	FMagicCubeLetInfo(){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorXn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorYn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor ColorZn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector CenterOffset;
	
};

UCLASS()
class MAGICCUBE_API AMCubeLet : public AActor
{
public:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AMCube* OwnerCube;

public:
	// Sets default values for this actor's properties
	AMCubeLet();

	UStaticMeshComponent* GetCoreMesh();

	UFUNCTION(BlueprintCallable)
	void SetCubeOwner(AMCube* Cube);

	UFUNCTION()
	AMCube* GetCubeOwner();

	UFUNCTION(BlueprintCallable)
	void MakeItShine(bool bShine);

	UFUNCTION()
	void SetCubeLetID(uint32 ID);

	UFUNCTION()
	uint32 GetCubeLetID();

	UFUNCTION(BlueprintCallable)
	void SetColorX(const FColor& XColor);

	UFUNCTION(BlueprintCallable)
	void SetColorXn(const FColor& XnColor);

	UFUNCTION(BlueprintCallable)
	void SetColorY(const FColor& YColor);

	UFUNCTION(BlueprintCallable)
	void SetColorYn(const FColor& YnColor);

	UFUNCTION(BlueprintCallable)
	void SetColorZ(const FColor& ZColor);

	UFUNCTION(BlueprintCallable)
	void SetColorZn(const FColor& ZnColor);

private:
	void SetPlaneColor(UStaticMeshComponent* PlaneMesh, const FVector& PlaneColor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> MaterialInstances;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneXn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneYn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneZn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorXn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorYn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor ColorZn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName MaterialColorPropName;

	UPROPERTY();
	uint32 CubeLetID;
};

inline void AMCubeLet::SetCubeLetID(uint32 ID)
{
	CubeLetID = ID;
}

inline uint32 AMCubeLet::GetCubeLetID()
{
	return CubeLetID;
}
