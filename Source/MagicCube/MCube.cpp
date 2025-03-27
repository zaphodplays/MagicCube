// Fill out your copyright notice in the Description page of Project Settings.

UE_DISABLE_OPTIMIZATION
#include "MCube.h"
#include "MCubeLet.h"


// Sets default values
AMCube::AMCube()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>("CoreMesh");
	CoreMesh->SetupAttachment(Root);

	CubeLets.Reserve(26);
	SelectedCubeLetID = -1;
}

UStaticMeshComponent* AMCube::GetCoreMesh()
{
	return CoreMesh;
}

// Called when the game starts or when spawned
void AMCube::BeginPlay()
{
	Super::BeginPlay();
	
	auto SpawnCubeLet = [this](const FVector& OffsetLoc)->AMCubeLet*
	{
		const FVector SpawnLocation = (GetActorLocation() + OffsetLoc);
		const FRotator SpawnRotation = (GetActorRotation());
		const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, GetActorScale());
		
		return Cast<AMCubeLet>(GetWorld()->SpawnActor(CubeLetClass.Get(), &SpawnTransform));
	};


	for (auto& CubeLetInfo : CubeLetsInfo)
	{
		AMCubeLet* CubeLet = SpawnCubeLet(CubeLetInfo.CenterOffset*(GetActorScale().X));
		CubeLet->SetCubeOwner(this);
		CubeLets.Push(CubeLet);
		CubeLet->SetCubeLetID(CubeLets.Num()-1);
		CubeLet->SetColorX(CubeLetInfo.ColorX);
		CubeLet->SetColorY(CubeLetInfo.ColorY);
		CubeLet->SetColorZ(CubeLetInfo.ColorZ);
		CubeLet->SetColorXn(CubeLetInfo.ColorXn);
		CubeLet->SetColorYn(CubeLetInfo.ColorYn);
		CubeLet->SetColorZn(CubeLetInfo.ColorZn);
		
	}
	
	
}

void AMCube::SelectCubeLet(uint32 CubeLetID, bool bIsSelected)
{
	CubeLets[CubeLetID]->MakeItShine(bIsSelected);
	SelectedCubeLetID = bIsSelected ? CubeLetID : -1;
}

AMCubeLet* AMCube::GetSelectedCubeLet()
{
	return SelectedCubeLetID != INDEX_NONE ? CubeLets[SelectedCubeLetID] : nullptr;
}

void AMCube::ActivateRotationPlane( const EMCubePlane& PlaneType)
{
	auto IsPlanarCube = [this, &PlaneType](const AMCubeLet* CubeLet)->bool
	{
		switch (PlaneType)
		{
		case EMCubePlane::XPlane:
			return (CubeLet->GetActorLocation().X - this->GetActorLocation().X) == CubeLets[SelectedCubeLetID]->GetActorLocation().X - this->GetActorLocation().X;

		case EMCubePlane::YPlane:
			return (CubeLet->GetActorLocation().Y - this->GetActorLocation().Y) == CubeLets[SelectedCubeLetID]->GetActorLocation().Y - this->GetActorLocation().Y;

		case EMCubePlane::ZPlane:
			return (CubeLet->GetActorLocation().Z - this->GetActorLocation().Z) == CubeLets[SelectedCubeLetID]->GetActorLocation().Z - this->GetActorLocation().Z;

		default:
			return false;
		}
	};
	
	CurrentPlaneType = PlaneType;
	AMCubeLet* CubeLet = GetSelectedCubeLet();
	for (AMCubeLet* CubeLetInPlane : CubeLets)
	{
		if (IsPlanarCube(CubeLetInPlane))
		{
			SelectedCubeLetsInPlane.Add(CubeLetInPlane);
			CubeLetInPlane->MakeItShine(true);
			const FVector RelativeLocation = CubeLetInPlane->GetRootComponent()->GetComponentLocation() - GetRootComponent()->GetComponentLocation();
			CubeLetInPlane->GetRootComponent()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			//CubeLetInPlane->GetRootComponent()->SetRelativeLocation(RelativeLocation, false, nullptr, ETeleportType::TeleportPhysics);
			CubeLetInPlane->GetCoreMesh()->WeldTo(GetCoreMesh());
		}
	}
	
}

void AMCube::DeActivateRotationPlane()
{
	if (SelectedCubeLetsInPlane.IsEmpty())
	{
		return;
	}

	this->GetCoreMesh()->UnWeldChildren();
	for (auto CubeLetInPlane : SelectedCubeLetsInPlane)
	{
		CubeLetInPlane->MakeItShine(false);
		CubeLetInPlane->GetCoreMesh()->UnWeldFromParent();
		CubeLetInPlane->GetCoreMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		CubeLetInPlane->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		CubeLetInPlane->GetCoreMesh()->AttachToComponent(CubeLetInPlane->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		//CubeLetInPlane->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
	SelectedCubeLetsInPlane.Empty();
	CurrentPlaneType = EMCubePlane::AllPlanes;
}

TArray<AMCubeLet*> AMCube::GetAllCubeLetsInPlane(EMCubePlane PlaneType, double PlaneOffset)
{
	TArray<AMCubeLet*> CubeLetsInPlane;
	auto IsPlanarCube = [this, &CubeLetsInPlane, &PlaneType, &PlaneOffset](const AMCubeLet* CubeLet)->bool
	{
		switch (PlaneType)
		{
			case EMCubePlane::XPlane:
				return (CubeLet->GetActorLocation().X - this->GetActorLocation().X) == PlaneOffset;

			case EMCubePlane::YPlane:
				return (CubeLet->GetActorLocation().Y - this->GetActorLocation().Y) == PlaneOffset;

			case EMCubePlane::ZPlane:
				return (CubeLet->GetActorLocation().Z - this->GetActorLocation().Z) == PlaneOffset;

			default:
				return false;
		}
	};
	for (AMCubeLet* CubeLet : CubeLets)
	{
		if (IsPlanarCube(CubeLet))
		{
			CubeLetsInPlane.Add(CubeLet);
		}
	}
	return CubeLetsInPlane;
}


