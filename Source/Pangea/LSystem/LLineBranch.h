// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLine.h"
/**
 * 
 */
class PANGEA_API LLineBranch
{
public:
	/**
		Pointer to line just before branching.
	*/
	LLine Line;

	/**
		Direction at which turtle is facing before branching.
	*/
	FVector DirectionAtBranching;

	/**
		Constructor.
		@param pLine Pointer to line just before branching.
		@param pDirection Vector that holds the direction turtle is facing.
	*/
	LLineBranch(LLine pLine, FVector pDirection);

	/**
		Default branching.
	*/
	~LLineBranch();
};
