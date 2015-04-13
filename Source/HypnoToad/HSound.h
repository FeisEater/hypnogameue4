// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class HYPNOTOAD_API HSound
{
public:
	HSound(FVector origin, float range) : m_origin(origin), m_range(range) {}

	static void BroadCastSound(HSound& sound);

	bool operator==(const HSound& rhs)
	{
		return Compare(rhs);
	}

protected:
	FVector m_origin;
	float m_range;

private:
	virtual bool Compare(const HSound& rhs) = 0;
};

class HWord : public HSound
{
public:
	HWord(FVector origin, FString con) : HSound(origin, 700), content(con) {}
	FString content;

private:
	virtual bool Compare(const HSound& rhs);
};

class HGunShot : public HSound
{
public:
	HGunShot(FVector origin) : HSound(origin, 5000) {}

private:
	virtual bool Compare(const HSound& rhs);
};