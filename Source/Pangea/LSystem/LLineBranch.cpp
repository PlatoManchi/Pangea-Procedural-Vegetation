// Fill out your copyright notice in the Description page of Project Settings.

#include "LLineBranch.h"

LLineBranch::LLineBranch(LLine pLine, FVector pDirection) :
	Line(pLine), DirectionAtBranching(pDirection)
{
}

LLineBranch::~LLineBranch()
{
}
