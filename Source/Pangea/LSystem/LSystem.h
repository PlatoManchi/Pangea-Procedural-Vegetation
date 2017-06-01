// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LQuad.h"
#include "LLineBranch.h"
/**
 * 
 */
class PANGEA_API LSystem
{
public:
	/**
		Default constructor
	*/
	LSystem();

	/**
		Default destructor
	*/
	~LSystem();

	/**
		Returns the turtle representation of the rules after iterating rules.
		@param Iterations Num of iterations
		@param Rules A array of strings that holds the rules.
	*/
	FString GetLString(uint32 Iterations, const TArray<FString>& Rules);

	/**
		Generate a quad using the L-String given as input.
		@param pLString FString that holds the L-String that needs to be processed.
		@param pAngle 
		@param pLineLenth Length of each turtle step.
	*/
	LQuad* GenerateQuad(FString pLString, float pAngle, float pLineLength = 20.0f);

private:
	/**
		Holds the stack for branching.
	*/
	TArray<LLineBranch> LineStack;
};
