// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

enum class HSoundType
{
	None,
	Word,
	GunShot
};

class HYPNOTOAD_API HSound
{
public:
	HSound(FVector origin, float range) : m_origin(origin), m_range(range), m_type(HSoundType::None) {}
	virtual ~HSound() {}

	static void BroadCastSound(UWorld* world, HSound* sound);

	bool operator==(const HSound* rhs)
	{
		return Compare(rhs);
	}

	HSoundType GetType() const
	{
		return m_type;
	}

	FVector GetOrigin() const
	{
		return m_origin;
	}

protected:
	FVector m_origin;
	float m_range;
	HSoundType m_type;

private:
	virtual bool Compare(const HSound* rhs) = 0;
};

class HWord : public HSound
{
public:
	HWord(FVector origin, FString con) : HSound(origin, 700), content(con)
	{
		m_type = HSoundType::Word;
	}
	virtual ~HWord() {}

	FString content;

private:
	virtual bool Compare(const HSound* rhs);
};

class HGunShot : public HSound
{
public:
	HGunShot(FVector origin) : HSound(origin, 5000)
	{
		m_type = HSoundType::GunShot;
	}
	virtual ~HGunShot() {}

private:
	virtual bool Compare(const HSound* rhs);
};