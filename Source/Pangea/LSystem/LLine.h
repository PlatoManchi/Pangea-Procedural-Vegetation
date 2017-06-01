// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Line has start and end points.
 */
class PANGEA_API LLine
{
public:
	/**
		Store the start and end of line
	*/
	FVector Start, End;

	/**
		Default constructor.
	*/
	LLine(FVector pStart, FVector pEnd);

	/**
		Default destructor.
	*/
	~LLine();
};
