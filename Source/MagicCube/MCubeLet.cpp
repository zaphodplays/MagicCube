// Fill out your copyright notice in the Description page of Project Settings.
UE_DISABLE_OPTIMIZATION

#include "MCubeLet.h"
#include "Components/BoxComponent.h"



// Sets default values
AMCubeLet::AMCubeLet()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	
	
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	CubeMesh->SetupAttachment(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetupAttachment(CubeMesh);
	
	PlaneX = CreateDefaultSubobject<UStaticMeshComponent>("PlaneX");
	PlaneX->SetupAttachment(CubeMesh);
	const float OffSet = 50.01f * GetActorScale().X;
	PlaneX->SetRelativeLocation(FVector(OffSet, 0, 0));
	PlaneX->SetRelativeRotation(FRotator(0, -90, 0));
	PlaneX->SetRelativeScale3D(FVector(0.9, 0.9, 1));

	PlaneXn = CreateDefaultSubobject<UStaticMeshComponent>("PlaneXn");
	PlaneXn->SetupAttachment(CubeMesh);
	PlaneXn->SetRelativeLocation(FVector(-OffSet, 0, 0));
	PlaneXn->SetRelativeRotation(FRotator(90,  0, 0));
	PlaneXn->SetRelativeScale3D(FVector(0.9, 0.9, 1));

	PlaneY = CreateDefaultSubobject<UStaticMeshComponent>("PlaneY");
	PlaneY->SetupAttachment(CubeMesh);
	PlaneY->SetRelativeLocation(FVector(0, OffSet, 0));
	PlaneY->SetRelativeRotation(FRotator(-90, 0, 0));
	PlaneY->SetRelativeScale3D(FVector(0.9, 0.9, 1));

	PlaneYn = CreateDefaultSubobject<UStaticMeshComponent>("PlaneYn");
	PlaneYn->SetupAttachment(CubeMesh);                              
	PlaneYn->SetRelativeLocation(FVector(0, -OffSet, 0));              
	PlaneYn->SetRelativeRotation(FRotator(90, 0, 0));               
	PlaneYn->SetRelativeScale3D(FVector(0.9, 0.9, 1));               

	PlaneZ = CreateDefaultSubobject<UStaticMeshComponent>("PlaneZ");
	PlaneZ->SetupAttachment(CubeMesh);
	PlaneZ->SetRelativeLocation(FVector(0, 0, OffSet));
	PlaneZ->SetRelativeRotation(FRotator(0, 0, 0));
	PlaneZ->SetRelativeScale3D(FVector(0.9, 0.9, 1));

	PlaneZn = CreateDefaultSubobject<UStaticMeshComponent>("PlaneZn");
	PlaneZn->SetupAttachment(CubeMesh);
	PlaneZn->SetRelativeLocation(FVector(0, 0, -OffSet));
	PlaneZn->SetRelativeRotation(FRotator(0, 180, 0));
	PlaneZn->SetRelativeScale3D(FVector(0.9, 0.9, 1));

	MaterialInstances.Reserve(6);
}

UStaticMeshComponent* AMCubeLet::GetCoreMesh()
{
	return CubeMesh;
}

void AMCubeLet::SetCubeOwner(AMCube* Cube)
{
	OwnerCube = Cube;
}

AMCube* AMCubeLet::GetCubeOwner()
{
	return OwnerCube;
}

void AMCubeLet::MakeItShine(bool bShine)
{
	for (auto MaterialInstance : MaterialInstances)
	{
		if (IsValid(MaterialInstance))
		{
			MaterialInstance->SetScalarParameterValue("ParamRoughness", !bShine * 1.f);
			MaterialInstance->SetScalarParameterValue("ParamShine", bShine * 1.f);
		}
	}
}

void AMCubeLet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMCubeLet::SetColorX(const FColor& XColor)
{
	ColorX = XColor;
	SetPlaneColor(PlaneX, FVector(ColorX.R/255.f, ColorX.G/255.f, ColorX.B/255.f));
}

void AMCubeLet::SetColorXn(const FColor& XnColor)
{
	ColorXn = XnColor;
	SetPlaneColor(PlaneXn, FVector(ColorXn.R/255.f, ColorXn.G/255.f, ColorXn.B/255.f));
}

void AMCubeLet::SetColorY(const FColor& YColor)
{
	ColorY = YColor;
	SetPlaneColor(PlaneY, FVector(ColorY.R/255.f, ColorY.G/255.f, ColorY.B/255.f));
}

void AMCubeLet::SetColorYn(const FColor& YnColor)
{
	ColorYn = YnColor;
	SetPlaneColor(PlaneYn, FVector(ColorYn.R/255.f, ColorYn.G/255.f, ColorYn.B/255.f));
}

void AMCubeLet::SetColorZ(const FColor& ZColor)
{
	ColorZ = ZColor;
	SetPlaneColor(PlaneZ, FVector(ColorZ.R/255.f, ColorZ.G/255.f, ColorZ.B/255.f));
}

void AMCubeLet::SetColorZn(const FColor& ZnColor)
{
	ColorZn = ZnColor;
	SetPlaneColor(PlaneZn, FVector(ColorZn.R/255.f, ColorZn.G/255.f, ColorZn.B/255.f));
}

void AMCubeLet::SetPlaneColor(UStaticMeshComponent* PlaneComp, const FVector& PlaneColor)
{
	if (UMaterialInstanceDynamic* DynMaterial = PlaneComp->CreateAndSetMaterialInstanceDynamic(0))
	{
		const FHashedMaterialParameterInfo Info(MaterialColorPropName);
		FLinearColor CurrentColor = FLinearColor::Black;
		DynMaterial->GetVectorParameterValue(Info, CurrentColor);
			
		DynMaterial->SetVectorParameterValue(MaterialColorPropName, PlaneColor);
		FLinearColor NewColor = FLinearColor::Black;
		DynMaterial->GetVectorParameterValue(Info, NewColor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, NewColor.Quantize(), "Old Color: " + CurrentColor.ToString());
		MaterialInstances.Add(DynMaterial);
	}
}


// Called when the game starts or when spawned
void AMCubeLet::BeginPlay()
{
	Super::BeginPlay();
	int x = 1;
	auto SetPlaneColor = [this, &x](UStaticMeshComponent* PlaneComp, FVector PlaneColor)
	{
		if (UMaterialInstanceDynamic* DynMaterial = PlaneComp->CreateAndSetMaterialInstanceDynamic(0))
		{
			const FHashedMaterialParameterInfo Info(MaterialColorPropName);
			FLinearColor CurrentColor = FLinearColor::Black;
			DynMaterial->GetVectorParameterValue(Info, CurrentColor);
			
			DynMaterial->SetVectorParameterValue(MaterialColorPropName, PlaneColor);
			FLinearColor NewColor = FLinearColor::Black;
			DynMaterial->GetVectorParameterValue(Info, NewColor);
			//GEngine->AddOnScreenDebugMessage(x, 5.0, NewColor.Quantize(), "Old Color: " + CurrentColor.ToString());
			//x++;
		}
		
	};
	
}


