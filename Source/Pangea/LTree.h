// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshActor.h"
#include "LTree.generated.h"

/**
 * 
 */
UCLASS()
class PANGEA_API ALTree : public AProceduralMeshActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "LSystem", meta = (ClampMin = 0))
	int32 Iterations = 1;

	UPROPERTY(EditAnywhere, Category = "LSystem", meta = (ClampMin = -360.0f, ClampMax = 360.0f))
	float Angle = 90.0f;

	UPROPERTY(EditAnywhere, Category = "LSystem", meta = (ClampMin = 1.0f))
	float SectionLength = 10.0f;

	UPROPERTY(EditAnywhere, Category = "LSystem")
	TArray<FString> Rules;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LSystem")
	int32 SectionsCount;

	UFUNCTION(BlueprintCallable, Category = "LTree")
	void UpdateLString();

	virtual void Tick(float DeltaSecond) override;
protected:
	LSystem lSystem;

	bool ValidateRules(const TArray<FString>& pRules) const;

private:
	int32 prevIterations;
	float prevAngle;
	float prevSectionLength;
	TArray<FString> prevRules;

	bool AreRulesSame();
};
