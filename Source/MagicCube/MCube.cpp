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

void AMCube::UnSelectCubeLet()
{
	DeActivateRotationPlane();
	SelectedCubeLetID = -1;
}

void AMCube::ActivateRotationPlane( const EMCubePlane& PlaneType)
{
	auto IsPlanarCube = [this, &PlaneType](const AMCubeLet* CubeLet)->bool
	{
		switch (PlaneType)
		{
			int RelativeDimension, RelativeDimension0;
		case EMCubePlane::XPlane:
			RelativeDimension = round(CubeLet->GetActorLocation().X - this->GetActorLocation().X);
			RelativeDimension0 = round(CubeLets[SelectedCubeLetID]->GetActorLocation().X - this->GetActorLocation().X);
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColorList::Coral, FString::Printf(TEXT("Is same Plane: RelativeX = %d, RelativeX0 = %d"), RelativeDimension, RelativeDimension0));
			return RelativeDimension == RelativeDimension0;
			
		case EMCubePlane::YPlane:
			RelativeDimension  = round(CubeLet->GetActorLocation().Y - this->GetActorLocation().Y);
			RelativeDimension0 = round(CubeLets[SelectedCubeLetID]->GetActorLocation().Y - this->GetActorLocation().Y);
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColorList::Coral, FString::Printf(TEXT("Is same Plane: RelativeY = %d, RelativeY0 = %d"), RelativeDimension, RelativeDimension0));
			return RelativeDimension == RelativeDimension0;
			
		case EMCubePlane::ZPlane:
			RelativeDimension = round(CubeLet->GetActorLocation().Z - this->GetActorLocation().Z);
			RelativeDimension0 = round(CubeLets[SelectedCubeLetID]->GetActorLocation().Z - this->GetActorLocation().Z);
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColorList::Coral, FString::Printf(TEXT("Is same Plane: RelativeZ = %d, RelativeZ0 = %d"), RelativeDimension, RelativeDimension0));
			return RelativeDimension == RelativeDimension0;
			
		default:
			return false;
		}
	};
	auto JoinCubeLetsInPlane = [this](AMCubeLet* CubeLetInPlane)
	{
		SelectedCubeLetsInPlane.Add(CubeLetInPlane);
		CubeLetInPlane->MakeItShine(true);
		FVector RelativeLocation = CubeLetInPlane->GetRootComponent()->GetRelativeLocation() - GetRootComponent()->GetRelativeLocation();
		CubeLetInPlane->GetRootComponent()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		CubeLetInPlane->GetCoreMesh()->WeldTo(GetCoreMesh(), NAME_None, true);
	};
	CurrentPlaneType = PlaneType;
	AMCubeLet* CubeLet = GetSelectedCubeLet();
	for (AMCubeLet* CubeLetInPlane : CubeLets)
	{
		if (CubeLet != CubeLetInPlane && IsPlanarCube(CubeLetInPlane))
		{
			JoinCubeLetsInPlane(CubeLetInPlane);
		}
	}
	JoinCubeLetsInPlane(CubeLet);
	
	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + GetRootComponent()->GetForwardVector()*500, 500, FColorList::Green, false, 300, 5);
	//DrawDebugDirectionalArrow(GetWorld(), CubeLets[SelectedCubeLetID]->GetActorLocation(), CubeLets[SelectedCubeLetID]->GetActorLocation() + CubeLets[SelectedCubeLetID]->GetActorForwardVector()*500, 500, FColorList::Red, false, 300, 5);
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
	//fix - we need to reset the root component back to the rest position so that the rotation axes for root always goes back to home position
	GetRootComponent()->SetRelativeRotation(FRotator(0, 0, 0));
	CurrentPlaneType = EMCubePlane::AllPlanes;
}


void AMCube::SnapToAlignment()
{
	if (!IsValid(GetSelectedCubeLet()))
	{
		return;
	}
	auto SetRotationValue = [](double& RotationValue, const EMCubePlane& ValidRotationPlane, const EMCubePlane& CurrentRotationPlane)
	{
		int Delta = static_cast<int>(RotationValue) % 90;
		RotationValue = Delta >= 0 ? (Delta > 45 ? 90 - Delta : -Delta) : (Delta < -45 ? -90 - Delta : - Delta);
	};
	
	FRotator RelativeRotation = GetRootComponent()->GetRelativeRotation();
	SetRotationValue(RelativeRotation.Pitch, EMCubePlane::YPlane, CurrentPlaneType);
	SetRotationValue(RelativeRotation.Yaw, EMCubePlane::ZPlane, CurrentPlaneType);
	SetRotationValue(RelativeRotation.Roll, EMCubePlane::XPlane, CurrentPlaneType);
	GetRootComponent()->AddRelativeRotation(RelativeRotation);
}


void AMCube::RotateCube(const float& PlaneRelativeCoordinate)
{
	if (!IsValid(GetSelectedCubeLet()))
	{
		return;
	}
	auto SetRotationValue = [&PlaneRelativeCoordinate](double& RotationValue, const EMCubePlane& ValidRotationPlane, const EMCubePlane& CurrentRotationPlane)
	{
		const int Sign = PlaneRelativeCoordinate == 0 ? 0 : PlaneRelativeCoordinate > 0 ? 1 : -1;
		RotationValue = CurrentRotationPlane == ValidRotationPlane ? Sign * 2.f : 0.f;
	};
		
	FRotator DeltaRot = FRotator(0, 0, 0);

	auto GetRotationType = [this]()-> void
	{
		const FRotator& RRR = CubeLets[SelectedCubeLetID]->GetRootComponent()->GetRelativeRotation();
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColorList::BrightGold, FString::Printf(TEXT("Relative Root Rotation: Pitch: %f, Yaw: %f, Roll: %f"), RRR.Pitch, RRR.Yaw, RRR.Roll));
	};
	GetRotationType();
	SetRotationValue(DeltaRot.Pitch, EMCubePlane::YPlane, CurrentPlaneType);
	SetRotationValue(DeltaRot.Yaw, EMCubePlane::ZPlane, CurrentPlaneType);
	SetRotationValue(DeltaRot.Roll, EMCubePlane::XPlane, CurrentPlaneType);

	GetRootComponent()->AddRelativeRotation(DeltaRot);
	
}

void AMCube::SnapToClosestRubicPlane()
{
	if (!IsValid(GetSelectedCubeLet()))
	{
		return;
	}
	
	FRotator CurrentRot = GetRootComponent()->GetRelativeRotation();
	auto QuantizeRotation = [](double& RotationValue)
	{
		float AbsRotationValue = abs(RotationValue);
		if (static_cast<int>(AbsRotationValue) % 90 <= 2.5)
		{
			int value = static_cast<int>(AbsRotationValue - (static_cast<int>(AbsRotationValue) % 90));
			RotationValue = RotationValue >= 0.f ? value : - value; 
		}
	};
	
	QuantizeRotation(CurrentRot.Pitch);
	QuantizeRotation(CurrentRot.Yaw);
	QuantizeRotation(CurrentRot.Roll);
	GetRootComponent()->SetRelativeRotation(CurrentRot, false, nullptr, ETeleportType::TeleportPhysics);
}



