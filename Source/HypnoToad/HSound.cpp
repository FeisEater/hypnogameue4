// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HSound.h"

bool HWord::Compare(const HSound& rhs)
{
	const HWord* pRhsWord = dynamic_cast<const HWord*>(&rhs);
	if (!pRhsWord)	return false;
	return content == pRhsWord->content;
}

bool HGunShot::Compare(const HSound& rhs)
{
	const HGunShot* pRhsShot = dynamic_cast<const HGunShot*>(&rhs);
	return pRhsShot ? true : false;
}
