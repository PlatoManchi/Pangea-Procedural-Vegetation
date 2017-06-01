// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLine.h"
/**
 * Contains list of lines that make up this quad.
 */
class PANGEA_API LQuad
{
public:
	/**
		Default constructor.
	*/
	LQuad();

	/**
		Default destructor.
	*/
	~LQuad();

	/**
		Adds the line into this quad. The Quad will take owner ship of line.
		@param pLine A constant reference to heap allocated LLine object.
	*/
	void AddLine(const LLine& pLine);

	/**
		Return line at an index;
		@param pIndex Unsigned int that holds the index.
		@return Constant reference to line object.
	*/
	const LLine& GetLine(uint32 pIndex) const;

	/**
		Return the number of lines in this quad.
		@return unsigned int that holds the number of lines in quad.
	*/
	uint32 LineCount() const;
private:

	/**
		Vector that holds all the lines in this quad
	*/
	TArray<const LLine*> LineList;
};
