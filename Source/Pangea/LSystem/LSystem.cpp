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
	// quad output 
	LQuad* quad = new LQuad();

	// initial direction. 
	FVector unitDirectionVector = FVector::UpVector;
	
	// first line
	LLine* currentLine = new LLine(FVector::ZeroVector, FVector::ZeroVector);
	quad->AddLine(currentLine);

	// temp data used to 
	LLine* tmpLine = new LLine(FVector::ZeroVector, FVector::ZeroVector);
	TArray<LLine*> toDelete;
	
	// this will have the last position of the turtle
	FVector lastKnownTurtlePosition = FVector::ZeroVector;
	
	// looping through all the operations
	for (int32 i = 0; i < pLString.Len(); ++i)
	{
		char operation = pLString[i];

		//UE_LOG(LogTemp, Warning, TEXT("Operation: %c"), operation);
		switch (operation)
		{
		case 'F': {
				// batching the lines improve performance alot.
				FVector direction = currentLine->End - currentLine->Start;
				direction.Normalize();

				if (!unitDirectionVector.Equals(direction))
				{
					// create new line only if the direction of the previous line is not same as current line.
					currentLine = new LLine(lastKnownTurtlePosition, lastKnownTurtlePosition);
					quad->AddLine(currentLine);
						
				}
				
				currentLine->End = currentLine->End + unitDirectionVector * pLineLength;
				lastKnownTurtlePosition = currentLine->End;
			}
			break;
		case 'f': {
				// batching the lines improve performance alot.
				FVector direction = currentLine->End - currentLine->Start;
				direction.Normalize();
				if (!unitDirectionVector.Equals(direction))
				{
					currentLine = new LLine(lastKnownTurtlePosition, lastKnownTurtlePosition);
					toDelete.Add(currentLine);
				}
				tmpLine->Start = currentLine->End;
				tmpLine->End = tmpLine->Start + unitDirectionVector * pLineLength;
				
				currentLine->Start = tmpLine->Start;
				currentLine->End = tmpLine->End;

				lastKnownTurtlePosition = currentLine->End;
			}
			break;
		case '+': {
				// rotating direction on Y-Axis
				unitDirectionVector = unitDirectionVector.RotateAngleAxis(pAngle, FVector(0, 1, 0));
				lastKnownTurtlePosition = currentLine->End;
			}
			break;
		case '-': {
				// rotating direction on Y-Axis
				unitDirectionVector = unitDirectionVector.RotateAngleAxis(-pAngle, FVector(0, 1, 0));
				lastKnownTurtlePosition = currentLine->End;
			}
			break;
		case '[':
			{
				// push current line and direction vector on to stack.
				lastKnownTurtlePosition = currentLine->End;
				LineStack.Push(LLineBranch(currentLine, unitDirectionVector));
			}
			break;
		case ']':
			{
				// pop the line from line branching stack.
				LLineBranch branch = LineStack.Pop();
				currentLine = branch.Line;

				unitDirectionVector = branch.DirectionAtBranching;
				lastKnownTurtlePosition = currentLine->End;
			}
			break;
		}
	}

	// freeing memory used
	delete tmpLine;
	for (LLine* var : toDelete)
	{
		delete var;
	}

	return quad;
}