// Fill out your copyright notice in the Description page of Project Settings.

#include "LSystem.h"

LSystem::LSystem()
{
}

LSystem::~LSystem()
{
}

FString LSystem::GetLString(uint32 Iterations, const TArray<FString>& Rules)
{
	FString result = "";
	if (Rules.Num() > 0)
	{
		result = Rules[0];

		FString lhs;
		FString rhs;

		// looping for number of iterations
		for (uint32 itr = 0; itr < Iterations; ++itr)
		{
			// replacing the lhs of rule equation from axiom with rhs
			for (int32 ruleCount = 1; ruleCount < Rules.Num(); ++ruleCount)
			{
				FString rule = Rules[ruleCount];
				
				// separating the lhs from rhs in rule
				if (rule.Split(FString("="), &lhs, &rhs, ESearchCase::CaseSensitive, ESearchDir::FromStart))
				{
					// splitting successful. valid rule
					result = result.Replace(*lhs, *rhs, ESearchCase::CaseSensitive);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Invalid rule (%d): %s"), ruleCount + 1, *rule);
				}
			}
			
		}
	}

	return result;
}

// rules 
// F : move forward and draw line forward
// f : move forward without drawing line forward
// + : turn left by angle, using rotation matrix Ru(-angle)
// - : turn right by angle, using rotation matrix Ru(angle)
// & : pitch down by angle, using rotation matrix Rl(-angle)
// ^ : pitch up by angle, using rotation matrix Rl(angle)
// \ : roll left by angle, using rotation matrix Rh(-angle)
// / : roll right by angle, using rotation matrx rh(angle)
// | : turn around using rotation matrix Ru(-180)
// [ : Push the current state of the turtle onto a pushdown
//     operations stack.The information saved on the stack contains the
//     turtle's position and orientation, and possibly other attributes
//     such as the color and width of lines being drawn.
// ] : Pop a state from the stack and make it the current state
//     of the turtle.No line is drawn, although in general the
//     position of the turtle changes.
// Ru(angle) : 
//             cos(angle)  sin(angle)      0
//            -sin(angle)  cos(angle)      0
//                0            0           1
// Rl(angle):
//            cos(angle)      0      -sin(angle)
//                0           1           0
//            sin(angle)      0      cos(angle)
// Rh(angle)
//                1           0           0
//                0      cos(angle)   -sin(angle)
//                0      sin(angle)    cos(angle)
LQuad* LSystem::GenerateQuad(FString pLString, float pAngle, float pLineLength)
{
	LQuad* quad = new LQuad();

	FVector unitDirectionVector = FVector::UpVector;
	LLine currentLine = LLine(FVector::ZeroVector, FVector::ZeroVector);

	for (int32 i = 0; i < pLString.Len(); ++i)
	{
		char operation = pLString[i];
		//UE_LOG(LogTemp, Warning, TEXT("Operation: %c"), operation);
		switch (operation)
		{
		case 'F': {
				FVector start = currentLine.End;
				FVector end = start + unitDirectionVector * pLineLength;
				LLine* tmpLine = new LLine(start, end);
				
				currentLine = *tmpLine;

				quad->AddLine(*tmpLine);
			}
			break;
		case 'f': {
				FVector start = currentLine.End;
				FVector end = start + unitDirectionVector * pLineLength;
				LLine tmpLine(start, end);

				currentLine = tmpLine;
			}
			break;
		case '+': {
				unitDirectionVector = unitDirectionVector.RotateAngleAxis(pAngle, FVector(0, 1, 0));
			}
			break;
		case '-': {
				unitDirectionVector = unitDirectionVector.RotateAngleAxis(-pAngle, FVector(0, 1, 0));
			}
			break;
		case '[':
			{
				LineStack.Push(LLineBranch(currentLine, unitDirectionVector));
			}
			break;
		case ']':
			{
				LLineBranch branch = LineStack.Pop();
				currentLine = branch.Line;

				unitDirectionVector = branch.DirectionAtBranching;
			}
			break;
		}
	}

	return quad;
}