// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMeshActor.h"


// Sets default values
AProceduralMeshActor::AProceduralMeshActor() : sections(2)
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

void AProceduralMeshActor::DrawLine(FVector StartLocation, FVector EndLocation, float Radius, int32 Section)
{
	// Populating the maps
	if (!vertexMap.Contains(Section))
	{
		TArray<FVector> vertexData;
		vertexMap.Add(Section, vertexData);
	}

	if (!indexMap.Contains(Section))
	{
		TArray<int32> indexData;
		indexMap.Add(Section, indexData);
	}

	if (!normalMap.Contains(Section))
	{
		TArray<FVector> normalData;
		normalMap.Add(Section, normalData);
	}

	if (!uvMap.Contains(Section))
	{
		TArray<FVector2D> uvData;
		uvMap.Add(Section, uvData);
	}

	if (!vertexColorMap.Contains(Section))
	{
		TArray<FLinearColor> vertexColorData;
		vertexColorMap.Add(Section, vertexColorData);
	}

	if (!tangentsMap.Contains(Section))
	{
		TArray<FProcMeshTangent> tangentData;
		tangentsMap.Add(Section, tangentData);
	}

	// getting the array
	TArray<FVector>& vertices = vertexMap[Section];
	TArray<int32>& indices = indexMap[Section];
	TArray<FVector>& normals = normalMap[Section];
	TArray<FVector2D>& UV0 = uvMap[Section];
	TArray<FLinearColor>& vertexColors = vertexColorMap[Section];
	TArray<FProcMeshTangent>& tangents = tangentsMap[Section];


	FVector direction = EndLocation - StartLocation;
	direction.Normalize();
	FVector perpendicularVector = direction.RightVector;

	float angle = 0.0f;
	float angleStep = 360.0f / edgeCount;

	// top of cylinder
	int32 offsetVertexCount = vertices.Num();
	int32 topStarting = offsetVertexCount;

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
			indices.Add(offsetVertexCount);
			
			indices.Add(offsetVertexCount + i + 2);
			indices.Add(offsetVertexCount + i + 1);
		}
		else
		{
			indices.Add(offsetVertexCount);
			indices.Add(offsetVertexCount + 1);
			indices.Add(offsetVertexCount + i + 1);
			
		}
		UV0.Add(FVector2D(sinValue / 2.0f + 0.5f, cosValue / 2.0f + 0.5f));
		vertexColors.Add(FLinearColor(1, 1, 1, 1.0));
		tangents.Add(FProcMeshTangent(0, 1, 0));
	}

	// bottom of cylinder
	offsetVertexCount = vertices.Num();
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


	// sides of cylinder
	int32 top = topStarting + 1;
	int32 bottom = topStarting + edgeCount + 2;
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
}

void AProceduralMeshActor::Finish()
{
	for (auto& element : vertexMap)
	{
		int32 Section = element.Key;
		
		proceduralMeshComponent->CreateMeshSection_LinearColor(Section, vertexMap[Section], indexMap[Section], normalMap[Section], uvMap[Section], vertexColorMap[Section], tangentsMap[Section], false);
	}
}

void AProceduralMeshActor::ClearMesh()
{
	proceduralMeshComponent->ClearAllMeshSections();
	for (auto& element : vertexMap)
	{
		int32 Section = element.Key;

		vertexMap[Section].Empty();
		indexMap[Section].Empty();
		normalMap[Section].Empty();
		uvMap[Section].Empty();
		vertexColorMap[Section].Empty();
		tangentsMap[Section].Empty();
	}
	sections = 0;
}