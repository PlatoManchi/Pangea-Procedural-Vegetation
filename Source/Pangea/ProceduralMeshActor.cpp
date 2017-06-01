// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMeshActor.h"


// Sets default values
AProceduralMeshActor::AProceduralMeshActor() : sections(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	proceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = proceduralMeshComponent;
}

// Called when the game starts or when spawned
void AProceduralMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AProceduralMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMeshActor::GenerateMesh()
{
	
}

void AProceduralMeshActor::DrawLine(FVector StartLocation, FVector EndLocation, float Radius)
{
	FVector direction = EndLocation - StartLocation;
	direction.Normalize();
	FVector perpendicularVector = direction.RightVector;

	TArray<FVector> vertices;
	TArray<int32> indices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;

	float angle = 0.0f;
	float angleStep = 360.0f / edgeCount;

	// top
	vertices.Add(EndLocation);
	UV0.Add(FVector2D(0.5, 0.5));
	for (int32 i = 0; i < edgeCount; ++i)
	{
		angle = angleStep * i;
		float sinValue = FMath::Sin(angle);
		float cosValue = FMath::Cos(angle);

		FVector newVector = perpendicularVector.RotateAngleAxis(angle, direction);

		vertices.Add(EndLocation + newVector);
		normals.Add(FVector(0, 0, 1));

		if (i != edgeCount - 1)
		{
			indices.Add(0);
			
			indices.Add(i + 2);
			indices.Add(i + 1);
		}
		else
		{
			indices.Add(0);
			indices.Add(1);
			indices.Add(i + 1);
			
		}
		UV0.Add(FVector2D(sinValue / 2.0f + 0.5f, cosValue / 2.0f + 0.5f));
		vertexColors.Add(FLinearColor(1, 1, 1, 1.0));
		tangents.Add(FProcMeshTangent(0, 1, 0));
	}

	// bottom
	int32 offsetVertexCount = vertices.Num();
	vertices.Add(StartLocation);
	UV0.Add(FVector2D(0.5, 0.5));

	for (int32 i = 0; i < edgeCount; ++i)
	{
		angle = angleStep * i;
		float sinValue = FMath::Sin(angle);
		float cosValue = FMath::Cos(angle);

		FVector newVector = perpendicularVector.RotateAngleAxis(angle, direction);
		vertices.Add(StartLocation + newVector);
		normals.Add(FVector(0, 0, -1));

		if (i != edgeCount - 1)
		{
			indices.Add(offsetVertexCount);
			indices.Add(offsetVertexCount + i + 1);
			indices.Add(offsetVertexCount + i + 2);
			
		}
		else
		{
			indices.Add(offsetVertexCount);
			indices.Add(offsetVertexCount + i + 1);
			indices.Add(offsetVertexCount + 1);
			
		}
		UV0.Add(FVector2D(sinValue / 2.0f + 0.5f, cosValue / 2.0f + 0.5f));
		vertexColors.Add(FLinearColor(1, 1, 1, 1.0));
		tangents.Add(FProcMeshTangent(0, 1, 0));
	}


	// edges
	int32 top = 1;
	int32 bottom = edgeCount + 2;
	for (int32 i = 0; i < edgeCount; ++i)
	{
		if (i != edgeCount - 1)
		{
			indices.Add(top + i);
			indices.Add(bottom + i + 1);
			indices.Add(bottom + i);
			

			indices.Add(bottom + i + 1);
			indices.Add(top + i);
			indices.Add(top + i + 1);
			
		}
		else
		{
			indices.Add(top + i);
			indices.Add(bottom);
			indices.Add(bottom + i);
			

			indices.Add(bottom);
			indices.Add(top + i);
			indices.Add(top);
			
		}
	}
	
	proceduralMeshComponent->CreateMeshSection_LinearColor(sections, vertices, indices, normals, UV0, vertexColors, tangents, false);
	sections++;
}
