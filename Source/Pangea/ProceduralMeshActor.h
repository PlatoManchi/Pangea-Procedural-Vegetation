// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "LSystem/LSystem.h"
#include "ProceduralMeshActor.generated.h"

UCLASS()
class PANGEA_API AProceduralMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
		Procedural mesh component.
	*/
	UPROPERTY(VisibleAnywhere, Category = "GeneratedMesh")
	UProceduralMeshComponent* proceduralMeshComponent;

	/**
		The edge count for the cylinder that is used to draw the line.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeneratedMesh", meta = (ClampMin = 2))
	int32 edgeCount = 10;

	/**
		Radius of the line.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeneratedMesh", meta = (ClampMin = 0.0001f))
	float radius = 1.0f;

	/**
		Height of the cylinder
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeneratedMesh", meta = (ClampMin = 0.0001f))
	float height = 100.0f;

	// Sets default values for this actor's properties
	AProceduralMeshActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	int32 sections;

	/**
		Generate the mesh with vertex and index buffer provided.
	*/
	UFUNCTION(BlueprintCallable, Category = "GeneratedMesh")
	void GenerateMesh();

	/**
		Draw line in between two locations. Creates a cylinder between lines.
		@param StatLocation FVector that holds the start location
		@param EndLocation FVector that holds the end location.
		@param Radius Float that holds the radius.
	*/
	void DrawLine(FVector StartLocation, FVector EndLocation, float Radius, int32 Section = 0);

	/**
		This has to be called for the mesh to get updated.
	*/
	void Finish();

	/**
		Remove all previous data from current mesh
	*/
	void ClearMesh();
private:
	LSystem lSystem;

	TMap<int32, TArray<FVector>> vertexMap;
	TMap<int32, TArray<int32>> indexMap;
	TMap<int32, TArray<FVector>> normalMap;
	TMap<int32, TArray<FVector2D>> uvMap;
	TMap<int32, TArray<FLinearColor>> vertexColorMap;
	TMap<int32, TArray<FProcMeshTangent>> tangentsMap;
};
