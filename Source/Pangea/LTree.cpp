// Fill out your copyright notice in the Description page of Project Settings.

#include "LTree.h"




void ALTree::UpdateLString()
{
	if (prevIterations != Iterations || prevAngle != Angle || prevSectionLength != SectionLength || !AreRulesSame())
	{
		// if any value changed
		if (ValidateRules(Rules))
		{
			proceduralMeshComponent->ClearAllMeshSections();
			sections = 0;

			// if the rules are valid
			FString result = lSystem.GetLString(Iterations, Rules);
			UE_LOG(LogTemp, Warning, TEXT("Building the LTree. (%s)"), *result);
			LQuad* quad = lSystem.GenerateQuad(result, Angle, SectionLength);

			for (uint32 i = 0; i < quad->LineCount(); ++i)
			{
				const LLine& line = quad->GetLine(i);
				// draw each line in quad
				DrawLine(line.Start, line.End, 5, i % 2);
			}
			
			SectionsCount = quad->LineCount();
			delete quad;
		}

		Finish();

		prevIterations = Iterations;
		prevAngle = Angle;
		prevSectionLength = SectionLength;
		prevRules = Rules;
	}
}

bool ALTree::ValidateRules(const TArray<FString>& pRules) const
{
	if (pRules.Num() == 0) return false;
	// first rule is axiom which doesn't need = in it
	for (int32 i = 1; i < pRules.Num(); ++i)
	{
		if (!pRules[i].Contains("="))
		{
			// if rule doesn't have = its invalid
			return false;
		}
	}

	return true;
}

bool ALTree::AreRulesSame()
{
	if (prevRules.Num() != Rules.Num()) return false;

	for (int i = 0; i < prevRules.Num(); ++i)
	{
		if (prevRules[i].Compare(Rules[i]) != 0)
		{
			return false;
		}
	}

	return true;
}

void ALTree::Tick(float DeltaSecond)
{
	UpdateLString();
}