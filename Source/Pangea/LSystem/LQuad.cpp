// Fill out your copyright notice in the Description page of Project Settings.

#include "LQuad.h"

LQuad::LQuad()
{
}

LQuad::~LQuad()
{
	for (const LLine* line : LineList)
	{
		delete line;
	}

	LineList.Empty();
}

void LQuad::AddLine(const LLine& pLine)
{
	LineList.Add(&pLine);
}

const LLine& LQuad::GetLine(uint32 pIndex) const
{
	return *LineList[pIndex];
}

uint32 LQuad::LineCount() const
{
	return LineList.Num();
}